#pragma once

#include <iostream>
#include <string_view>

#include "token.hpp"

extern bool has_error;
void set_error(std::string_view message, const token& current_token);