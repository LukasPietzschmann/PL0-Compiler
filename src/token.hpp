#pragma once

#include "token_type.hpp"

using token = struct token {
	token_type type;
	unsigned int line_number;
};