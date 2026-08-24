#include "User.h"

#include <cstring>  // char
#include <fstream> // archivos
#include <iostream> 
#include <sstream> // stringstream 
using namespace std;
 
vector<User> loadUsers(const string& path, vector<Profile>& profiles) {
    ifstream file(path);
 
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo '" << path << "'" << endl;
        exit(1);
    }
 
    vector<User> users;
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
 
        users.push_back(u);
    }
 
    return users;
}
 
void appendUser(const string& path, const User& u) {
    ofstream file(path, ios::app);
 
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo '" << path << "'" << endl;
        exit(1);
    }
 
    file << u.id << ";"
         << u.name << ";"
         << u.username << ";"
         << u.password << ";"
         << u.profile->name << endl;
}
 
void saveAllUsers(const string& path, const vector<User>& users) {
    ofstream file(path); // sin ios::append =>  reescribe desde cero
 
    for (int i = 0; i < users.size(); i++) {
        file << users[i].id << ";" << users[i].name << ";" << users[i].username << ";"
             << users[i].password << ";" << users[i].profile->name << endl;
    }
}
 
vector<User>& listUsers(vector<User>& users, bool& loaded, const string& path, vector<Profile>& profiles) {
    if (!loaded) {
        users = loadUsers(path, profiles);
        loaded = true;
    }
    return users;
}
 
void createUser(vector<User>& users, bool& loaded, const string& path, vector<Profile>& profiles, const User& u) {
    listUsers(users, loaded, path, profiles); // asegura que este cargado antes de agregar
    users.push_back(u);
    appendUser(path, u);
}
 
bool deleteUser(vector<User>& users, bool& loaded, const string& path, vector<Profile>& profiles, int id) {
    listUsers(users, loaded, path, profiles);
 
    for (int i = 0; i < users.size(); i++) {
        if (users[i].id == id) {
            users.erase(users.begin() + i);
            saveAllUsers(path, users);
            return true;
        }
    }
 
    return false;
}
 

