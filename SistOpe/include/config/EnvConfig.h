#ifndef ENVCONFIG_H // necesario para evitar redefiniciones del mismo header
#define ENVCONFIG_H // necesario para evitar redefiniciones del mismo header

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

void loadEnv(const string& path = ".env");
 
string getEnvVar(const string& key);

#endif 