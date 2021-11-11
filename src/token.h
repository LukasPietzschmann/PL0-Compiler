#pragma once

#include <token_type.h>

typedef struct {
	token_type type;
	unsigned int line_number;
} token;