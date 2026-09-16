#ifndef ADMINMENU_H
#define ADMINMENU_H

#include <string>
#include <vector>
#include "Profile.h"
#include "User.h"

using namespace std;

void adminUsersProfilesMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath);

// Menu de usuarios
void runUserMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath);

// Menu de perfiles
void runProfileMenu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath);

#endif