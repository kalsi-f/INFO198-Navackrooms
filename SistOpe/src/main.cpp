#include "config/EnvConfig.h"
#include "Profile.h"
#include "User.h"
#include "Menu.h"
#include "Auth.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    //args de ejecucion
    RunArgs args = parseArguments(argc, argv);
    
    loadEnv();
    ENV_CONFIG.PROFILES_FILE_PATH =
        getEnvVar("PERFIL_FILE");
    ENV_CONFIG.USERS_FILE_PATH =
        getEnvVar("USER_FILE");

    // cargar usuarios y perfiles
    vector<Profile> profiles =
        loadProfiles();
    Users users =
        loadUsers(profiles);

    // autenticar usuario
    int loggedUserId =
        authenticateUser(
            users,
            args.username,
            args.password
        );

    if (loggedUserId == -1) {

        cerr << "Error: credenciales invalidas."
             << endl;

        return 1;
    }

    runMainMenu(
        profiles,
        users,
        loggedUserId,
        args.filePath
    );

    return 0;
}