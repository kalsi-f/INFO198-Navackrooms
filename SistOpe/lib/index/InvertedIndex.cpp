#include "index/InvertedIndex.h"

#include <fstream>

using namespace std;

InvertedIndex::InvertedIndex() {
    sem_init(&mutex, 0, 1);
}

InvertedIndex::~InvertedIndex() {
    sem_destroy(&mutex);
}

void InvertedIndex::merge(int bookId, const unordered_map<string, int>& counts) {
    // Si dos hilos insertan a la vez en el map, su arbol interno se puede corromper
    // (o perder palabras): solo un hilo a la vez puede estar aqui adentro.
    sem_wait(&mutex);

    // Cada libro se procesa una sola vez, asi que (palabra, bookId) nunca se repite:
    // basta con asignar, no hace falta sumar.
    for (const auto& [word, times] : counts) {
        index[word][bookId] = times;
    }

    sem_post(&mutex);
}

bool InvertedIndex::writeTo(const string& path) const {
    ofstream file(path);
    if (!file.is_open()) return false;

    for (const auto& [word, books] : index) {
        file << word;
        for (const auto& [bookId, times] : books) {
            file << ";(" << bookId << "," << times << ")";
        }
        file << "\n";
    }
    return true;
}
