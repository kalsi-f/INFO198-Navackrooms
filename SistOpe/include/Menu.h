#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include "Profile.h"
#include "User.h"

using namespace std;

struct MenuOption {
    int id;
    string name;
    void (*callback)(vector<Profile>& profiles, Users& users,int loggedUserId, const string& filePath);
};

void clearScreen();

void adminUsersProfilesSystemMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath);


void dynamicListMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath,
                    const string& title,vector<MenuOption>& options,const string& exitName = "Retroceder");

void runMainMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath);

#endif
