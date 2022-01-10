// Based on the implementation of Prof. Dr. Winfried Bantel(https://bantel.informatik.hs-aalen.de)

#pragma once

#include <cassert>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

#include "logger.hpp"

#define UNEXPECTED_TYPE(ident_name, type_name) \
	SET_ERROR((ident_name) + " does not have the expected type " + (context::get_name_for_type(type_name)))
#define VAR_ALREADY_DECLARED(name) SET_ERROR("Variable " + (name) + " is already declared")
#define CONST_ALREADY_DECLARED(name) SET_ERROR("Constant " + (name) + " is already declared")
#define PROCEDURE_ALREADY_DECLARED(name) SET_ERROR("Procedure " + (name) + " is already declared")
#define UNKNOWN_IDENTIFIER(name) SET_ERROR("Use of undeclared identifier " + (name))

#define LOOKUP(identifier, expected_type, out_level_delta, out_value)                                      \
	do {                                                                                                   \
		if(const auto& res = context::the().lookup(identifier, expected_type, out_level_delta, out_value); \
				res != context::c_okay) {                                                                  \
			if(res == context::c_not_found)                                                                \
				UNKNOWN_IDENTIFIER(identifier);                                                            \
			else if(res == context::c_wrong_type)                                                          \
				UNEXPECTED_TYPE(identifier, expected_type);                                                \
		}                                                                                                  \
	} while(0)

#define CHECK_TYPE(identifier, expected_type)    \
	do {                                         \
		int d;                                   \
		LOOKUP(identifier, expected_type, d, d); \
	} while(0)

class context {
public:
	enum entry_type { t_unset = -1, t_var = 1, t_const = 2, t_procedure = 4 };
	enum error_code { c_not_found = 1, c_already_declared = 2, c_wrong_type = 4, c_okay = 8 };

	static std::string get_name_for_type(int entry_type);

	struct context_entry {
		explicit context_entry(entry_type type = t_unset, int value = -1) : type(type), value(value) {}
		entry_type type;
		int value;
	};

	context& operator=(const context&) = delete;
	context& operator=(context&&) = default;
	context(const context&) = delete;
	context(context&&) = default;

	static context& the();

	void level_up();
	void level_down();

	error_code insert(const std::string& name, entry_type type, int value = 0);
	error_code lookup(const std::string& name, int type, int& out_level_delta, int& out_value);

	void print() const;
	friend std::ostream& operator<<(std::ostream& os, const context& context);

private:
	context() = default;
	std::vector<std::unordered_map<std::string, context_entry> > m_context_table;
};