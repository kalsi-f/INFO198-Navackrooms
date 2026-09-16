#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include "Profile.h"
#include "User.h"

using namespace std;

struct ProgramOption {
    string name;
    string path;
    vector<string> args;
};

struct MenuOption {
    int id;
    string name;
    void (*callback)(vector<Profile>& profiles, Users& users,int loggedUserId, const string& filePath);
};

void clear_screen();

void display_error(const string& error);

int simple_menu(vector<string> options);

void program_menu(
    vector<Profile>& profiles, 
    Users& users, 
    int userId, 
    const string& title, 
    vector<ProgramOption> programs,
    const string& exitName = "Retroceder"
);

void dynamicListMenu(
    vector<Profile>& profiles, 
    Users& users,int loggedUserId, 
    const string& filePath, 
    const string& title, 
    vector<MenuOption>& options, 
    const string& exitName = "Retroceder"
);

void main_menu(vector<Profile>& profiles,Users& users,int loggedUserId,const string& filePath);

#endif
