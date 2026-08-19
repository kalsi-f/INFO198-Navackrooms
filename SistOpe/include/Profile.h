#ifndef PROFILE_H
#define PROFILE_H
 
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

struct Profile {
    string name;          // ej: "ADMIN", "GENERAL"
    vector<int> options;   // opciones de menu permitidas
};
 
// persistencia 
// lee el archivo txt
vector<Profile> loadProfiles(const string& path);
// agrega una nueva linea al final del USUARIOS.TXT 
void appendProfile(const string& path, const Profile& p);

 
#endif 
