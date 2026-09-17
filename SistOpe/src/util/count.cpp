#include "CountSystem.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {

    string filePath;

    if (argc == 2) {
        filePath = argv[1];
    }


    runCountSystem(filePath);

    return 0;
}