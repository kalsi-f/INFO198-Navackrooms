#include "AdminMenu.h"
#include "Menu.h"

#include <cstring>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

namespace {

bool askNonEmpty(const string& label, size_t maxLength, string& value, bool showCancelHint = false) {
    while (true) {
        cout << label;
        if (showCancelHint) {
            cout << " (ENTER para cancelar)";
        }
        cout << ": ";
        if (!getline(cin, value)) {
            return false;
        }

        if (value.empty()) {
            cout << "Operacion cancelada." << endl;
            return false;
        }

        if (value.size() >= maxLength) {
            cout << "Error: el campo no puede superar "
                 << maxLength - 1 << " caracteres." << endl;
            continue;
        }

        return true;
    }
}

bool copyToField(char* destination, size_t capacity, const string& value) {
    if (value.size() >= capacity) {
        return false;
    }
    strncpy(destination, value.c_str(), capacity - 1);
    destination[capacity - 1] = '\0';
    return true;
}

bool usernameExists(const Users& users, const string& username) {
    for (const User& user : users.data) {
        if (username == user.username) {
            return true;
        }
    }
    return false;
}

bool parseInteger(const string& text, int& value) {
    try {
        size_t pos;
        value = stoi(text, &pos);
        return pos == text.size();
    } catch (...) {
        return false;
    }
}

void printProfiles(const vector<Profile>& profiles) {
    for (size_t i = 0; i < profiles.size(); i++) {
        cout << "ID: " << i + 1 << " - " << profiles[i].name << endl;
    }
}

vector<int> parseOptions(const string& text) {
    vector<int> result;
    stringstream ss(text);
    string token;

    while (getline(ss, token, ',')) {
        if (token.empty()) {
            throw invalid_argument("opcion vacia");
        }

        int value;
        if (!parseInteger(token, value)) {
            throw invalid_argument("opcion invalida");
        }
        if (value < 0) {
            throw invalid_argument("opcion negativa");
        }
        result.push_back(value);
    }

    return result;
}

int askProfileChoice(vector<Profile>& profiles) {
    while (true) {
        cout << "(ENTER para cancelar)" << endl;
        cout << "Seleccione el ID del perfil:" << endl;
        printProfiles(profiles);
        cout << "ID: ";

        string text;
        if (!getline(cin, text) || text.empty()) {
            return -1;
        }

        int option;
        if (!parseInteger(text, option) || option < 1 || option > static_cast<int>(profiles.size())) {
            cout << "Error: opcion invalida." << endl;
            continue;
        }

        return option - 1;
    }
}

} 


// CREAR PERFIL
void createProfileMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    (void)users;
    (void)loggedUserId;
    (void)filePath;

    Profile p{};
    string nameInput;

    cout << "(ENTER para cancelar)" << endl;

    while (true) {
        cout << "Nombre del perfil: ";
        if (!getline(cin, nameInput)) {
            return;
        }
        if (nameInput.empty()) {
            cout << "Creacion cancelada." << endl;
            return;
        }
        if (nameInput.size() >= sizeof(p.name)) {
            cout << "Error: el nombre no puede superar " << sizeof(p.name) - 1 << " caracteres." << endl;
            continue;
        }

        bool exists = false;
        for (const Profile& existing : profiles) {
            if (strcmp(existing.name, nameInput.c_str()) == 0) {
                exists = true;
                break;
            }
        }
        if (exists) {
            cout << "Error: ya existe un perfil con ese nombre." << endl;
            continue;
        }

        copyToField(p.name, sizeof(p.name), nameInput);
        break;
    }

    while (true) {
        cout << "Opciones separadas por coma: ";
        string optionsText;
        if (!getline(cin, optionsText)) {
            cout << "Creacion cancelada." << endl;
            return;
        }

        if (optionsText.empty()) {
            cout << "Creacion cancelada." << endl;
            return;
        }

        try {
            vector<int> parsed = parseOptions(optionsText);
            if (parsed.size() > static_cast<size_t>(MAX_OPTIONS)) {
                cout << "Error: no se permiten mas de " << MAX_OPTIONS << " opciones." << endl;
                continue;
            }

            for (size_t i = 0; i < parsed.size(); ++i) {
                p.options[i] = parsed[i];
            }
            p.optionsCount = static_cast<int>(parsed.size());
            break;
        } catch (const invalid_argument&) {
            cout << "Error: ingrese solo numeros no negativos separados por coma." << endl;
        }
    }

    createProfile(profiles, p);
    cout << "Perfil creado." << endl;
    cout << "Presione ENTER para continuar...";
    cin.get();
}


// LISTAR PERFILES
void listProfilesMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    (void)users;
    (void)loggedUserId;
    (void)filePath;

    for (const Profile& p : profiles) {
        cout << p.name << " -> opciones: ";
        for (int i = 0; i < p.optionsCount; ++i) {
            cout << p.options[i] << " ";
        }
        cout << endl;
    }

    cout << endl << "Presione ENTER para continuar...";
    cin.get();
}


