#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <map>
#include <queue>
#include <semaphore.h>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// libro pendiente de procesar, cada elemento de la cola de trabajo
struct Book {
    int id; // identificador unico que se usa en el indice
    string name; // ej: "DonQuijote.txt"
    string path; // ruta completa para abrirlo
};

// palabra -> lista de (id del libro, cantidad de apariciones)
typedef map<string, vector<pair<int, int>>> InvertedIndex;

// estado compartido entre el thread principal y los trabajadores
// todo lo que esta aqui se protege con los semaforos de abajo
struct IndexState {
    queue<Book> workQueue; // cola de trabajo compartida
    InvertedIndex index; // indice global con los resultados de todos los libros
    int processedBooks = 0; // libros integrados al indice
    int failedBooks = 0; // libros que no se pudieron leer

    sem_t availableBooks; // contador, cuantos elementos hay en la cola
    sem_t queueMutex; // binario, acceso a workQueue
    sem_t indexMutex; // binario, acceso a index y a los contadores
    sem_t outputMutex; // binario, para que los mensajes no se mezclen
};

// valida N_THREADS, solo digitos y entre 1 y 64
int parseThreadCount(const string& value);

// revisa que la carpeta exista y se pueda leer, y devuelve sus libros
// ordenados por nombre con id 1..N
vector<Book> findBooks(const string& booksDir);

// guarda la asociacion id;nombre de cada libro
void saveBookMap(const vector<Book>& books, const string& path);

// crea los threads, les reparte los libros con la cola y espera a que terminen
void buildInvertedIndex(IndexState& state, const vector<Book>& books, int nThreads);

// escribe el indice con el formato palabra;(id,cantidad);(id,cantidad)
void saveInvertedIndex(const InvertedIndex& index, const string& path);

#endif
