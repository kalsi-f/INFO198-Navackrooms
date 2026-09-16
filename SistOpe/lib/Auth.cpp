#include "Auth.h"

#include <cstdlib>
#include <iostream>
using namespace std;

namespace {

void printUsageAndExit(const string& programName) {
    cerr << "Uso: " << programName
         << " -u <usuario> -p <password> -f <archivo>" << endl;
    exit(1);
}

}

RunArgs parseArguments(int argc, char* argv[]) {
    RunArgs args;
    bool hasUser = false;
    bool hasPassword = false;
    bool hasFile = false;

    for (int i = 1; i < argc; i++) {
        string flag = argv[i];

        if (flag != "-u" && flag != "-p" && flag != "-f") {
            cerr << "Error: argumento desconocido '" << flag << "'" << endl;
            printUsageAndExit(argv[0]);
        }

        if (i + 1 >= argc) {
            cerr << "Error: falta el valor para " << flag << endl;
            printUsageAndExit(argv[0]);
        }

        string value = argv[++i];

        if (value.empty()) {
            cerr << "Error: el valor de " << flag << " no puede estar vacio." << endl;
            printUsageAndExit(argv[0]);
        }

        if (flag == "-u") {
            if (hasUser) {
                cerr << "Error: argumento -u repetido." << endl;
                printUsageAndExit(argv[0]);
            }
            args.username = value;
            hasUser = true;
        } else if (flag == "-p") {
            if (hasPassword) {
                cerr << "Error: argumento -p repetido." << endl;
                printUsageAndExit(argv[0]);
            }
            args.password = value;
            hasPassword = true;
        } else {
            if (hasFile) {
                cerr << "Error: argumento -f repetido." << endl;
                printUsageAndExit(argv[0]);
            }
            args.filePath = value;
            hasFile = true;
        }
    }

    if (!hasUser || !hasPassword || !hasFile) {
        cerr << "Error: faltan argumentos obligatorios (-u, -p y -f son requeridos)." << endl;
        printUsageAndExit(argv[0]);
    }

    return args;
}

int authenticateUser(const Users& users, const string& username, const string& password) {
    for (const User& u : users.data) {
        if (username == u.username && password == u.password) {
            return u.id;
        }
    }

    return -1;
}
