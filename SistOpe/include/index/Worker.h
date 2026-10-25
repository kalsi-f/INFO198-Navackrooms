#ifndef WORKER_H
#define WORKER_H

#include "index/InvertedIndex.h"
#include "index/WorkQueue.h"

#include <semaphore.h>

// La funcion que corre cada hilo trabajador.
// Repite: sacar libro de la cola -> contar palabras (local) -> merge al indice global,
// hasta que la cola quede vacia. Ahi el hilo termina.
// coutSem protege la consola (SECCION CRITICA #3) para que los mensajes no se mezclen.
void worker(int threadId, WorkQueue& queue, InvertedIndex& index, sem_t& coutSem);

#endif
