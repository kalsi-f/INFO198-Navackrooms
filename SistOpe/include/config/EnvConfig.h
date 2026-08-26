#ifndef ENVCONFIG_H // necesario para evitar redefiniciones del mismo header
#define ENVCONFIG_H // necesario para evitar redefiniciones del mismo header

#include <string>
using namespace std;

// Variables globales
// Usar variables globales solo para constantes
struct EnvConfig {
    string PROFILES_FILE_PATH; // Path por defecto
    string USERS_FILE_PATH;    // Path por defecto
};

extern struct EnvConfig ENV_CONFIG;

void loadEnv(const string& path = ".env");
 
string getEnvVar(const string& key);

#endif 