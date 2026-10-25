#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <unordered_map>

using namespace std;

// Lee un libro completo y cuenta cuantas veces aparece cada palabra.
// Las palabras se guardan en minuscula; la puntuacion separa palabras.
// Soporta letras con tilde y la ñ (UTF-8). No usa nada compartido: es seguro llamarla desde varios hilos.
// Devuelve false si no se pudo abrir el archivo.
bool countWords(const string& path, unordered_map<string, int>& counts);

#endif
