#pragma once

#include <string>
#include <utility>

#include "token_type.hpp"

struct token {
	struct token_value {
		explicit token_value(std::string raw_value) : raw_value(std::move(raw_value)) {}
		std::string raw_value;
		std::string as_string() const { return raw_value; }
		int as_int() const { return std::stoi(raw_value); }
	};

	token() noexcept : type(END_OF_FILE), line_number(0), read_value("") {}
	token(token_type type, unsigned int lineNumber, char* read_string) :
		type(type), line_number(lineNumber), read_value(std::string(read_string)) {}

	token_type type;
	unsigned int line_number;
	token_value read_value;
};