// ELIMINAR PERFIL
void deleteProfileMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    (void)filePath;

    int profileIndexDelete = askProfileChoice(profiles);

    if (profileIndexDelete == -1) {
        cout << "Eliminacion cancelada." << endl;
        return;
    }

    string profileName = profiles[profileIndexDelete].name;

    User* loggedUser = findUserById(users, loggedUserId);
    if (loggedUser != nullptr && loggedUser->profileIndex == profileIndexDelete) {
        cout << "Error: no se puede eliminar el perfil activo." << endl;
        cout << "Presione ENTER para continuar...";
        string pause;
        getline(cin, pause);
        return;
    }

    for (const User& user : users.data) {
        if (user.profileIndex == profileIndexDelete) {
            cout << "Error: no se puede eliminar el perfil porque hay usuarios asociados a este perfil." << endl;
            cout << "Presione ENTER para continuar...";
            cin.get();
            return;
        }
    }

    bool loggedProfileAfterDelete = false;
    loggedUser = findUserById(users, loggedUserId);
    if (loggedUser != nullptr && loggedUser->profileIndex > profileIndexDelete) {
        loggedProfileAfterDelete = true;
    }

    if (!deleteProfile(profiles, profileName)) {
        cout << "Error al borrar el perfil." << endl;
        return;
    }

    if (loggedProfileAfterDelete) {
        loggedUser = findUserById(users, loggedUserId);
        if (loggedUser != nullptr) {
            loggedUser->profileIndex--;
        }
    }

    saveAllUsers(users, profiles);
    cout << "Perfil eliminado." << endl;
}


// CREAR USUARIO
void createUserMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    (void)filePath;

    User u{};
    string value;

    cout << "(ENTER para cancelar)" << endl;
    if (!askNonEmpty("Nombre", sizeof(u.name), value)) return;
    if (!copyToField(u.name, sizeof(u.name), value)) return;

    if (!askNonEmpty("Username", sizeof(u.username), value)) return;
    if (usernameExists(users, value)) {
        cout << "Error: ya existe un usuario con ese username." << endl;
        return;
    }
    if (!copyToField(u.username, sizeof(u.username), value)) return;

    if (!askNonEmpty("Password", sizeof(u.password), value)) return;
    if (!copyToField(u.password, sizeof(u.password), value)) return;

    int chosen = askProfileChoice(profiles);
    if (chosen == -1) {
        cout << "Creacion cancelada." << endl;
        return;
    }
    u.profileIndex = chosen;

    createUser(profiles, users, u);
    cout << "Usuario creado." << endl;
}


// LISTAR USUARIOS
void listUsersMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    (void)loggedUserId;
    (void)filePath;

    for (const User& u : users.data) {
        cout << u.id << " - " << u.name << " (" << u.username << ") perfil=";
        if (u.profileIndex >= 0 && u.profileIndex < static_cast<int>(profiles.size())) {
            cout << profiles[u.profileIndex].name;
        } else {
            cout << "INVALIDO";
        }
        cout << endl;
    }

    cout << endl << "Presione ENTER para continuar...";
    cin.get();
}


// ELIMINAR USUARIO
void deleteUserMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    (void)filePath;

    cout << "Usuarios disponibles:" << endl;
    for (const User& u : users.data) {
        cout << "ID: " << u.id << " - " << u.name << " (" << u.username << ") perfil=";
        if (u.profileIndex >= 0 && u.profileIndex < static_cast<int>(profiles.size())) {
            cout << profiles[u.profileIndex].name;
        } else {
            cout << "INVALIDO";
        }
        cout << endl;
    }

    cout << endl << "(ENTER para cancelar)" << endl;
    cout << "Id del usuario a eliminar: ";
    string input;
    if (!getline(cin, input) || input.empty()) {
        cout << "Eliminacion cancelada." << endl;
        return;
    }

    int id;
    if (!parseInteger(input, id)) {
        cout << "Error: debe ingresar un id numerico valido." << endl;
        return;
    }

    User* target = findUserById(users, id);
    if (target == nullptr) {
        cout << "Error: no existe un usuario con ese id." << endl;
        return;
    }

    User* loggedUser = findUserById(users, loggedUserId);
    if (loggedUser != nullptr &&
        strcmp(target->username, loggedUser->username) == 0) {
        cout << "No se pudo eliminar el usuario porque el usuario esta activo." << endl;
        cout << "Presione ENTER para continuar...";
        cin.get();
        return;
    }

    if (target->profileIndex >= 0 &&
        target->profileIndex < static_cast<int>(profiles.size()) &&
        strcmp(profiles[target->profileIndex].name, "ADMIN") == 0) {
        cout << "Alerta: estas eliminando un usuario ADMIN." << endl;
        cout << "Deseas continuar? (s/n): ";

        string confirm;
        if (!getline(cin, confirm) || confirm.empty()) {
            cout << "Eliminacion cancelada." << endl;
            return;
        }

        if (confirm != "s" && confirm != "S") {
            cout << "Eliminacion cancelada." << endl;
            return;
        }
    }

    if (deleteUser(profiles, users, id)) {
        cout << "Usuario eliminado." << endl;
    } else {
        cout << "Error al eliminar el usuario." << endl;
    }
}


// MENÚ DE USUARIOS
void runUserMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    vector<MenuOption> options = {
        {1, "Crear Usuario", createUserMenu},
        {2, "Listar Usuarios", listUsersMenu},
        {3, "Eliminar Usuario", deleteUserMenu}
    };

    dynamicListMenu(profiles, users, loggedUserId, filePath, "MENU DE USUARIOS", options);
}


// MENÚ DE PERFILES
void runProfileMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    vector<MenuOption> options = {
        {1, "Crear Perfil", createProfileMenu},
        {2, "Listar Perfiles", listProfilesMenu},
        {3, "Eliminar Perfil", deleteProfileMenu}
    };

    dynamicListMenu(profiles, users, loggedUserId, filePath, "MENU DE PERFILES", options);
}


// ADMINISTRACIÓN DE USUARIOS Y PERFILES
void adminUsersProfilesMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath) {
    vector<MenuOption> options = {
        {1, "Gestion de Usuarios", runUserMenu},
        {2, "Gestion de Perfiles", runProfileMenu}
    };

    dynamicListMenu(profiles, users, loggedUserId, filePath, "ADMINISTRACION DE USUARIOS Y PERFILES", options);
}