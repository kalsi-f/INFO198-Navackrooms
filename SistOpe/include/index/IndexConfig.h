#ifndef INDEX_CONFIG_H
#define INDEX_CONFIG_H

#include <string>

using namespace std;

// Configuracion del indexador, toda sacada de variables de entorno.
struct IndexConfig {
    string booksDir;     // BOOKS_DIR: carpeta con los libros
    string indexFile;    // INDEX_FILE: donde se escribe el indice invertido
    string bookMapFile;  // BOOK_MAP_FILE: donde se escribe id;nombre
    int nThreads;        // N_THREADS: cantidad de hilos trabajadores
};

// Lee y valida las variables. Si alguna falta o es invalida, muestra el error y termina.
IndexConfig loadIndexConfig();

#endif
