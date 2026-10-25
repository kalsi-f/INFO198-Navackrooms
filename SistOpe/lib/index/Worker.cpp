#include "index/Worker.h"
#include "index/Tokenizer.h"

#include <iostream>
#include <unordered_map>

using namespace std;

void worker(int threadId, WorkQueue& queue, InvertedIndex& index, sem_t& coutSem) {
    Book book;
    int processed = 0;

    while (queue.tryPop(book)) {
        // Conteo LOCAL: este map es solo de este hilo, aqui no hace falta ningun semaforo.
        // Es la parte pesada y la que realmente corre en paralelo.
        unordered_map<string, int> counts;
        bool ok = countWords(book.path, counts);

        // Una sola entrada a la seccion critica por libro (no una por palabra)
        if (ok) index.merge(book.id, counts);
        processed++;

        sem_wait(&coutSem);
        if (ok)
            cout << "  [hilo " << threadId << "] " << book.id << ";" << book.name
                 << " (" << counts.size() << " palabras distintas)" << endl;
        else
            cerr << "  [hilo " << threadId << "] Aviso: no se pudo leer '" << book.name << "', se omite" << endl;
        sem_post(&coutSem);
    }

    sem_wait(&coutSem);
    cout << "  [hilo " << threadId << "] cola vacia, termina (" << processed << " libros)" << endl;
    sem_post(&coutSem);
}
