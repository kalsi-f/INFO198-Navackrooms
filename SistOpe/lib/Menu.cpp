#include "Menu.h"
#include "Auth.h"
#include "Profile.h"
#include "AdminMenu.h"
#include "config/EnvConfig.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>

using namespace std;

void clear_screen() {
    cout << "\033[2J\033[H";
}

void display_title(const string& title) {
    cout << "————————————————————————————————" << endl;
    cout << title << endl;
    cout << "————————————————————————————————" << endl;
}

void display_user(vector<Profile>& profiles, Users& users, int userId) {
    User* user = findUserById(users, userId);
    if (user != nullptr) {
        cout << "Usuario: " << user->name << " (" << user->username << ")" << endl;
        cout << "Perfil: " << profiles[user->profileIndex].name << endl;
        cout << "————————————————————————————————" << endl;
    }
}

void display_error(const string& error) {
    cerr << "Error: " << error << endl;
    cout << "Presione ENTER para continuar...";
    cin.get();
}

int get_option() {
    string input;
    int option = -1;

    cout << endl << "Opcion: ";

    if (!getline(cin, input)) option = -1;

    try {
        size_t pos;
        option = stoi(input, &pos);
        if (pos != input.size()) option = -1;
    }
    catch (...) {
        option = -1;
    }

    return option;
}

int simple_menu(vector<string> options) {
    for (size_t i = 0; i < options.size(); i++) {
        cout << i << ". " << options[i] << endl;
    }

    int option = get_option();

    if (option < 0 || option >= static_cast<int>(options.size())) 
        display_error("Opcion invalida.");

    return option;
 }

void program_menu(
    vector<Profile>&profiles, 
    Users& users, 
    const int userId, 
    const string& title, vector<ProgramOption> programs, 
    const string& exitName
) {
    int option = -1;

    while (option != 0) {
        clear_screen();
        display_title(title);
        display_user(profiles, users, userId);
        
        cout << "0. " << exitName << endl;
    
        // Listar programas
        for (size_t i = 0; i < programs.size(); i++) {
            cout << i+1 << ". " << programs[i].name << endl;
        }

        option = get_option();
        if (option == 0) break;

        if (option < 0 || option > static_cast<int>(programs.size())) {
            display_error("Opcion invalida.");
            continue;
        }

        // Validar que el programa tenga permiso
        if (isUserAllowed(profiles, users, userId, option)) {
            // La opcion empieza del indice 1 y el vector comienza desde el indice 0 por lo que hay que restarle 1 a la opcion
            ProgramOption programOption = programs[option-1];
            string command = programOption.path;

            for (string& arg: programOption.args)
                command += " " + arg;


            if (system(command.c_str()) != 0) 
                display_error("El programa finalizo incorrectamente.");
        }
        else 
            display_error("Acceso denegado. Permisos insuficientes.");

    }
}

void dynamicListMenu(
    vector<Profile>& profiles, 
    Users& users, 
    int userId, 
    const string& filePath,    
    const string& title, 
    vector<MenuOption>& options, 
    const string& exitName
) {
    int option = -1;

    while (option != 0) {
        clear_screen();
        display_title(title);
        display_user(profiles, users, userId);

        cout << "0. " << exitName << endl;
        for (const MenuOption& menuOption : options) {
            cout << menuOption.id << ". " << menuOption.name << endl;
        }

        option = get_option();
        if (option == 0) break;

        bool found = false;
        for (MenuOption& menuOption : options) {
            if (menuOption.id == option) {
                clear_screen();
                menuOption.callback(profiles, users, userId, filePath);
                found = true;
                break;
            }
        }

        if (!found) display_error("Opcion invalida.");
    }
}

void countSystemMenu(vector<Profile>&, Users&, int, const string& filePath, int mode) {
    if (mode == 6) {
        setenv("COUNT_MODE", "6", 1);
        setenv("COUNT_FILE", filePath.c_str(), 1);
    } else {
        setenv("COUNT_MODE", "7", 1);
        unsetenv("COUNT_FILE");
    }

    string command = getEnvVar("COUNT_PROGRAM");
    int returnCode = system(command.c_str());

    if (returnCode != 0) {
        cout << "Error: no se pudo ejecutar el sistema de conteo." << endl;
        cout << "Presione ENTER para continuar...";
        cin.get();
    }
}

void main_menu(vector<Profile>& profiles, Users& users, int userId, const string& filePath) {
    vector<ProgramOption> programs = {
        {"Administración de usuarios y perfiles", getEnvVar("ADMIN_PROGRAM"), {findUserById(users, userId)->username}},
        {"Multiplicar matrices n*m", getEnvVar("MATRIX_MULTIPLICATION_PROGRAM"), {"-i"}},
        {"Juego", getEnvVar("GAME_PROGRAM"), {}},
        {"Es palíndromo?", getEnvVar("PALINDROME_PROGRAM"), {}},
        {"Calcular f(x) = x^2 + 2x + 8", getEnvVar("FX_PROGRAM"), {}},
        {"Conteo sobre texto", getEnvVar("COUNT_PROGRAM"), {filePath}},
        {"Conteo sobre archivo", getEnvVar("COUNT_PROGRAM"), {}},
    };

    program_menu(
        profiles, 
        users, 
        userId, 
        "MENU PRINCIPAL", 
        programs, 
        "Salir");
    cout << "Finalizacion del programa exitosa." << endl;
}