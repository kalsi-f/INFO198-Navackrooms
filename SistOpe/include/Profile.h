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

// reescribe el archivo completo desde cero con la lista actual
void saveAllProfiles(const string& path, const vector<Profile>& profiles);
 
// si "loaded" es false, carga desde archivo y lo marca como true.
// si ya es true, devuelve directamente la lista que ya esta en memoria.
vector<Profile>& listProfiles(vector<Profile>& profiles, bool& loaded, const string& path);
 
// agrega el perfil a la lista en memoria y al archivo
void createProfile(vector<Profile>& profiles, bool& loaded, const string& path, const Profile& p);
 
// elimina el perfil (por nombre) de la lista en memoria y reescribe el archivo
void deleteProfile(vector<Profile>& profiles, bool& loaded, const string& path, const string& name);

#endif 

