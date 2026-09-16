#include "CountSystem.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    int mode;

    string filePath;

    switch (argc) {
        case 1:
            mode = 7;
            break;
        case 2:
            mode = 6;
            filePath = argv[1];
            break;
        default:
            cerr << "Error: Número de argumentos invalido" << endl;
            cout << "Uso: " << endl; 
            cout << "   " << argv[0] << endl;
            cout << "   " <<  argv[0] << " <RUTA_ARCHIVO_TEXTO>" << endl;
            return 1;
    }

    runCountSystem(mode, filePath);
    return 0;
}