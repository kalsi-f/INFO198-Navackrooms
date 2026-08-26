#include "config/EnvConfig.h"
#include "Profile.h"
#include "User.h"
#include "Menu.h"

using namespace std;



int main() {
    loadEnv();

    ENV_CONFIG.PROFILES_FILE_PATH = getEnvVar("PERFIL_FILE");
    ENV_CONFIG.USERS_FILE_PATH = getEnvVar("USER_FILE");

    vector<Profile> profiles = loadProfiles();
    vector<User> users = loadUsers(profiles);

    
    

    runMainMenu(profiles, users);

    return 0;
}