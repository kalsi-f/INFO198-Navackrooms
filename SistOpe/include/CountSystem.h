#ifndef COUNT_SYSTEM_H
#define COUNT_SYSTEM_H

#include <string>
#include <unordered_map>

using namespace std;

struct CountResult {
    int vowels;
    int consonants;
    int specialCharacters;
    int words;
};

CountResult countText(const string& text);

// cuenta cuantas veces aparece cada palabra (palabra -> cantidad), usa las
// mismas reglas de countText y deja las palabras en minusculas
unordered_map<string, int> countWordFrequency(const string& text);


string readTextFile(const string& path);

// para diferenciar entre 6 y 7
void runCountSystem(const string& FilePath);

#endif