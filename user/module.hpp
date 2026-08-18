#pragma once
#import "status.hpp"
#import "../data_structures/array.hpp"
#import "../db/file.hpp" 
#import "user.hpp"

#define EMPTY_ARRAY_CAPACITY 256

namespace user {
	User** build_id_index(array::Dynamic<User> array) {
		
	}

	Users build() {
		array::Dynamic<User> array = db::file:load_users();
		// El arreglo es nulo. El archivo no existe.
		if (array.data == nullptr) {
			array = array::reserve<User>(EMPTY_ARRAY_CAPACITY);
		}

		return {
			array,
			build_id_index(array)
		}
	}

}
