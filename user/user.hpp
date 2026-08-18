#pragma once
#import "status.hpp"
#import "../data_structures/array.hpp"

#define NAME_SIZE 64
#define USERNAME_SIZE 32
#define PASSWORD_SIZE 32

namespace user {
	enum Profile {
		GENERAL,
		ADMIN
	};
	
	string profiles = ["GENERAL", "ADMIN"];

	struct User {
		int64_t id;
		char name[NAME_SIZE];
		char username[USERNAME_SIZE];
		char password[PASSWORD_SIZE];
		Profile profile;
	};
	
	struct Users {
		array::Dynamic<User> data;
		User **idIndex;
	}

	struct ProfilePermissions {
		Profile profile;
		array::Dynamic<bool> *allowList;
	}

	StatusCode _validate_user(string name, string username, string password, Profile profile);
	StatusCode add(Users *users, string name, string username, string password, Profile profile);
	User* get(Users *users, int id);
	StatusCode delete(Users *users, int id);

	StatusCode _validate_user(string name, string username, string password, Profile profile) {
		if (name.length() == 0) return ERROR_NAME_IS_EMPTY;
		if (name.length() > NAME_SIZE) return ERROR_NAME_IS_TOO_LONG;
		if (username.length() == 0) return ERROR_USERNAME_IS_EMPTY;
		if (username.length > USERNAME_SIZE) return ERROR_USERNAME_IS_TOO_LONG;
		if (password.length() == 0) return ERROR_PASSWORD_IS_EMPTY;
		if (password.length() > PASSWORD_SIZE) return ERROR_PASSWORD_IS_TOO_LONG;
		return OK;
	}

	StatusCode add(Users *users, string name, string username, string password, Profile profile) {
		StatusCode statusCode = _validate_user(name, username, password, profile);

		if (statusCode == OK) {
			array::push_back(
				users->data, 
					{
						name,
						username,
						password,
						profile,
					}
			);
		}

		return statusCode;	
	}

	User* get(Users *users, int id) {
		return users->idIndex[id];
	}

	/*
	StatusCode update(Users *users, int id, string username, string password, Profile profile) {
		if (username == "") return ERROR_USERNAME_IS_EMPTY;
		if (password == "") return ERROR_PASSWORD_IS_EMPTY;
		return OK;
	}
	*/

	StatusCode delete(Users *users, int id) {
		if (profile == ADMIN) return ERROR_USER_IS_ADMIN;
		User* user = get(users, id);
		if (user == nullptr) return ERROR_USER_NOT_FOUND;
		
		array::pop(users->data, user);	

		return OK;
	}
}
