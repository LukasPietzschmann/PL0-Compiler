#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
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

#define LOOKUP_T(identifier, expected_type, out_level_delta, out_value, out_type)                                    \
	do {                                                                                                             \
		if(const auto& res = context::the().lookup(identifier, expected_type, out_level_delta, out_value, out_type); \
				res != context::c_okay) {                                                                            \
			if(res == context::c_not_found)                                                                          \
				UNKNOWN_IDENTIFIER(identifier);                                                                      \
			else if(res == context::c_wrong_type)                                                                    \
				UNEXPECTED_TYPE(identifier, expected_type);                                                          \
		}                                                                                                            \
	} while(0)

#define LOOKUP(identifier, expected_type, out_level_delta, out_value)       \
	do {                                                                    \
		context::entry_type t;                                              \
		LOOKUP_T(identifier, expected_type, out_level_delta, out_value, t); \
	} while(0)

class stmt_list;
class context {
public:
	struct proc_table_entry {
		std::shared_ptr<stmt_list> procedure;
		int number_of_variables;
	};

	struct context_position {
		context_position(int delta, int id) : delta(delta), id(id) {}
		int delta;
		int id;
	};

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

	error_code insert(const std::string& name,
			entry_type type,
			std::optional<int> value = {},
			std::shared_ptr<stmt_list> proc_start = {});
	error_code lookup(const std::string& name,
			int type,
			int& out_level_delta,
			int& out_value,
			entry_type& out_type) const;
	const context::proc_table_entry& lookup_procedure(unsigned long number) const;
	unsigned long get_proc_count() const;

private:
	context();
	std::vector<std::unordered_map<std::string, context_entry> > m_context_table;
	std::map<int, proc_table_entry> m_proc_table;
};