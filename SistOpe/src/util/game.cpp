#include "config/EnvConfig.h"
#include "Menu.h"
#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
    loadEnv();
    string option;
    clearScreen();
    while (true) {
        cout << "========================================" << endl
             << "          JUEGO MULTIPLAYER" << endl
             << "========================================" << endl
             << "0. Volver" << endl
             << "1. Iniciar servidor" << endl
             << "2. Conectarse como cliente" << endl
             << "Opcion: " << flush;
        if (!getline(cin, option) || option == "0") return 0;
        if (option != "1" && option != "2") {
            cerr << "Opcion invalida." << endl;
            continue;
        }
        string path = getEnvVar(option == "1" ? "GAME_SERVER_PROGRAM" : "GAME_CLIENT_PROGRAM");
        if (path.empty()) { cerr << "Ruta del programa vacia." << endl; continue; }
        if (system(path.c_str()) != 0) {
            cerr << "El programa del juego termino con un error." << endl;
        }
        cout << endl << "Presiona ENTER para volver al menu." << flush;
        if (!getline(cin, option)) return 0;
        clearScreen();
    }
}
