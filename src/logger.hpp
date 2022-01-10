#pragma once

#include <iostream>
#include <string_view>

#include "current_token.hpp"
#include "token.hpp"

#define SET_ERROR(msg) set_error(msg, current_token)

extern bool has_error;
void set_error(std::string_view message, const token& current_token);