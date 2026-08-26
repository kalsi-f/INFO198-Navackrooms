#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Profile.h"
using namespace std;
 
struct User {
    int id;
    char name[50];
    char username[20];
    char password[20];
    Profile* profile;   // puntero al Profile correspondiente
};
 
// persistencia
// lee USUARIOS.TXT y devuelve la lista de usuarios cargados,
// con el puntero "profile" de cada uno por nombre
vector<User> loadUsers(vector<Profile> &profiles);
 
// agrega una nueva linea al final de USUARIOS.TXT
void appendUser(const string& path, const User& u);
 
// reescribe el archivo completo desde cero con la lista actual
void saveAllUsers(const vector<User>& users);
 
// si "loaded" es false, carga desde archivo y lo marca como true.
// si ya es true, devuelve directamente la lista que ya esta en memoria.
// vector<User>& listUsers(vector<User>& users, bool& loaded, const string& path, vector<Profile>& profiles);
 
// agrega el usuario a la lista en memoria y al archivo
void createUser(vector<Profile>& profiles, vector<User>& users, const User& u);
 
// elimina el usuario por id. si su perfil es "ADMIN", imprime una alerta
// pero igual lo elimina (solo advierte, no bloquea)
bool deleteUser(vector<Profile>& profiles, vector<User>& users, int id);
 
#endif
 
