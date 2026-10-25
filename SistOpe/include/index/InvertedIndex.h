#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <map>
#include <semaphore.h>
#include <string>
#include <unordered_map>

using namespace std;

// Indice invertido global: palabra -> (id libro -> veces que aparece).
// Se usa map (ordenado) para que el archivo salga en orden alfabetico sin trabajo extra.
// SECCION CRITICA #2: merge() modifica el map compartido, lo protege un semaforo binario.
class InvertedIndex {
public:
    InvertedIndex();
    ~InvertedIndex();

    InvertedIndex(const InvertedIndex&) = delete;
    InvertedIndex& operator=(const InvertedIndex&) = delete;

    // Incorpora el conteo de UN libro al indice global. Seguro de llamar desde varios hilos.
    void merge(int bookId, const unordered_map<string, int>& counts);

    // Escribe el indice con formato: palabra;(id,veces);(id,veces)...
    // Se llama una sola vez al final, cuando ya no hay nadie modificando el indice.
    bool writeTo(const string& path) const;

    size_t wordCount() const { return index.size(); }

private:
    map<string, map<int, int>> index;
    sem_t mutex;
};

#endif
