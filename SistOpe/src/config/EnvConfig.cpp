#include "config/EnvConfig.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;
 
void loadEnv(const string& path) {
    ifstream file(path);
 
    if (!file.is_open()) {
        cerr << "Error: no se pudo abrir el archivo .env en '" << path << "'" << endl;
        exit(1);
    }
 
    string line;
    while (getline(file, line)) {
        int equalPos = line.find('=');
        string key = line.substr(0, equalPos);
        string value = line.substr(equalPos + 1);
 
        setenv(key.c_str(), value.c_str(), 1);
    }
}
 
string getEnvVar(const string& key) {
    const char* value = getenv(key.c_str());
 
    if (value == nullptr) {
        cerr << "Error: la variable de entorno '" << key << "' no esta definida" << endl;
        exit(1);
    }
 
    return string(value);
}
 

