#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include "Profile.h"
#include "User.h"
using namespace std;


struct MenuOption {
    string name;
    void (*callback)(vector<Profile>&, vector<User>& users);
};

// muestra el menu principal (0=Exit, 1=Users, 2=Profiles) y corre
// el programa hasta que el usuario elige salir. las listas se pasan
// por referencia para que los datos persistan durante toda la ejecucion.
void runMainMenu(vector<Profile>& profiles, vector<User>& users);

#endif
