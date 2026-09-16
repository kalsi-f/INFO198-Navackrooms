#ifndef PROFILE_H
#define PROFILE_H
 
#include <string>
#include <vector>
using namespace std;

const int MAX_PROFILE_NAME = 20;   
const int MAX_OPTIONS = 8;   

struct Profile {
    char name[MAX_PROFILE_NAME];          // ej: "ADMIN", "GENERAL"
    int options[MAX_OPTIONS];   // opciones de menu permitidas
    int optionsCount;           // cantidad de opciones de menu permitidas
};
 
// persistencia 
// lee el archivo txt
vector<Profile> loadProfiles();

// agrega una nueva linea al final del USUARIOS.TXT 
void appendProfile(const Profile& p);

// reescribe el archivo completo desde cero con la lista actual
void saveAllProfiles(const vector<Profile>& profiles);
 
// si "loaded" es false, carga desde archivo y lo marca como true.
// si ya es true, devuelve directamente la lista que ya esta en memoria.
// vector<Profile>& listProfiles(vector<Profile>& profiles, bool& loaded, const string& path);
 
// agrega el perfil a la lista en memoria y al archivo
void createProfile(vector<Profile>& profiles, const Profile& p);
 
// elimina el perfil (por nombre) de la lista en memoria y reescribe el archivo
// devuelve true si se encontro y elimino, false si no existia ese nombre.
bool deleteProfile(vector<Profile>& profiles, const string& name);


#endif 

