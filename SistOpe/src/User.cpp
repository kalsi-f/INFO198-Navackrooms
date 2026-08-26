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
        cerr << "Error: no se pudo abrir el archivo '" << ENV_CONFIG.USERS_FILE_PATH << "'" << endl;
        exit(1);
    }
 
    Users users;

    users.currentId = -1;
    string line;
 
    while (getline(file, line)) {
        stringstream ss(line);
        string idText, name, username, password, profileText;
 
        getline(ss, idText, ';');
        getline(ss, name, ';');
        getline(ss, username, ';');
        getline(ss, password, ';');
        getline(ss, profileText, ';');
 
        User u;
        u.id = stoi(idText);
        
        if (u.id > users.currentId) users.currentId = u.id;

        strncpy(u.name, name.c_str(), sizeof(u.name) - 1);
        u.name[sizeof(u.name) - 1] = '\0';
 
        strncpy(u.username, username.c_str(), sizeof(u.username) - 1);
        u.username[sizeof(u.username) - 1] = '\0';
 
        strncpy(u.password, password.c_str(), sizeof(u.password) - 1);
        u.password[sizeof(u.password) - 1] = '\0';
 
        for (int i = 0; i < profiles.size(); i++) {
            if (profiles[i].name == profileText) {
                u.profile = &profiles[i];
                break;
            }
        }
 
        users.data.push_back(u);
    }
 
    return users;
}
 
void appendUser(const User& u) {
    ofstream file(ENV_CONFIG.USERS_FILE_PATH, ios::app);
 
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo '" << ENV_CONFIG.USERS_FILE_PATH << "'" << endl;
        exit(1);
    }
 
    file << u.id << ";"
         << u.name << ";"
         << u.username << ";"
         << u.password << ";"
         << u.profile->name << endl;
}
 
void saveAllUsers(const Users& users) {
    ofstream file(ENV_CONFIG.USERS_FILE_PATH); // sin ios::append => reescribe desde cero
 
    for (int i = 0; i < users.data.size(); i++) {
        file << users.data[i].id << ";" << users.data[i].name << ";" << users.data[i].username << ";"
             << users.data[i].password << ";" << users.data[i].profile->name << endl;
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
 
void createUser(vector<Profile>& profiles, Users& users, const User& u) {
    // listUsers(users, loaded, path, profiles); // asegura que este cargado antes de agregar
    users.data.push_back(u);
    appendUser(u);
}
 
bool deleteUser(vector<Profile>& profiles, Users& users, int id) {
 
    for (int i = 0; i < users.data.size(); i++) {
        if (users.data[i].id == id) {
            users.data.erase(users.data.begin() + i);
            saveAllUsers(users);
            return true;
        }
    }
 
    return false;
}
 

