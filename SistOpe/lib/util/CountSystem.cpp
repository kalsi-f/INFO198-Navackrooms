#include "CountSystem.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace std;

namespace {

int getLetterType(const string& text, size_t index, size_t& bytes) {
    unsigned char c = static_cast<unsigned char>(text[index]);
    bytes = 1;

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            return 1; // vocales
        }
        return 2; // consonantes
    }

    // vocales con tilde, ü y ñ
    if (index + 1 < text.size() && c == 0xC3) {
        unsigned char c2 = static_cast<unsigned char>(text[index + 1]);
        bytes = 2;

        if (c2 == 0xA1 || c2 == 0xA9 || c2 == 0xAD ||
            c2 == 0xB3 || c2 == 0xBA || c2 == 0xBC ||
            c2 == 0x81 || c2 == 0x89 || c2 == 0x8D ||
            c2 == 0x93 || c2 == 0x9A || c2 == 0x9C) {
            return 1; 
        }

        if (c2 == 0xB1 || c2 == 0x91) {
            return 2; 
        }
    }

    return 0; // no es una letra
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

size_t utf8CharacterLength(unsigned char c) {
    if ((c & 0x80) == 0) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

void clearScreen() {
    cout << "\033[2J\033[H";
}

void showResult(const CountResult& result) {
    cout << "\n----------- RESUMEN DE CONTEO -----------" << endl;
    cout << "Vocales              : " << result.vowels << endl;
    cout << "Consonantes          : " << result.consonants << endl;
    cout << "Caracteres especiales: " << result.specialCharacters << endl;
    cout << "Palabras             : " << result.words << endl;
    cout << "------------------------------------------" << endl;
}

}

CountResult countText(const string& text) {
    CountResult result = {0, 0, 0, 0};
    bool insideWord = false;

    size_t i = 0;

    while (i < text.size()) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        size_t bytes = 1;
        int letterType = getLetterType(text, i, bytes);

        if (letterType == 1) {
            result.vowels++;
            if (!insideWord) {
                result.words++;
                insideWord = true;
            }
            i += bytes;
            continue;
        }

        if (letterType == 2) {
            result.consonants++;
            if (!insideWord) {
                result.words++;
                insideWord = true;
            }
            i += bytes;
            continue;
        }

        if (isDigit(static_cast<char>(c))) {
            if (!insideWord) {
                result.words++;
                insideWord = true;
            }
            i++;
            continue;
        }
        //exclusion de especial
        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            insideWord = false;
            i++;
            continue;
        }

        // cualquier otro es un caracter especial
        result.specialCharacters++;
        insideWord = false;
        i += utf8CharacterLength(c);
    }

    return result;
}

string readTextFile(const string& path) {
    ifstream file(path);

    if (!file.is_open()) {
        throw runtime_error("no se pudo abrir el archivo '" + path + "'");
    }

    string text;
    string line;

    while (getline(file, line)) {
        text += line;
        text += '\n';
    }

    if (file.bad()) {
        throw runtime_error("ocurrio un error mientras se leia el archivo '" + path + "'");
    }

    return text;
}

void runCountSystem(int mode, const string& defaultFilePath) {
    int option = -1;

    while (option != 0) {
        clearScreen();

        cout << "========================================" << endl;
        if (mode == 6) {
            cout << " CONTEO SOBRE TEXTO" << endl;
        } else {
            cout << " CONTEO SOBRE ARCHIVO" << endl;
        }
        cout << "========================================" << endl;
        cout << "0. Volver" << endl;
        cout << "1. Realizar conteo" << endl;
        cout << endl << "Opcion: ";

        string input;
        if (!getline(cin, input)) {
            return;
        }

        try {
            size_t pos;
            option = stoi(input, &pos);
            if (pos != input.size()) {
                throw invalid_argument("opcion invalida");
            }
        } catch (const exception&) {
            cout << "Error: opcion invalida." << endl;
            cout << "Presione ENTER para continuar...";
            cin.get();
            continue;
        }

        if (option == 0) {
            return;
        }

        if (option != 1) {
            cout << "Error: opcion invalida." << endl;
            cout << "Presione ENTER para continuar...";
            cin.get();
            continue;
        }

        string path;

        if (mode == 6) {
            path = defaultFilePath;

            cout << endl;
            cout << "Archivo indicado por -f: " << path << endl;
        } else if (mode == 7) {
            cout << endl << "Ingrese el path del archivo: ";
            if (!getline(cin, path) || path.empty()) {
                cout << "Error: el path no puede estar vacio." << endl;
                cout << "Presione ENTER para continuar...";
                cin.get();
                continue;
            }
        } 

        try {
            string text = readTextFile(path);
            CountResult result = countText(text);
            showResult(result);
        } catch (const exception& e) {
            cout << endl << "Error de conteo: " << e.what() << endl;
        }

        cout << "Presione ENTER para continuar...";
        cin.get();
    }
}
