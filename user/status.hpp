#pragma once

namespace user {
	enum StatusCodes: int {
		OK,
		ERROR_USER_IS_ADMIN,
		ERROR_USERNAME_IS_EMPTY,
		ERROR_PASSWORD_IS_EMPTY
	};
}
