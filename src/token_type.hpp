#pragma once

#include <string>

enum token_type {
	END_OF_FILE = 0,
	DOT = 1,
	CONST = 2,
	IDENT = 3,
	EQUAL = 4,
	NUMBER = 5,
	COMMA = 6,
	SEMICOLON = 7,
	VAR = 8,
	PROCEDURE = 9,
	ASSIGNMENT = 10,
	CALL = 11,
	GET = 12,
	POST = 13,
	BEG = 14,
	END = 15,
	IF = 16,
	THEN = 17,
	WHILE = 18,
	DO = 19,
	ODD = 20,
	HASH = 21,
	LESS = 22,
	LESS_EQUAL = 23,
	GREATER = 24,
	GREATER_EQUAL = 25,
	PLUS = 26,
	MINUS = 27,
	STAR = 28,
	SLASH = 29,
	PAREN_OPEN = 30,
	PAREN_CLOSE = 31,
	UNDEFINED_CHAR = 32
};

std::string get_name_for_type(token_type t);