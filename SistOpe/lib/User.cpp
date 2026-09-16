#include "User.h"
#include "config/EnvConfig.h"

#include <cstring>  // char
#include <fstream> // archivos
#include <iostream> 
#include <sstream> // stringstream 
using namespace std;
 
Users loadUsers(const vector<Profile>& profiles) {
    ifstream file(ENV_CONFIG.USERS_FILE_PATH, ios::binary);
 
    if (!file.is_open()) {
        throw runtime_error("Error: no se pudo abrir el archivo " + ENV_CONFIG.USERS_FILE_PATH);
    }

    Users users;
    users.currentId = 0;

    User u;
    int recordNumber = 0;
 
    while (file.read(reinterpret_cast<char*>(&u), sizeof(User))) {
        recordNumber++;
 
        if (u.profileIndex < 0 || u.profileIndex >= (int)profiles.size()) {
            throw runtime_error("Error: registro " + to_string(recordNumber) + " de " +
                                 ENV_CONFIG.USERS_FILE_PATH + " referencia un perfil inexistente (indice " +
                                 to_string(u.profileIndex) + ").");
        }
        // Asegura que las cadenas terminen en null para evitar desbordamientos
        u.name[sizeof(u.name) - 1] = '\0';
        u.username[sizeof(u.username) - 1] = '\0';
        u.password[sizeof(u.password) - 1] = '\0';
 
        if (u.id > users.currentId) {
            users.currentId = u.id;
        }
 
        users.data.push_back(u);
    }
 
    return users;
}
 
void appendUser(const User& u, const vector<Profile>& profiles) {
    ofstream file(ENV_CONFIG.USERS_FILE_PATH, ios::binary | ios::app);
 
    if (!file.is_open()) {
        throw runtime_error("Error: no se pudo abrir el archivo '" + string(ENV_CONFIG.USERS_FILE_PATH) + "'");
    }
 
    file.write(reinterpret_cast<const char*>(&u), sizeof(User));
}
 
void saveAllUsers(const Users& users, const vector<Profile>& profiles) {
    ofstream file(ENV_CONFIG.USERS_FILE_PATH, ios::binary | ios::trunc);
 
    if (!file.is_open()) {
        throw runtime_error("Error: no se pudo abrir el archivo " + ENV_CONFIG.USERS_FILE_PATH + " para escritura");
    }
 
    for (size_t i = 0; i < users.data.size(); i++) {
        file.write(reinterpret_cast<const char*>(&users.data[i]), sizeof(User));
    }

}
 
/*
vector<User>& listUsers(vector<User>& users, bool& loaded, const string& path, vector<Profile>& profiles) {
    if (!loaded) {
        users = loadUsers(path, profiles);
        loaded = true;
    }
    return users;
}
*/    
 
void createUser(vector<Profile>& profiles, Users& users, User& u) {
    // listUsers(users, loaded, path, profiles); // asegura que este cargado antes de agregar
    //users.data.push_back(u);
    //appendUser(u);

    //logica nueva 
    users.currentId ++; // para no chocar con los ides
    u.id =  users.currentId;
    users.data.push_back(u);
    appendUser(u, profiles);
}
 
bool deleteUser(vector<Profile>& profiles, Users& users, int id) {
 
    for (size_t i = 0; i < users.data.size(); i++) {
        if (users.data[i].id == id) {
            users.data.erase(users.data.begin() + i);
            saveAllUsers(users, profiles);
            return true;
        }
    }
 
    return false;
}
 


User* findUserById(Users& users, int id) {
    for (User& u : users.data) {
        if (u.id == id) {
            return &u;
        }
    }
    return nullptr;
}
