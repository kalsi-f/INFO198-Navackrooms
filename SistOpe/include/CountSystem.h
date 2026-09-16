#ifndef COUNT_SYSTEM_H
#define COUNT_SYSTEM_H

#include <string>

using namespace std;

struct CountResult {
    int vowels;
    int consonants;
    int specialCharacters;
    int words;
};

CountResult countText(const string& text);


string readTextFile(const string& path);

// para diferenciar entre 6 y 7
void runCountSystem(int mode, const string& defaultFilePath);

#endif