#ifndef BOOK_H
#define BOOK_H

#include <cstdint>
#include <string>

using namespace std;

// Un libro a procesar. Es lo que viaja por la cola de trabajo.
struct Book {
    int id;          // identificador numerico unico (1..N), es el que va en el indice
    string name;     // nombre del archivo, ej: "Dracula.txt"
    string path;     // ruta completa para abrirlo
    uintmax_t size;  // tamano en bytes (sirve para ordenar la cola despues)
};

#endif
