#pragma once


namespace user {
	enum profileEnum {
		GENERAL,
		ADMIN
	};

	struct data {
		int id;
		string name;
		string username;
		string password;
		profileEnum profile;
	};
}
