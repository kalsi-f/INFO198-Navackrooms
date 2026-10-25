#include "index/WorkQueue.h"

using namespace std;

WorkQueue::WorkQueue() {
    // 0 = compartido entre hilos del mismo proceso, 1 = valor inicial (libre)
    sem_init(&mutex, 0, 1);
}

WorkQueue::~WorkQueue() {
    sem_destroy(&mutex);
}

void WorkQueue::push(const Book& book) {
    sem_wait(&mutex);
    books.push(book);
    sem_post(&mutex);
}

bool WorkQueue::tryPop(Book& book) {
    sem_wait(&mutex);

    // empty + front + pop tienen que ocurrir juntos: si otro hilo se mete entre medio,
    // dos hilos podrian tomar el mismo libro y otro libro quedaria sin procesar
    bool hasBook = !books.empty();
    if (hasBook) {
        book = books.front();
        books.pop();
    }

    sem_post(&mutex);
    return hasBook;
}
