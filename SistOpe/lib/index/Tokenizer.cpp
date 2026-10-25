#include "index/Tokenizer.h"

#include <cctype>
#include <fstream>
#include <sstream>

using namespace std;

// En UTF-8 las letras con tilde y la ñ ocupan 2 bytes y el primero siempre es 0xC3:
//   á = C3 A1   é = C3 A9   ñ = C3 B1   Ñ = C3 91   ü = C3 BC
// Las mayusculas (C3 80..C3 9E) se pasan a minuscula sumando 0x20 al segundo byte.
// Los demas caracteres no ASCII (“ ” ’ — …) son puntuacion de 3 bytes y separan palabras.
static const unsigned char UTF8_LATIN = 0xC3;
static const unsigned char UTF8_TIMES = 0x97;   // C3 97 = '×', no es letra
static const unsigned char UTF8_DIVIDE = 0xB7;  // C3 B7 = '÷', no es letra

static void addWord(string& word, unordered_map<string, int>& counts) {
    if (!word.empty()) {
        counts[word]++;
        word.clear();
    }
}

bool countWords(const string& path, unordered_map<string, int>& counts) {
    ifstream file(path, ios::binary);
    if (!file.is_open()) return false;

    // Leemos el libro entero de una vez: es mucho mas rapido que caracter a caracter
    stringstream buffer;
    buffer << file.rdbuf();
    const string text = buffer.str();

    string word;
    for (size_t i = 0; i < text.size(); i++) {
        unsigned char c = text[i];

        if (isalnum(c) && c < 128) {
            word += tolower(c);
        }
        else if (c == UTF8_LATIN && i + 1 < text.size()) {
            unsigned char next = text[i + 1];

            if (next == UTF8_TIMES || next == UTF8_DIVIDE) {
                addWord(word, counts);
            }
            else {
                if (next >= 0x80 && next <= 0x9E) next += 0x20;  // Ñ -> ñ, Á -> á
                word += (char)c;
                word += (char)next;
            }
            i++;  // ya consumimos el segundo byte
        }
        else {
            // espacio, salto de linea, puntuacion o cualquier otro simbolo: termina la palabra
            addWord(word, counts);
        }
    }
    addWord(word, counts);  // la ultima palabra del archivo

    return true;
}
