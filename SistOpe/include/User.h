#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Profile.h"
using namespace std;

const int MAX_NAME = 50;
const int MAX_USERNAME = 20;
const int MAX_PASSWORD = 20;
 
struct User {
    int id;
    char name[MAX_NAME];
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int profileIndex;   
};
 
struct Users {
    vector<User> data;
    int currentId;
};

// persistencia
// lee USUARIOS.TXT y devuelve la lista de usuarios cargados,
// con el puntero "profile" de cada uno por nombre
Users loadUsers(const vector<Profile> &profiles);
 
// agrega una nueva linea al final de USUARIOS.TXT
void appendUser(const User& u, const vector<Profile>& profiles);
 
// reescribe el archivo completo desde cero con la lista actual
void saveAllUsers(const Users& users, const vector<Profile>& profiles);
 
// si "loaded" es false, carga desde archivo y lo marca como true.
// si ya es true, devuelve directamente la lista que ya esta en memoria.
// vector<User>& listUsers(vector<User>& users, bool& loaded, const string& path, vector<Profile>& profiles);
 
// agrega el usuario a la lista en memoria y al archivo
void createUser(vector<Profile>& profiles, Users& users, User& u);
 
// elimina el usuario por id. si su perfil es "ADMIN", imprime una alerta
// pero igual lo elimina (solo advierte, no bloquea)
bool deleteUser(vector<Profile>& profiles, Users& users, int id);
 
// busca nuevamente al usuario por ID. puntero puede quedar invalido
// si cambia el vector de los usuarios
User* findUserById(Users& users, int id);

#endif
 
