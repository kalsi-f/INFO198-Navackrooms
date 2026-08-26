#include "Menu.h"

#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>
using namespace std;

namespace {
// ingreso de saltos de linea -> ENTER
void clearBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printProfiles(vector<Profile>& profiles) {
    for (int i = 0; i < profiles.size(); i++) {
        cout << i + 1 << ") " << profiles[i].name << endl;
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
void createProfileMenu(vector<Profile>& profiles, bool& profilesLoaded, const string& profileFile) {
    listProfiles(profiles, profilesLoaded, profileFile);

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

    createProfile(profiles, profilesLoaded, profileFile, p);
    cout << "Perfil creado." << endl;
}
// LISTAR PERFIL
void listProfilesMenu(vector<Profile>& profiles, bool& profilesLoaded, const string& profileFile) {
    listProfiles(profiles, profilesLoaded, profileFile);

    for (Profile& p : profiles) {
        cout << p.name << " -> opciones: ";
        for (int op : p.options) {
            cout << op << " ";
        }
        cout << endl;
    }
}
// ELIMINAR PERFIL
void deleteProfileMenu(vector<Profile>& profiles, bool& profilesLoaded, const string& profileFile,
                        vector<User>& users, bool& usersLoaded, const string& userFile) {

    listProfiles(profiles, profilesLoaded, profileFile);
    listUsers(users, usersLoaded, userFile, profiles);   

    cout << "Nombre del perfil a eliminar: ";
    string namef;
    getline(cin, namef);

    bool hasUsers = false;                                 
    for (User& u : users) {                                 
        if (u.profile->name == namef) {                      
            hasUsers = true;                                 
            break;                                            
        }                                                     
    }                                                          

    if (hasUsers) {                                            
        cout << "Error: no es posible eliminar, ya existen usuarios con este perfil asignado." << endl;
        return;                                                 
    }                                                           

    bool deleted = deleteProfile(profiles, profilesLoaded, profileFile, namef);
    if (deleted) {
        cout << "Perfil eliminado." << endl;
    } else {
        cout << "Error: no existe un perfil con ese nombre." << endl;
    }
}

void runProfileMenu(vector<Profile>& profiles, bool& profilesLoaded, const string& profileFile,
                     vector<User>& users, bool& usersLoaded, const string& userFile)  {
    int option = -1;

    while (option != 0) {
        cout << "\n-- Menu de Perfiles --" << endl;
        cout << "0) Volver" << endl;
        cout << "1) Crear Perfil" << endl;
        cout << "2) Listar Perfiles" << endl;
        cout << "3) Eliminar Perfil" << endl;
        cout << "Opcion: ";

        cin >> option;
        clearBuffer();

        switch (option) {
            case 1:
                createProfileMenu(profiles, profilesLoaded, profileFile);
                break;
            case 2:
                listProfilesMenu(profiles, profilesLoaded, profileFile);
                break;
            case 3:
                deleteProfileMenu(profiles, profilesLoaded, profileFile, users, usersLoaded, userFile);
                break;
        }
    }
}

// MENU DE USUARIOS
// pide el Id repitiendo hasta que sea valido y no exista todavia.
// devuelve true y deja el id en "id" si se ingreso correctamente,
// false si el usuario cancelo dejando la linea vacia.
bool askNewUserId(vector<User>& users, int& id) {
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
        for (User& existing : users) {
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

// muestra los perfiles y pide una opcion repitiendo hasta que sea valida.
// devuelve el puntero elegido, o nullptr si el usuario cancelo dejando vacio.
Profile* askProfileChoice(vector<Profile>& profiles) {
    while (true) {
        cout << "Elija un perfil (vacio para cancelar):" << endl;
        printProfiles(profiles);
        cout << "Opcion: ";
        string text;
        getline(cin, text);

        if (text.empty()) {
            return nullptr;
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
            return &profiles[option - 1];
        }
    }
}

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
void createUserMenu(vector<User>& users, bool& usersLoaded, const string& userFile,
                     vector<Profile>& profiles, bool& profilesLoaded, const string& profileFile) {
    listProfiles(profiles, profilesLoaded, profileFile);
    listUsers(users, usersLoaded, userFile, profiles);

    User u;
    int id;
    if (!askNewUserId(users, id)) {
        cout << "Creacion cancelada." << endl;
        return;
    }
    u.id = id;

    string name = askNonEmpty("Nombre");
    strncpy(u.name, name.c_str(), sizeof(u.name) - 1);
    u.name[sizeof(u.name) - 1] = '\0';

    string username = askNonEmpty("Username");
    strncpy(u.username, username.c_str(), sizeof(u.username) - 1);
    u.username[sizeof(u.username) - 1] = '\0';

    string password = askNonEmpty("Password");
    strncpy(u.password, password.c_str(), sizeof(u.password) - 1);
    u.password[sizeof(u.password) - 1] = '\0';

    Profile* chosen = askProfileChoice(profiles);
    if (chosen == nullptr) {
        cout << "Creacion cancelada." << endl;
        return;
    }
    u.profile = chosen;

    createUser(users, usersLoaded, userFile, profiles, u);
    cout << "Usuario creado." << endl;
}

// LISTAR USUARIO 
void listUsersMenu(vector<User>& users, bool& usersLoaded, const string& userFile,
                    vector<Profile>& profiles, bool& profilesLoaded, const string& profileFile) {
    listProfiles(profiles, profilesLoaded, profileFile);
    listUsers(users, usersLoaded, userFile, profiles);

    for (User& u : users) {
        cout << u.id << " - " << u.name << " (" << u.username << ") perfil=" << u.profile->name << endl;
    }
}

// ELIMINAR USUARIO
void deleteUserMenu(vector<User>& users, bool& usersLoaded, const string& userFile,
                     vector<Profile>& profiles, bool& profilesLoaded, const string& profileFile) {
    listProfiles(profiles, profilesLoaded, profileFile);
    listUsers(users, usersLoaded, userFile, profiles);

    cout << "Id del usuario a eliminar: ";
    int id;
    cin >> id;
    clearBuffer();

    User* target = nullptr;
    for (User& u : users) {
        if (u.id == id) {
            target = &u;
            break;
        }
    }

    if (target == nullptr) {
        cout << "Error: no existe un usuario con ese id." << endl;
        return;
    }

    if (target->profile->name == "ADMIN") {
        cout << "Alerta: estas eliminando un usuario ADMIN. Deseas continuar? (s/n): ";
        string confirm;
        getline(cin, confirm);

        if (confirm != "s" && confirm != "S") {
            cout << "Eliminacion cancelada." << endl;
            return;
        }
    }

    deleteUser(users, usersLoaded, userFile, profiles, id);
    cout << "Usuario eliminado." << endl;
}

void runUserMenu(vector<User>& users, bool& usersLoaded, const string& userFile,
                  vector<Profile>& profiles, bool& profilesLoaded, const string& profileFile) {
    int option = -1;

    while (option != 0) {
        cout << "\n-- Menu de Usuarios --" << endl;
        cout << "0) Volver" << endl;
        cout << "1) Crear Usuario" << endl;
        cout << "2) Listar Usuarios" << endl;
        cout << "3) Eliminar Usuario" << endl;
        cout << "Opcion: ";

        cin >> option;
        clearBuffer();

        switch (option) {
            case 1:
                createUserMenu(users, usersLoaded, userFile, profiles, profilesLoaded, profileFile);
                break;
            case 2:
                listUsersMenu(users, usersLoaded, userFile, profiles, profilesLoaded, profileFile);
                break;
            case 3:
                deleteUserMenu(users, usersLoaded, userFile, profiles, profilesLoaded, profileFile);
                break;
        }
    }
}

} 

// MENU PRINCIPAL

void runMainMenu(vector<Profile>& profiles, bool& profilesLoaded,
                  vector<User>& users, bool& usersLoaded,
                  const string& userFile, const string& profileFile) {
    int option = -1;

    while (option != 0) {
        cout << "\n-- Menu Principal --" << endl;
        cout << "0) Salir" << endl;
        cout << "1) Gestion de Usuarios" << endl;
        cout << "2) Gestion de Perfiles" << endl;
        cout << "Opcion: ";

        cin >> option;
        clearBuffer();

        switch (option) {
            case 1:
                runUserMenu(users, usersLoaded, userFile, profiles, profilesLoaded, profileFile);
                break;
            case 2:
                runProfileMenu(profiles, profilesLoaded, profileFile, users, usersLoaded, userFile);
                break;
        }
    }
}