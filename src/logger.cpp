#include "logger.hpp"

bool has_error;

void set_error(std::string_view message, const token& current_token) {
	has_error = true;
	std::cout << "[Line " << current_token.line_number << "] " << message << std::endl;
}