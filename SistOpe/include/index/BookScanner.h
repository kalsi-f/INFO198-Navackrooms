#ifndef BOOK_SCANNER_H
#define BOOK_SCANNER_H

#include "index/Book.h"

#include <string>
#include <vector>

using namespace std;

// Verifica que la carpeta exista y se pueda leer, y lista los .txt que contiene.
// Asigna ids 1..N en orden alfabetico (asi el mismo libro siempre tiene el mismo id).
// Si hay un problema que impide comenzar, muestra el error y termina.
vector<Book> scanBooks(const string& dir);

// Escribe la asociacion "id;nombre" (una por linea) en el archivo indicado.
// Devuelve false si no se pudo escribir.
bool writeBookMap(const vector<Book>& books, const string& path);

#endif
