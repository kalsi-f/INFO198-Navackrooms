#ifndef WORK_QUEUE_H
#define WORK_QUEUE_H

#include "index/Book.h"

#include <queue>
#include <semaphore.h>

using namespace std;

// Cola de trabajo compartida entre los hilos.
// SECCION CRITICA #1: la protege un semaforo binario (inicializado en 1) que vive aqui adentro,
// asi nadie puede usar la cola sin pasar por el.
class WorkQueue {
public:
    WorkQueue();
    ~WorkQueue();

    // Un sem_t no se puede copiar: prohibimos copiar la cola para no duplicar el semaforo por error
    WorkQueue(const WorkQueue&) = delete;
    WorkQueue& operator=(const WorkQueue&) = delete;

    void push(const Book& book);

    // Saca el siguiente libro en una sola operacion atomica (mirar + leer + sacar).
    // Devuelve false si la cola esta vacia: es la señal para que el hilo termine.
    bool tryPop(Book& book);

private:
    queue<Book> books;
    sem_t mutex;
};

#endif
