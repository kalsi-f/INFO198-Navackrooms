#include "index/BookScanner.h"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

using namespace std;
namespace fs = std::filesystem;

// true si el nombre termina en ".txt", ignorando espacios al final
// (en db/LIBROS hay un archivo llamado "LosmiserablesTOMO2.txt " con un espacio).
static bool isTxt(string name) {
    while (!name.empty() && name.back() == ' ') name.pop_back();
    return name.size() > 4 && name.compare(name.size() - 4, 4, ".txt") == 0;
}

vector<Book> scanBooks(const string& dir) {
    error_code ec;

    if (!fs::exists(dir, ec)) {
        cerr << "Error: la carpeta de libros '" << dir << "' no existe" << endl;
        exit(1);
    }
    if (!fs::is_directory(dir, ec)) {
        cerr << "Error: '" << dir << "' no es una carpeta" << endl;
        exit(1);
    }

    // Si no tenemos permiso de lectura, el iterador falla y deja el error en ec
    fs::directory_iterator it(dir, ec);
    if (ec) {
        cerr << "Error: no se puede leer la carpeta '" << dir << "': " << ec.message() << endl;
        exit(1);
    }

    vector<Book> books;
    for (const fs::directory_entry& entry : it) {
        if (!entry.is_regular_file(ec)) continue;

        string name = entry.path().filename().string();
        if (!isTxt(name)) continue;

        // Que exista no significa que se pueda abrir: lo probamos ahora y no a mitad del proceso
        if (!ifstream(entry.path()).is_open()) {
            cerr << "Aviso: no se puede leer '" << name << "', se omite" << endl;
            continue;
        }

        books.push_back({0, name, entry.path().string(), entry.file_size(ec)});
    }

    if (books.empty()) {
        cerr << "Error: la carpeta '" << dir << "' no contiene libros .txt" << endl;
        exit(1);
    }

    // directory_iterator no garantiza orden: ordenamos para que los ids sean estables
    sort(books.begin(), books.end(), [](const Book& a, const Book& b) { return a.name < b.name; });
    for (size_t i = 0; i < books.size(); i++) books[i].id = i + 1;

    return books;
}

bool writeBookMap(const vector<Book>& books, const string& path) {
    ofstream file(path);
    if (!file.is_open()) return false;

    for (const Book& book : books) {
        file << book.id << ";" << book.name << "\n";
    }
    return true;
}
