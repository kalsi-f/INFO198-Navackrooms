#pragma once
#include "../data_structures/array.hpp"
#include "env.hpp"
#include "../user/user.hpp"
#include <filesystem>

namespace db::file {
	// TODO: arreglar posible condicion de carrera
	array::Dynamic<T> load_dynamic_array(string file_path) {
		if (std::filesystem::exists(file_path)) {
			int64_t file_size = std::filesystem::file_size(file_path);i
			int64_t size = file_size/sizeof(T);
			
			std::ifstream file_stream(file_path);
			array::Dynamic<T> array = array::build(size);

			if (file_stream.is_open()) {
				file_stream.read((char*)(array->data), file_size);
				return array;
			}
		}
		return Dynamic<T> empty;	
	}

	void write_dynamic_array(string file_path) {
		// TODO
	}

	array::Dynamic<user::User> load_users() {
		return load_dynamic_array<user::User>(db::env::get_user_file_path());

	}
}
