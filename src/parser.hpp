#pragma once

#include <initializer_list>
#include <optional>
#include <string>

#include "context.hpp"
#include "logger.hpp"
#include "oplist.hpp"
#include "redefine_yylex.hpp"

YY_DECL;

#define SET_ERROR(msg) set_error(msg, current_token)
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

std::optional<oplist> parse();

token get_token();

template <typename... Args>
std::optional<token_type> match_and_advance(Args... types);
template <typename... Args>
std::optional<token_type> match(Args... types);
template <typename... Args>
std::optional<token> consume(Args... types);

oplist::ptr program();

oplist::ptr block();

oplist::ptr statement();
oplist::ptr assignment();
oplist::ptr call();
oplist::ptr get();
oplist::ptr post();
oplist::ptr begin();
oplist::ptr condition();
oplist::ptr loop();

expr_tree::ptr comparison();
expr_tree::ptr expression();
expr_tree::ptr term();
expr_tree::ptr factor();

oplist::ptr get_last_entry_in_list(oplist::ptr list);