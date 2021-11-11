#pragma once

#include <token_type.h>

struct token {
	enum token_type type;
	unsigned int line_number;
};
