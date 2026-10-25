#include "index/IndexConfig.h"
#include "config/EnvConfig.h"

#include <cstdlib>
#include <iostream>

using namespace std;

// Convierte N_THREADS a entero y verifica que sea > 0.
// Rechaza cosas como "", "abc", "4x", "0" o "-2".
static int parseThreads(const string& value) {
    try {
        size_t pos;
        int n = stoi(value, &pos);
        if (pos == value.size() && n > 0) return n;
    }
    catch (...) {}

    cerr << "Error: N_THREADS debe ser un entero mayor a 0 (valor actual: '" << value << "')" << endl;
    exit(1);
}

IndexConfig loadIndexConfig() {
    // getEnvVar ya termina el programa con un mensaje si la variable no existe
    IndexConfig config;
    config.booksDir    = getEnvVar("BOOKS_DIR");
    config.indexFile   = getEnvVar("INDEX_FILE");
    config.bookMapFile = getEnvVar("BOOK_MAP_FILE");
    config.nThreads    = parseThreads(getEnvVar("N_THREADS"));
    return config;
}
