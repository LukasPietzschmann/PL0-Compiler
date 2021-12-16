#pragma once

#include <initializer_list>
#include <iostream>
#include <optional>
#include <string>

#include "context.hpp"
#include "logger.hpp"
#include "redefine_yylex.hpp"

YY_DECL;

#define SET_ERROR(msg) set_error(msg, current_token)
#define UNEXPECTED_TYPE(ident_name, type_name) \
	SET_ERROR((ident_name) + " does not have the expected type " + (context::get_name_for_type(type_name)))
#define VAR_ALREADY_DECLARED(name) SET_ERROR("Variable " + (name) + " is already declared")
#define CONST_ALREADY_DECLARED(name) SET_ERROR("Constant " + (name) + " is already declared")
#define PROCEDURE_ALREADY_DECLARED(name) SET_ERROR("Procedure " + (name) + " is already declared")
#define UNKNOWN_IDENTIFIER(name) SET_ERROR("Use of undeclared identifier " + (name))

#define LOOKUP(identifier, type, out_level_delta, out_value)                                      \
	do {                                                                                          \
		if(const auto& res = context::the().lookup(identifier, type, out_level_delta, out_value); \
				res != context::c_okay) {                                                         \
			if(res == context::c_not_found)                                                       \
				UNKNOWN_IDENTIFIER(identifier);                                                   \
			else if(res == context::c_wrong_type)                                                 \
				UNEXPECTED_TYPE(identifier, type);                                                \
		}                                                                                         \
	} while(0)

bool parse();

token get_token();

template <typename... Args>
bool match_and_advance(Args... types);
template <typename... Args>
bool match(Args... types);
template <typename... Args>
std::optional<token> consume(Args... types);

void program();

void block();

void statement();
void assignment();
void call();
void get();
void post();
void begin();
void condition();
void loop();

void comparison();
void expression();
void term();
void factor();