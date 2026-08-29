#include "User.h"
#include "config/EnvConfig.h"

#include <cstring>  // char
#include <fstream> // archivos
#include <iostream> 
#include <sstream> // stringstream 
using namespace std;
 
Users loadUsers(vector<Profile> &profiles) {
    ifstream file(ENV_CONFIG.USERS_FILE_PATH);
 
    if (!file.is_open()) {
        throw runtime_error("Error: no se pudo abrir el archivo '" + string(ENV_CONFIG.USERS_FILE_PATH) + "'");
    }
 
    Users users;

    users.currentId = -1;
    string line;
    int lineNumber = 0;
 
    while (getline(file, line)) {
        lineNumber++;

        if (line.empty()) {
            throw runtime_error("Error: linea " + to_string(lineNumber) + " de '" + 
                                string(ENV_CONFIG.USERS_FILE_PATH) + 
                                "' esta vacia. No se permiten lineas en blanco.");
        }

        //if (line.empty()) {
        //    continue;
        //}
 
        stringstream ss(line);
        string idText, name, username, password, profileText;
 
        bool ok = (bool)getline(ss, idText, ';')
                  && (bool)getline(ss, name, ';')
                  && (bool)getline(ss, username, ';')
                  && (bool)getline(ss, password, ';')
                  && (bool)getline(ss, profileText, ';');
 
        if (!ok) {
            throw runtime_error("Error: linea " + to_string(lineNumber) + " de '" + 
                                string(ENV_CONFIG.USERS_FILE_PATH) + 
                                "' no tiene los 5 campos esperados: " + line);
        }
 
        User u;
 
        try {
            size_t pos;
            u.id = stoi(idText, &pos);
            if (pos != idText.size()) {
                throw invalid_argument("sobra texto no numerico");
            }
        } catch (const invalid_argument&) {
            throw runtime_error("Error: linea " + to_string(lineNumber) + " de '" + 
                                string(ENV_CONFIG.USERS_FILE_PATH) + 
                                "' tiene un id invalido ('" + idText + "'): " + line);
        }
 
        if (u.id > users.currentId) {
            users.currentId = u.id;
        }
 
        strncpy(u.name, name.c_str(), sizeof(u.name) - 1);
        u.name[sizeof(u.name) - 1] = '\0';
 
        strncpy(u.username, username.c_str(), sizeof(u.username) - 1);
        u.username[sizeof(u.username) - 1] = '\0';
 
        strncpy(u.password, password.c_str(), sizeof(u.password) - 1);
        u.password[sizeof(u.password) - 1] = '\0';

        u.profile_index = -1;
 
        for (size_t i = 0; i < profiles.size(); i++) {
            if (profiles[i].name == profileText) {
                u.profile_index = i; // guardando indice
                break;
            }
        }
 
        if (u.profile_index == -1) {
            throw runtime_error("Error: linea " + to_string(lineNumber) + " de '" + 
                                string(ENV_CONFIG.USERS_FILE_PATH) + 
                                "' referencia un perfil inexistente ('" + profileText + "'): " + line);
        }
 
        users.data.push_back(u);
    }
 
    return users;
}
 
void appendUser(const User& u, const vector<Profile>& profiles) {
    ofstream file(ENV_CONFIG.USERS_FILE_PATH, ios::app);
 
    if (!file.is_open()) {
        throw runtime_error("Error: no se pudo abrir el archivo '" + string(ENV_CONFIG.USERS_FILE_PATH) + "'");
    }

    file << u.id << ";"
         << u.name << ";"
         << u.username << ";"
         << u.password << ";"
         << profiles[u.profile_index].name << endl; //accede al nombre mediante el index
}
 
void saveAllUsers(const Users& users, const vector<Profile>& profiles) {
    ofstream file(ENV_CONFIG.USERS_FILE_PATH); // sin ios::append => reescribe desde cero
 
    for (size_t i = 0; i < users.data.size(); i++) {
        file << users.data[i].id << ";" << users.data[i].name << ";" << users.data[i].username << ";"
             << users.data[i].password << ";" << profiles[users.data[i].profile_index].name << endl;
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
 

