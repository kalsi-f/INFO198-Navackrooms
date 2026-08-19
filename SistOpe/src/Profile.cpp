#include "Profile.h"

using namespace std;

vector<Profile> loadProfiles(const string& path) {
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo '" << path << "'" << endl;
        exit(1);
    }

    vector<Profile> profiles;
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

        profiles.push_back(p);
    }

    return profiles;
}

void appendProfile(const string& path, const Profile& p) {
    ofstream file(path, ios::app); // ios app para agregar al final sin borrar lo que existe

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