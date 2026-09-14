#include "Menu.h"
#include "AdminMenu.h"
#include "config/EnvConfig.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[H";
}

void dynamicListMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath,
                    const string& title,vector<MenuOption>& options,const string& exitName) {
    string input;
    int option = -1;

    while (option != 0) {
        clearScreen();

        cout << "========================================" << endl;
        cout << " " << title << endl;
        cout << "========================================" << endl;

        User* loggedUser = findUserById(users, loggedUserId);
        if (loggedUser != nullptr &&
            loggedUser->profileIndex >= 0 &&
            loggedUser->profileIndex < static_cast<int>(profiles.size())) {
            cout << "Usuario : " << loggedUser->name << endl;
            cout << "Perfil  : " << profiles[loggedUser->profileIndex].name << endl;
            cout << "----------------------------------------" << endl;
        }

        cout << "0. " << exitName << endl;
        for (const MenuOption& menuOption : options) {
            cout << menuOption.id << ". " << menuOption.name << endl;
        }

        cout << endl << "Opcion: ";

        if (!getline(cin, input)) {
            break;
        }

        if (input.empty()) {
            cout << "Error: debe seleccionar una opcion." << endl;
            cout << "Presione ENTER para continuar...";
            cin.get();
            continue;
        }

        try {
            size_t pos;
            option = stoi(input, &pos);
            if (pos != input.size()) {
                option = -1;
            }
        } catch (...) {
            option = -1;
        }

        if (option == 0) {
            break;
        }

        bool found = false;
        for (MenuOption& menuOption : options) {
            if (menuOption.id == option) {
                menuOption.callback(profiles, users, loggedUserId, filePath);
                found = true;
                break;
            }
        }

        if (!found) {
            cout << endl << "Error: Opcion invalida." << endl;
            cout << "Presione ENTER para continuar...";
            cin.get();
        }
    }
}

void matrixMultiplyMenu(vector<Profile>&, Users&, int, const string&) {
    cout << "Multiplicador de matrices: en construccion." << endl;
    cout << "Presione ENTER para continuar...";
    cin.get();
}

void gameMenu(vector<Profile>&, Users&, int, const string&) {
    cout << "Juego: en construccion." << endl;
    cout << "Presione ENTER para continuar...";
    cin.get();
}

void palindromeMenu(vector<Profile>&, Users&, int, const string&) {
    cout << "Es palindromo: en construccion." << endl;
    cout << "Presione ENTER para continuar...";
    cin.get();
}

void calculateFxMenu(vector<Profile>&, Users&, int, const string&) {
    cout << "Calcular f(x): en construccion." << endl;
    cout << "Presione ENTER para continuar...";
    cin.get();
}

void textCountMenu(vector<Profile>&, Users&, int, const string&) {
    cout << "Conteo sobre texto: en construccion." << endl;
    cout << "Presione ENTER para continuar...";
    cin.get();
}

void fileCountMenu(vector<Profile>&, Users&, int, const string&) {
    cout << "Conteo sobre archivo: en construccion." << endl;
    cout << "Presione ENTER para continuar...";
    cin.get();
}
// llamada al sistema para ejecutar el programa de administracion de usuarios y perfiles
void adminUsersProfilesSystemMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    User* loggedUser = findUserById(users, loggedUserId);

    if (loggedUser == nullptr ||
        loggedUser->profileIndex < 0 ||
        loggedUser->profileIndex >= static_cast<int>(profiles.size()) ||
        strcmp(profiles[loggedUser->profileIndex].name, "ADMIN") != 0) {
        cout << "Error: solo el perfil ADMIN puede acceder a esta opcion." << endl;
        cout << "Presione ENTER para continuar...";
        cin.get();
        return;
    }

    setenv("ADMIN_USERNAME", loggedUser->username, 1);

    string command = getEnvVar("ADMIN_PROGRAM");
    int returnCode = system(command.c_str());

    if (returnCode != 0) {
        cout << "Error: no se pudo ejecutar la administracion de usuarios y perfiles." << endl;
        cout << "Presione ENTER para continuar...";
        cin.get();
    }
}

vector<MenuOption> buildMainMenuOptions(vector<Profile>& profiles, User* loggedUser) {
    vector<MenuOption> allOptions = {
        {1, "Admin de usuarios y perfiles", adminUsersProfilesSystemMenu},
        {2, "Multiplica matrices NxM", matrixMultiplyMenu},
        {3, "Juego", gameMenu},
        {4, "Es palindromo?", palindromeMenu},
        {5, "Calcular f(x) = x*x + 2x + 8", calculateFxMenu},
        {6, "Conteo sobre texto", textCountMenu},
        {7, "Conteo sobre archivo", fileCountMenu}
    };

    vector<MenuOption> allowed;
    if (loggedUser == nullptr ||
        loggedUser->profileIndex < 0 ||
        loggedUser->profileIndex >= static_cast<int>(profiles.size())) {
        return allowed;
    }

    Profile& userProfile = profiles[loggedUser->profileIndex];

    bool isAdmin =
        strcmp(userProfile.name, "ADMIN") == 0;

    for (const MenuOption& option : allOptions) {

        if (option.id == 1 && isAdmin) {
            allowed.push_back(option);
            continue;
        }

        if (option.id == 1) {
            allowed.push_back(option);
            continue;
        }

        bool permitted = false;

        for (int i = 0;
             i < userProfile.optionsCount;
             i++) {

            if (userProfile.options[i] == option.id) {
                permitted = true;
                break;
            }
        }

        if (permitted) {
            allowed.push_back(option);
        }
    }

    return allowed;
}

void runMainMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    User* loggedUser = findUserById(users, loggedUserId);
    if (loggedUser == nullptr) {
        cout << "Error: no se encontro el usuario autenticado." << endl;
        return;
    }

    vector<MenuOption> options = buildMainMenuOptions(profiles, loggedUser);

    dynamicListMenu(
        profiles,
        users,
        loggedUserId,
        filePath,
        "MENU PRINCIPAL",
        options,
        "Salir"
    );

    clearScreen();
    cout << "Finalizacion del programa exitosa." << endl;
}
