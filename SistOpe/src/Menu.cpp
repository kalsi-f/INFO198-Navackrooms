
#include "Menu.h"

#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>

#define CLEAR_SCREEN_CODE "\033[2J\033[3J\033[2J\033[1;1H"  // Deberia limpiar la pantalla

using namespace std;


// ingreso de saltos de linea -> ENTER
void clearBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void dynamicListMenu(vector<Profile>& profiles, Users& users, const string &title, vector<MenuOption> &options) {
    string input;
    int option = -1;

    cout << CLEAR_SCREEN_CODE;
    
    while (option != 0) {
        cout << "— " << title << " —" << endl;
        cout << "0. Retroceder" << endl;
        for (size_t i = 0; i < options.size(); i++) {
            cout << i + 1 << ". " << options[i].name << endl; 
        }

        cout << "Opcion: ";
        cin >> input;
        try {
            option = stoi(input);
        }
        catch (const std::invalid_argument & e) {
            
        }
        clearBuffer();
        cout << CLEAR_SCREEN_CODE;

        if (0 < option && option <= options.size()) options[option-1].callback(profiles, users);
        else if (option == 0) break;
        else cout << "Error: Opción inválida" << endl;

    }
}

void printProfiles(vector<Profile>& profiles) {
    for (size_t i = 0; i < profiles.size(); i++) {
        cout << i + 1 << ". " << profiles[i].name << endl;
    }
}

// convierte "0,1,3" en {0,1,3}. lanza invalid_argument si algun
// pedazo no corresponde al formato abc o separar por puntos.
vector<int> parseOptions(const string& text) {
    vector<int> result;
    stringstream ss(text);
    string format;

    while (getline(ss, format, ',')) {
        size_t pos;
        int value = stoi(format, &pos);
        if (pos != format.size()) {
            throw invalid_argument("formato invalido: " + format);
        }
        result.push_back(value);
    }

    return result;
}

// MENU DE PERFILES
// CREAR PERFIL
void createProfileMenu(vector<Profile>& profiles) {
    Profile p;

    while (true) {
        cout << "Nombre del perfil (vacio para cancelar): ";
        getline(cin, p.name);

        if (p.name.empty()) {
            cout << "Creacion cancelada." << endl;
            return;
        }

        bool exists = false;
        for (Profile& existing : profiles) {
            if (existing.name == p.name) {
                exists = true;
                break;
            }
        }

        if (exists) {
            cout << "Error: ya existe un perfil con ese nombre." << endl;
        } else {
            break;
        }
    }

    while (true) {
        cout << "Opciones separadas por coma (ej: 0,1,3), vacio para ninguna: ";
        string optionsText;
        getline(cin, optionsText);

        if (optionsText.empty()) {
            break; // perfil sin opciones, es valido
        }

        try {
            p.options = parseOptions(optionsText);
            break;
        } catch (const invalid_argument&) {
            cout << "Error: ingrese solo numeros separados por coma." << endl;
        }
    }

    createProfile(profiles, p);
    cout << "Perfil creado." << endl;
}
// LISTAR PERFIL
void listProfilesMenu(vector<Profile>& profiles) {

    for (Profile& p : profiles) {
        cout << p.name << " -> opciones: ";
        for (int op : p.options) {
            cout << op << " ";
        }
        cout << endl;
    }
}

// muestra los perfiles y pide una opcion repitiendo hasta que sea valida.
// devuelve el puntero elegido, o nullptr si el usuario cancelo dejando vacio.
int askProfileChoice(vector<Profile>& profiles) {
    while (true) {
        cout << "Elija un perfil (vacio para cancelar):" << endl;
        printProfiles(profiles);
        cout << "Opcion: ";
        string text;
        getline(cin, text);

        if (text.empty()) {
            return -1;
        }

        int option;
        try {
            size_t pos;
            option = stoi(text, &pos);
            if (pos != text.size()) {
                throw invalid_argument("opcion invalida");
            }
        } catch (const invalid_argument&) {
            cout << "Error: debe ingresar un numero valido." << endl;
            continue;
        }

        if (option < 1 || option > (int)profiles.size()) {
            cout << "Error: opcion invalida. Intente de nuevo." << endl;
        } else {
            return option - 1;
        }
    }
}

void deleteProfileMenu(vector<Profile>& profiles, Users& users) {
    cout << "Seleccione el perfil a eliminar:" << endl;
    int profile_index_delete = askProfileChoice(profiles);
    
    if (profile_index_delete == -1) {
        cout << "Eliminacion cancelada." << endl;
        return;
    }
    
    
    string namef = profiles[profile_index_delete].name; 
    
    vector<User*> users_to_update;

    for (User& u : users.data) {
        if (u.profile_index == profile_index_delete) {
            cout << "Error: ya existen usuarios con este perfil asignado." << endl;
            return; 
        }
        if (u.profile_index > profile_index_delete) {
            users_to_update.push_back(&u);
        }
    } 

    
    bool deleted = deleteProfile(profiles, namef);

    if (deleted) {
        for (User* target_user : users_to_update) {
            target_user->profile_index--;
        }
        
        saveAllUsers(users, profiles); 
        cout << "Perfil eliminado" << endl;
    } else {
        cout << "Error: al borrar el perfil." << endl;
    }
}

