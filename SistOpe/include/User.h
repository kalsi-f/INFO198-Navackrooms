#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Profile.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

struct User {
    int id;
    char name[50];
    char username[20];
    char password[20];
    Profile* profile;   // puntero al Perfil
};

// persistencia 

// Lee USUARIOS.TXT completo y devuelve la lista de usuarios cargados.
// Cada usuario queda con su puntero "profile" apuntando al Perfil
// correspondiente dentro del vector "profiles" 
vector<User> loadUsers(const string& path, vector<Profile>& profiles);

// Agrega una nueva linea al final de USUARIOS.TXT con el usuario dado.
void appendUser(const string& path, const User& u);

#endif 