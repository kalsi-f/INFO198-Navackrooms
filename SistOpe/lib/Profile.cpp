#include "Profile.h"
#include "config/EnvConfig.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

vector<Profile> loadProfiles() {
    ifstream file(ENV_CONFIG.PROFILES_FILE_PATH, ios::binary);
 
    if (!file.is_open()) {
        throw runtime_error("Error: no se pudo abrir el archivo " + ENV_CONFIG.PROFILES_FILE_PATH);
    }

    vector<Profile> profiles;
    Profile p;
    int recordNumber = 0;
 
    while (file.read(reinterpret_cast<char*>(&p), sizeof(Profile))) {
        recordNumber++;
 
        if (p.optionsCount < 0 || p.optionsCount > MAX_OPTIONS) {
            throw runtime_error("Error: registro " + to_string(recordNumber) + " de " +
                                ENV_CONFIG.PROFILES_FILE_PATH + " esta corrupto (optionsCount fuera de rango).");
        }
 
        p.name[MAX_PROFILE_NAME - 1] = '\0'; // para no terminr en null
        profiles.push_back(p);
    }
 
    return profiles;

}


void appendProfile(const Profile& p) {
    ofstream file(ENV_CONFIG.PROFILES_FILE_PATH, ios::binary | ios::app);
 
    if (!file.is_open()) {
        throw runtime_error("Error: no se pudo abrir el archivo " + ENV_CONFIG.PROFILES_FILE_PATH + "' para escritura");
    }
 
    file.write(reinterpret_cast<const char*>(&p), sizeof(Profile));
}

void saveAllProfiles(const vector<Profile>& profiles) {
    // ios::trunc borra el contenido previo, para reescribir desde cero
    ofstream file(ENV_CONFIG.PROFILES_FILE_PATH, ios::binary | ios::trunc);
 
    if (!file.is_open()) {
        throw runtime_error("Error: no se pudo abrir el archivo " + ENV_CONFIG.PROFILES_FILE_PATH+ " para reescritura");
    }
 
    for (size_t i = 0; i < profiles.size(); i++) {
        file.write(reinterpret_cast<const char*>(&profiles[i]), sizeof(Profile));
    }
}


/*
vector<Profile>& listProfiles(vector<Profile>& profiles, bool& loaded, const string& path) {
    if (!loaded) {
        profiles = loadProfiles(path);
        loaded = true;
    }
    return profiles;
}
*/

void createProfile(vector<Profile>& profiles, const Profile& p) {
    profiles.push_back(p);
    appendProfile(p);
}

bool deleteProfile(vector<Profile>& profiles, const string& name) {
    for (size_t i = 0; i < profiles.size(); ++i) {
        if (name == profiles[i].name) {
            profiles.erase(profiles.begin() + static_cast<long>(i));
            saveAllProfiles(profiles);
            return true;
        }
    }
    return false;
}
