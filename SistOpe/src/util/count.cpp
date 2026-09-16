#include "CountSystem.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int main() {
    const char* modeText = getenv("COUNT_MODE");
    const char* fileText = getenv("COUNT_FILE");

    int mode = stoi(modeText);


    string filePath;
    if (fileText != nullptr) {
        filePath = fileText;
    }


    runCountSystem(mode, filePath);
    return 0;
}