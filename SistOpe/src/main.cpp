#include "config/EnvConfig.h"
#include "Profile.h"
#include "User.h"
#include "Menu.h"

using namespace std;

int main() {
    loadEnv();

    vector<Profile> profiles;
    bool profilesLoaded = false;

    vector<User> users;
    bool usersLoaded = false;

    string userFile = getEnvVar("USER_FILE");
    string profileFile = getEnvVar("PERFIL_FILE");

    runMainMenu(profiles, profilesLoaded, users, usersLoaded, userFile, profileFile);

    return 0;
}