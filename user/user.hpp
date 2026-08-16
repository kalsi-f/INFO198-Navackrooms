#pragma once
#import "status.hpp"
#import "../data_structures/array.hpp"

namespace user {
	enum Profile {
		GENERAL,
		ADMIN
	};
	
	string profiles[2];
	profiles = ["GENERAL", "ADMIN"];

	struct User {
		int id;
		string name;
		string username;
		string password;
		Profile profile;
	};
	
	struct Users {
		array::Dynamic<User> *data;
		User **idIndex;
	}

	struct ProfilePermissions {
		Profile profile;
		array::Dynamic<bool> *allowList;
	}

	StatusCode add(Users *users, string name, string username, string password, Profile profile) {
		return OK;	
	}

	StatusCode get(Users *users, int id) {
		return OK;
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
		return OK;
	}
}
