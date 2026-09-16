#ifndef AUTH_H
#define AUTH_H
 
#include <string>
#include "User.h"
using namespace std;

struct RunArgs {
    string username;
    string password;
    string filePath;
};
 
// recorre argv buscando -u, -p y -f. Si falta alguno
// muestra el uso esperado por stderr y termina el programa (exit(1))
RunArgs parseArguments(int argc, char* argv[]);
 
// devuelve el ID del usuario autenticado o -1 si las credenciales no coinciden
int authenticateUser(const Users& users, const string& username, const string& password);
 
#endif
