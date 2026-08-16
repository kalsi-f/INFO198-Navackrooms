#pragma once
#include <string>

namespace db::env {
	string get(string key) {
		const char* value = std::getenv(key);
		if (char == nullptr) return "";
		return std::string(value);
	}

	string get_user_filepath() {
		return get("USER_FILE");
	}

	string get_profile_filepath() {
		return get("PROFILE_FILE");
	}	
}