void createProfileMenu(vector<Profile>& profiles, Users& users) {
    return createProfileMenu(profiles);
}

void listProfilesMenu(vector<Profile>& profiles, Users& users) {
    return listProfilesMenu(profiles);
}

void runProfileMenu(vector<Profile>& profiles, Users& users) {
    vector<MenuOption> options =  {
        {"Crear Perfil", createProfileMenu},
        {"Listar Perfiles", listProfilesMenu},
        {"Eliminar Perfil", deleteProfileMenu}
    };

    dynamicListMenu(profiles, users, "Menu de Perfiles", options);
}

// MENU DE USUARIOS
// pide el Id repitiendo hasta que sea valido y no exista todavia.
// devuelve true y deja el id en "id" si se ingreso correctamente,
// false si el usuario cancelo dejando la linea vacia.
/*
bool askNewUserId(Users& users, int& id) {
    while (true) {
        cout << "Id (vacio para cancelar): ";
        string idText;
        getline(cin, idText);

        if (idText.empty()) {
            return false;
        }

        try {
            size_t pos;
            id = stoi(idText, &pos);
            if (pos != idText.size()) {
                throw invalid_argument("id invalido");
            }
        } catch (const invalid_argument&) {
            cout << "Error: debe ingresar un numero valido." << endl;
            continue;
        }

        bool exists = false;
        for (User& existing : users.data) {
            if (existing.id == id) {
                exists = true;
                break;
            }
        }

        if (exists) {
            cout << "Error: ya existe un usuario con ese id. Intente con otro." << endl;
        } else {
            return true;
        }
    }
}
*/

// pide una linea de texto, repitiendo hasta que no quede vacia
string askNonEmpty(const string& label) {
    while (true) {
        cout << label << ": ";
        string text;
        getline(cin, text);

        if (!text.empty()) {
            return text;
        }

        cout << "Error: este campo no puede quedar vacio." << endl;
    }
}

// CREAR USUARIO
void createUserMenu(vector<Profile>& profiles, Users& users) {

    User u;
    u.id = users.currentId + 1;

    string name = askNonEmpty("Nombre");
    strncpy(u.name, name.c_str(), sizeof(u.name) - 1);
    u.name[sizeof(u.name) - 1] = '\0';

    string username = askNonEmpty("Username");
    strncpy(u.username, username.c_str(), sizeof(u.username) - 1);
    u.username[sizeof(u.username) - 1] = '\0';

    string password = askNonEmpty("Password");
    strncpy(u.password, password.c_str(), sizeof(u.password) - 1);
    u.password[sizeof(u.password) - 1] = '\0';

    int chosen = askProfileChoice(profiles);
    if (chosen == -1) {
        cout << "Creacion cancelada." << endl;
        return;
    }
    u.profile_index = chosen;

    createUser(profiles, users, u);

    //users.currentId++; esto se hace em createUSER.

    cout << "Usuario creado." << endl;
}

// LISTAR USUARIO 
void listUsersMenu(vector<Profile>& profiles, Users& users) {
    for (User& u : users.data) {
        cout << u.id << " - " << u.name << " (" << u.username << ") perfil=" << profiles[u.profile_index].name << endl;
    }
}

// ELIMINAR USUARIO
void deleteUserMenu(vector<Profile>& profiles, Users& users) {

    cout << "Id del usuario a eliminar: ";
    int id;
    cin >> id;
    clearBuffer();

    User* target = nullptr;
    for (User& u : users.data) {
        if (u.id == id) {
            target = &u;
            break;
        }
    }

    if (target == nullptr) {
        cout << "Error: no existe un usuario con ese id." << endl;
        return;
    }

    if (profiles[target->profile_index].name == "ADMIN") {
        cout << "Alerta: estas eliminando un usuario ADMIN. Deseas continuar? (s/n): ";
        string confirm;
        getline(cin, confirm);

        if (confirm != "s" && confirm != "S") {
            cout << "Eliminacion cancelada." << endl;
            return;
        }
    }

    deleteUser(profiles, users, id);
    cout << "Usuario eliminado." << endl;
}

void runUserMenu(vector<Profile>& profiles, Users& users) {
    vector<MenuOption> options = {
        {"Crear Usuario", createUserMenu},
        {"Listar Usuarios", listUsersMenu},
        {"Eliminar Usuario", deleteUserMenu}
    };

    dynamicListMenu(profiles, users, "Menu de Usuarios", options);
}

// MENU PRINCIPAL

void runMainMenu(vector<Profile>& profiles, Users& users) {
    vector<MenuOption> options = {
        {"Gestion de Usuarios", runUserMenu},
        {"Gestion de Perfiles", runProfileMenu}
    };

    dynamicListMenu(profiles, users, "Menu Principal", options);
    cout << "Finalización del programa exitosa" << endl;
}
