#include "InvertedIndex.h"
#include "CountSystem.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <system_error>
#include <thread>

using namespace std;

namespace {

const int MAX_THREADS = 64;

// un libro con id 0 es la marca de fin, le avisa a un thread que no queda trabajo
const int END_OF_WORK = 0;

// agrega un elemento a la cola y avisa que hay uno mas disponible
void pushWork(IndexState& state, const Book& book) {
    // seccion critica, solo un thread a la vez modifica la cola
    sem_wait(&state.queueMutex);
    state.workQueue.push(book);
    sem_post(&state.queueMutex);

    // despierta a un thread que este esperando trabajo
    sem_post(&state.availableBooks);
}

// ciclo de cada thread trabajador, saca un libro -> cuenta -> integra al indice
void worker(IndexState& state, int threadNumber) {
    while (true) {
        // espera a que haya algo en la cola, si esta vacia el thread se bloquea aqui
        sem_wait(&state.availableBooks);

        // seccion critica, sacar un libro de la cola (un libro nunca lo toman dos threads)
        sem_wait(&state.queueMutex);
        Book book = state.workQueue.front();
        state.workQueue.pop();
        sem_post(&state.queueMutex);

        if (book.id == END_OF_WORK) {
            break;
        }

        // leer y contar fuera de las secciones criticas, esta parte corre en paralelo
        map<string, int> frequency;
        try {
            frequency = countWordFrequency(readTextFile(book.path));
        } catch (const exception& e) {
            sem_wait(&state.indexMutex);
            state.failedBooks++;
            sem_post(&state.indexMutex);

            sem_wait(&state.outputMutex);
            cerr << "[Thread " << threadNumber << "] Error en el libro " << book.id
                 << " (" << book.name << "): " << e.what() << endl;
            sem_post(&state.outputMutex);
            continue;
        }

        // seccion critica, integrar el resultado parcial al indice global
        sem_wait(&state.indexMutex);
        for (const auto& entry : frequency) {
            state.index[entry.first].push_back({book.id, entry.second});
        }
        state.processedBooks++;
        sem_post(&state.indexMutex);

        // seccion critica de pantalla
        sem_wait(&state.outputMutex);
        cout << "[Thread " << threadNumber << "] Libro " << book.id << " (" << book.name
             << "): " << frequency.size() << " palabras distintas" << endl;
        sem_post(&state.outputMutex);
    }
}

void destroySemaphores(IndexState& state) {
    sem_destroy(&state.availableBooks);
    sem_destroy(&state.queueMutex);
    sem_destroy(&state.indexMutex);
    sem_destroy(&state.outputMutex);
}

}

int parseThreadCount(const string& value) {
    if (value.empty()) {
        throw runtime_error("N_THREADS no puede estar vacio.");
    }
    for (char character : value) {
        if (character < '0' || character > '9') {
            throw runtime_error("N_THREADS debe ser un numero entero positivo ('" + value + "').");
        }
    }
    long long number;
    try { number = stoll(value); }
    catch (...) { throw runtime_error("N_THREADS esta fuera de rango."); }
    if (number < 1 || number > MAX_THREADS)
        throw runtime_error("N_THREADS debe estar entre 1 y " + to_string(MAX_THREADS) + ".");
    return static_cast<int>(number);
}

vector<Book> findBooks(const string& booksDir) {
    if (!filesystem::exists(booksDir)) {
        throw runtime_error("la carpeta de libros '" + booksDir + "' no existe.");
    }
    if (!filesystem::is_directory(booksDir)) {
        throw runtime_error("'" + booksDir + "' no es una carpeta.");
    }

    vector<filesystem::path> files;
    try {
        for (const filesystem::directory_entry& entry : filesystem::directory_iterator(booksDir)) {
            string name = entry.path().filename().string();

            // solo archivos normales, se ignoran carpetas y ocultos (ej: .DS_Store)
            if (!entry.is_regular_file() || name.empty() || name[0] == '.') {
                continue;
            }

            ifstream test(entry.path());
            if (!test.is_open()) {
                cerr << "Aviso: no se puede leer '" << name << "', se omite." << endl;
                continue;
            }
            files.push_back(entry.path());
        }
    } catch (const filesystem::filesystem_error& e) {
        throw runtime_error("no se puede leer la carpeta '" + booksDir + "': " + e.code().message());
    }

    if (files.empty()) {
        throw runtime_error("la carpeta '" + booksDir + "' no tiene libros para procesar.");
    }

    // orden alfabetico para que cada libro reciba siempre el mismo id
    sort(files.begin(), files.end());

    vector<Book> books;
    for (size_t i = 0; i < files.size(); i++) {
        books.push_back({static_cast<int>(i + 1), files[i].filename().string(), files[i].string()});
    }
    return books;
}

void saveBookMap(const vector<Book>& books, const string& path) {
    ofstream file(path);

    if (!file.is_open()) {
        throw runtime_error("no se pudo crear el archivo de libros '" + path + "'.");
    }

    for (const Book& book : books) {
        file << book.id << ";" << book.name << "\n";
    }
}

void buildInvertedIndex(IndexState& state, const vector<Book>& books, int nThreads) {
    // availableBooks parte en 0 para que los threads esperen trabajo
    // los otros parten en 1 porque son exclusion mutua (un thread a la vez)
    sem_init(&state.availableBooks, 0, 0);
    sem_init(&state.queueMutex, 0, 1);
    sem_init(&state.indexMutex, 0, 1);
    sem_init(&state.outputMutex, 0, 1);

    // crea los threads trabajadores
    vector<thread> workers;
    try {
        for (int i = 1; i <= nThreads; i++) {
            workers.emplace_back(worker, ref(state), i);
        }
    } catch (const system_error& e) {
        // si falla la creacion, se cierran ordenadamente los que ya partieron
        for (size_t i = 0; i < workers.size(); i++) {
            pushWork(state, Book{END_OF_WORK, "", ""});
        }
        for (thread& workerThread : workers) {
            workerThread.join();
        }
        destroySemaphores(state);
        throw runtime_error(string("no se pudieron crear los threads: ") + e.what());
    }

    // el thread principal es el productor, pone cada libro en la cola
    for (const Book& book : books) {
        pushWork(state, book);
    }

    // una marca de fin por thread, como la cola es FIFO salen despues de todos los libros
    for (int i = 0; i < nThreads; i++) {
        pushWork(state, Book{END_OF_WORK, "", ""});
    }

    // espera a que todos los threads terminen
    for (thread& workerThread : workers) {
        workerThread.join();
    }

    destroySemaphores(state);

    // los resultados llegan en cualquier orden, se ordenan por id de libro
    for (auto& entry : state.index) {
        sort(entry.second.begin(), entry.second.end());
    }
}

void saveInvertedIndex(const InvertedIndex& index, const string& path) {
    ofstream file(path);

    if (!file.is_open()) {
        throw runtime_error("no se pudo crear el archivo del indice '" + path + "'.");
    }

    // "\n" en vez de endl porque son cientos de miles de lineas
    for (const auto& entry : index) {
        file << entry.first;
        for (const pair<int, int>& occurrence : entry.second) {
            file << ";(" << occurrence.first << "," << occurrence.second << ")";
        }
        file << "\n";
    }

    if (!file) {
        throw runtime_error("ocurrio un error mientras se escribia el indice '" + path + "'.");
    }
}
