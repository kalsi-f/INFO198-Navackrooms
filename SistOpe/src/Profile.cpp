#include "Profile.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

vector<Profile> loadProfiles(const string& path) {
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo '" << path << "'" << endl;
        exit(1);
    }

    vector<Profile> profiles; // vector de structs Profile
    string line;

    while (getline(file, line)) {
        int cutPos = line.find(';');
        string name = line.substr(0, cutPos);
        string optionsText = line.substr(cutPos + 1);

        Profile p;
        p.name = name;

        stringstream ss(optionsText);
        string option;
        while (getline(ss, option, ',')) {
            p.options.push_back(stoi(option));
        }

        profiles.push_back(p); // agrego al vector de structs Profile
    }

    return profiles;
}

void appendProfile(const string& path, const Profile& p) {
    ofstream file(path, ios::app); // ios app para agregar al final sin borrar lo que existe (tipos de ios::)

    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo '" << path << "'" << endl;
        exit(1);
    }

    file << p.name << ";";

    for (int i = 0; i < p.options.size(); i++) {
        file << p.options[i];
        if (i < p.options.size() - 1) {
            file << ",";
        }
    }

    file << endl;
}

void saveAllProfiles(const string& path, const vector<Profile>& profiles) {
    ofstream file(path); // lo mismo que ios::out, pero este borra todo el contenido previo

    for (int i = 0; i < profiles.size(); i++) {
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

vector<Profile>& listProfiles(vector<Profile>& profiles, bool& loaded, const string& path) {
    if (!loaded) {
        profiles = loadProfiles(path);
        loaded = true;
    }
    return profiles;
}

void createProfile(vector<Profile>& profiles, bool& loaded, const string& path, const Profile& p) {
    listProfiles(profiles, loaded, path); // asegura que este cargado antes de agregar
    profiles.push_back(p);
    appendProfile(path, p);
}

bool deleteProfile(vector<Profile>& profiles, bool& loaded, const string& path, const string& name) {
    listProfiles(profiles, loaded, path);
 
    for (int i = 0; i < profiles.size(); i++) {
        if (profiles[i].name == name) {
            profiles.erase(profiles.begin() + i);
            saveAllProfiles(path, profiles);
            return true;
        }
    }
 
    return false;
}
