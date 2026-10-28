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

    // letras latinas en UTF-8 (C3 80..C3 BF): tildes, dieresis, ñ, ç, æ, etc.
    if (index + 1 < text.size() && c == 0xC3) {
        unsigned char c2 = static_cast<unsigned char>(text[index + 1]);
        bytes = 2;

        // las mayusculas (80..9E) se pasan a minuscula para revisar una sola vez
        if (c2 >= 0x80 && c2 <= 0x9E) c2 += 0x20;

        // à á â ã ä å æ, è é ê ë, ì í î ï, ò ó ô õ ö, ø ù ú û ü
        if ((c2 >= 0xA0 && c2 <= 0xA6) || (c2 >= 0xA8 && c2 <= 0xAF) ||
            (c2 >= 0xB2 && c2 <= 0xB6) || (c2 >= 0xB8 && c2 <= 0xBC)) {
            return 1;
        }

        // ç ð ñ ý þ ÿ y ß (0x9F, no tiene mayuscula)
        if (c2 == 0xA7 || c2 == 0xB0 || c2 == 0xB1 || c2 == 0xBD ||
            c2 == 0xBE || c2 == 0xBF || c2 == 0x9F) {
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

// agrega la letra en minuscula a la palabra
// en UTF-8 las mayusculas latinas (C3 80..9E) estan 0x20 antes que su minuscula
void appendLowercase(string& word, const string& text, size_t index, size_t bytes) {
    unsigned char c = static_cast<unsigned char>(text[index]);

    if (bytes == 1) {
        if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        word += static_cast<char>(c);
        return;
    }

    unsigned char c2 = static_cast<unsigned char>(text[index + 1]);
    if (c2 >= 0x80 && c2 <= 0x9E) c2 += 0x20;
    word += static_cast<char>(c);
    word += static_cast<char>(c2);
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

// unordered_map porque no necesita orden y es mas rapido que map para contar
unordered_map<string, int> countWordFrequency(const string& text) {
    unordered_map<string, int> frequency;
    string word;

    size_t i = 0;

    while (i < text.size()) {
        unsigned char c = static_cast<unsigned char>(text[i]);
        size_t bytes = 1;
        int letterType = getLetterType(text, i, bytes);

        // letras y digitos forman parte de la palabra, igual que en countText
        if (letterType == 1 || letterType == 2) {
            appendLowercase(word, text, i, bytes);
            i += bytes;
            continue;
        }

        if (isDigit(static_cast<char>(c))) {
            word += static_cast<char>(c);
            i++;
            continue;
        }

        // espacios y caracteres especiales cierran la palabra
        if (!word.empty()) {
            frequency[word]++;
            word.clear();
        }

        if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
            i++;
        } else {
            i += utf8CharacterLength(c);
        }
    }

    // ultima palabra si el texto no termina en separador
    if (!word.empty()) {
        frequency[word]++;
    }

    return frequency;
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

void runCountSystem(const string& FilePath) {

    int option = -1;
    while (option != 0) {

        clearScreen();
        cout << "========================================" << endl;

        if (!FilePath.empty()) {
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

        if (!FilePath.empty()) {
            path = FilePath;
            cout << endl;
            cout << "Archivo indicado por -f: " << path << endl;
        } else {
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
