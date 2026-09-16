#include "config/EnvConfig.h"
#include "Profile.h"
#include "User.h"
#include "Auth.h"
#include "AdminMenu.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    loadEnv();

    ENV_CONFIG.PROFILES_FILE_PATH =
        getEnvVar("PERFIL_FILE");

    ENV_CONFIG.USERS_FILE_PATH =
        getEnvVar("USER_FILE");

    if (argc != 2) {
        cerr << "Error: Número de argumentos invalido" << endl;
        cout << "Uso: " << argv[0] << " <USUARIO>" << endl;
        return 1;
    }

    const char* username = argv[1];

    if (username == nullptr) {
        cerr << "Error: usuario administrador no especificado." << endl;
        return 1;
    }

    vector<Profile> profiles = loadProfiles();

    Users users = loadUsers(profiles);

    User* loggedUser = nullptr;

    for (User& user : users.data) {
        if (strcmp(user.username, username) == 0) {
            loggedUser = &user;
            break;
        }
    }

    adminUsersProfilesMenu(
        profiles,
        users,
        loggedUser->id,
        ""
    );

    return 0;
}
