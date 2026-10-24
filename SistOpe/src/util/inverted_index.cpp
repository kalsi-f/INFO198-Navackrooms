#include "config/EnvConfig.h"
#include "InvertedIndex.h"
#include "Menu.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// lee una variable de entorno con una ruta y valida que no este vacia
// si no existe, getEnvVar avisa y termina el programa
string getPathVar(const string& key) {
    string value = getEnvVar(key);
    if (value.empty()) {
        throw runtime_error("la variable de entorno " + key + " esta vacia.");
    }
    return value;
}

// valida que exista la carpeta del archivo antes de procesar los libros
void checkOutputFolder(const string& path) {
    filesystem::path folder = filesystem::path(path).parent_path();
    if (!folder.empty() && !filesystem::is_directory(folder)) {
        throw runtime_error("la carpeta para '" + path + "' no existe.");
    }
}

void createInvertedIndex() {
    // validar variables de entorno antes de empezar
    int nThreads = parseThreadCount(getEnvVar("N_THREADS"));
    string booksDir = getPathVar("BOOKS_DIR");
    string bookMapFile = getPathVar("BOOK_MAP_FILE");
    string indexFile = getPathVar("INVERTED_INDEX_FILE");
    checkOutputFolder(bookMapFile);
    checkOutputFolder(indexFile);

    // revisar la carpeta de libros
    vector<Book> books = findBooks(booksDir);
    cout << "Carpeta de libros: " << booksDir << " (" << books.size() << " libros)" << endl;
    cout << "Threads          : " << nThreads << endl << endl;

    // guardar la asociacion id -> nombre
    saveBookMap(books, bookMapFile);

    // procesamiento concurrente, cola -> threads -> semaforos -> indice
    IndexState state;
    auto start = chrono::steady_clock::now();
    buildInvertedIndex(state, books, nThreads);
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();

    // guardar el indice
    saveInvertedIndex(state.index, indexFile);

    cout << endl << "------------ INDICE INVERTIDO ------------" << endl;
    cout << "Libros procesados : " << state.processedBooks << " de " << books.size() << endl;
    cout << "Libros con error  : " << state.failedBooks << endl;
    cout << "Palabras distintas: " << state.index.size() << endl;
    cout << "Tiempo            : " << elapsed << " ms" << endl;
    cout << "Indice            : " << indexFile << endl;
    cout << "Mapeo de libros   : " << bookMapFile << endl;
    cout << "------------------------------------------" << endl;
}

int main() {
    loadEnv();

    vector<string> options = {
        "Volver",
        "Crear indice invertido"
    };
    int option = -1;

    while (option != 0) {
        clearScreen();
        cout << "========================================" << endl
             << "          CREAR INDICE INVERTIDO" << endl
             << "========================================" << endl;

        option = simple_menu(options);

        // se corto la entrada (EOF), se sale igual que con 0
        if (!cin) break;
        if (option != 1) continue;

        try {
            createInvertedIndex();
        } catch (const exception& e) {
            cerr << endl << "Error: " << e.what() << endl;
        }

        cout << endl << "Presione ENTER para continuar...";
        cin.get();
    }

    return 0;
}
