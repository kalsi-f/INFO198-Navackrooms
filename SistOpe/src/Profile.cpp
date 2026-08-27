#include "Profile.h"
#include "config/EnvConfig.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

vector<Profile> loadProfiles() {
    ifstream file(ENV_CONFIG.PROFILES_FILE_PATH);

    if (!file.is_open()) {
        throw runtime_error("Error: no se pudo abrir el archivo '" + string(ENV_CONFIG.PROFILES_FILE_PATH) + "'");
    }

    vector<Profile> profiles; // vector de structs Profile
    string line;
    int lineNumber = 0;
 
    while (getline(file, line)) {
        lineNumber++;

        if (line.empty()) {
            throw runtime_error("Error: linea " + to_string(lineNumber) + " de '" + 
                                string(ENV_CONFIG.PROFILES_FILE_PATH) + 
                                "' esta vacia. No se permiten lineas en blanco.");
        }

        // if (line.empty()) {
        //    continue;
        // }
 
        int cutPos = line.find(';');

        
        if (cutPos == (int)string::npos || cutPos == 0) {
            throw runtime_error("Error: linea " + to_string(lineNumber) + " de '" + 
                                string(ENV_CONFIG.PROFILES_FILE_PATH) + 
                                "' tiene formato invalido (falta ';' o falta el nombre): " + line);
        }
 
        string name = line.substr(0, cutPos);
        string optionsText = line.substr(cutPos + 1);
 
        Profile p;
        p.name = name;
 
        stringstream ss(optionsText);
        string option;
        while (getline(ss, option, ',')) {
            try {
                size_t pos;
                int value = stoi(option, &pos);
                if (pos != option.size()) {
                    throw invalid_argument("sobra texto no numerico");
                }
                p.options.push_back(value);
            } catch (const invalid_argument&) {
                throw runtime_error("Error: linea " + to_string(lineNumber) + " de '" + 
                                    string(ENV_CONFIG.PROFILES_FILE_PATH) + 
                                    "' tiene una opcion invalida ('" + option + "'): " + line);
            } catch (const out_of_range&) {
                throw runtime_error("Error: linea " + to_string(lineNumber) + " de '" + 
                                    string(ENV_CONFIG.PROFILES_FILE_PATH) + 
                                    "' tiene un numero demasiado grande ('" + option + "'): " + line);
            }
        }
 
        profiles.push_back(p); // agrega al vector
    }
 
    return profiles;
}


void appendProfile(const Profile& p) {
    ofstream file(ENV_CONFIG.PROFILES_FILE_PATH, ios::app); // ios app para agregar al final sin borrar lo que existe (tipos de ios::)

    if (!file.is_open()) {
        throw runtime_error("Error: no se pudo abrir el archivo '" + string(ENV_CONFIG.PROFILES_FILE_PATH) + "' para escritura");
    }

    file << p.name << ";";

    for (size_t i = 0; i < p.options.size(); i++) {
        file << p.options[i];
        if (i < p.options.size() - 1) {
            file << ",";
        }
    }

    file << endl;
}

void saveAllProfiles(const vector<Profile>& profiles) {
    ofstream file(ENV_CONFIG.PROFILES_FILE_PATH); // lo mismo que ios::out, pero este borra todo el contenido previo

    for (size_t i = 0; i < profiles.size(); i++) {
        file << profiles[i].name << ";";
        for (int j = 0; j < profiles[i].options.size(); j++) {
            file << profiles[i].options[j];
            if (j < profiles[i].options.size() - 1) {
                file << ",";
            }
        }
        file << endl;
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
    for (size_t i = 0; i < profiles.size(); i++) {
        if (profiles[i].name == name) {
            profiles.erase(profiles.begin() + i);
            saveAllProfiles(profiles);
            return true;
        }
    }
 
    return false;
}
