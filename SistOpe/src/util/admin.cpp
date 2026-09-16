#include "config/EnvConfig.h"
#include "Profile.h"
#include "User.h"
#include "AdminMenu.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    loadEnv();

    ENV_CONFIG.PROFILES_FILE_PATH =
        getEnvVar("PERFIL_FILE");

    ENV_CONFIG.USERS_FILE_PATH =
        getEnvVar("USER_FILE");

    const char* username = getenv("ADMIN_USERNAME");

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

    if (loggedUser == nullptr ||
        loggedUser->profileIndex < 0 ||
        loggedUser->profileIndex >= static_cast<int>(profiles.size()) ||
        strcmp(profiles[loggedUser->profileIndex].name, "ADMIN") != 0) {
        cerr << "Error: solo el perfil ADMIN puede acceder a esta opcion." << endl;
        return 1;
    }

    adminUsersProfilesMenu(
        profiles,
        users,
        loggedUser->id,
        ""
    );

    return 0;
}
