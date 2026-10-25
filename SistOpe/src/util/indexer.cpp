// Orquesta, solo llama modulos.
// Creacion paralela del indice invertido: cola -> threads -> semaforos -> resultados -> indice.

#include "config/EnvConfig.h"
#include "index/BookScanner.h"
#include "index/IndexConfig.h"
#include "index/InvertedIndex.h"
#include "index/WorkQueue.h"
#include "index/Worker.h"

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <semaphore.h>
#include <thread>
#include <unistd.h>

using namespace std;

int main() {
    // Si nos lanza sistope las variables ya vienen heredadas (loadEnv no las sobreescribe);
    // esto permite ademas ejecutar el indexador solo, desde la carpeta SistOpe.
    loadEnv();

    // 1. Configuracion
    IndexConfig config = loadIndexConfig();

    // 2. Libros a procesar
    vector<Book> books = scanBooks(config.booksDir);
    cout << "Libros encontrados: " << books.size() << endl;

    if (!writeBookMap(books, config.bookMapFile)) {
        cerr << "Error: no se pudo escribir el mapeo de libros en '" << config.bookMapFile << "'" << endl;
        return 1;
    }

    // Mas hilos que libros no aporta nada: los sobrantes encontrarian la cola vacia y terminarian
    int nThreads = config.nThreads;
    if (nThreads > (int)books.size()) {
        nThreads = books.size();
        cout << "Aviso: N_THREADS=" << config.nThreads << " es mayor que la cantidad de libros, se usaran "
             << nThreads << " hilos" << endl;
    }

    // 3. Cola de trabajo. Los libros grandes primero: si el mas grande saliera al final,
    // un hilo quedaria trabajando solo mientras los demas ya terminaron.
    sort(books.begin(), books.end(), [](const Book& a, const Book& b) { return a.size > b.size; });

    WorkQueue queue;
    for (const Book& book : books) queue.push(book);

    // 4. Hilos trabajadores
    InvertedIndex index;
    sem_t coutSem;
    sem_init(&coutSem, 0, 1);

    cout << "Procesando con " << nThreads << " hilos..." << endl;
    auto start = chrono::steady_clock::now();

    vector<thread> threads;
    for (int i = 0; i < nThreads; i++) {
        // ref(): thread copia sus argumentos, y queremos que todos usen la MISMA cola e indice
        threads.emplace_back(worker, i + 1, ref(queue), ref(index), ref(coutSem));
    }

    // 5. join bloquea hasta que cada hilo termina. "Cola vacia" no basta: puede haber
    // hilos todavia contando el ultimo libro que sacaron.
    for (thread& t : threads) t.join();

    auto end = chrono::steady_clock::now();
    double seconds = chrono::duration<double>(end - start).count();
    sem_destroy(&coutSem);

    // 6. Escritura del indice. Ya no queda ningun hilo vivo: no hace falta semaforo.
    if (!index.writeTo(config.indexFile)) {
        cerr << "Error: no se pudo escribir el indice en '" << config.indexFile << "'" << endl;
        return 1;
    }

    cout << "Indice creado: " << config.indexFile << " (" << index.wordCount() << " palabras)" << endl;
    cout << "Mapeo de libros: " << config.bookMapFile << endl;
    cout << "Tiempo de procesamiento: " << seconds << " s con " << nThreads << " hilos" << endl;

    // El menu limpia la pantalla al volver: esperamos para que se alcance a leer el resumen.
    // Solo si hay una persona en la terminal, para no trabar ejecuciones desde scripts.
    if (isatty(STDIN_FILENO)) {
        cout << "Presione ENTER para continuar...";
        cin.get();
    }
    return 0;
}
