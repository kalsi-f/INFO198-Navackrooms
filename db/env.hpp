#pragma once
#include <string>

namespace db::env {
	string get(string key) {
		const char* value = std::getenv(key);
		if (value == nullptr) return "";
		return std::string(value);
	}

	string get_user_file_path() {
		return get("USER_FILE");
	}

	string get_profile_file_path() {
		return get("PROFILE_FILE");
	}	
}
