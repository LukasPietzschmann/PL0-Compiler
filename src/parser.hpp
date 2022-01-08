#pragma once

#include <initializer_list>
#include <iostream>
#include <optional>
#include <string>

#include "logger.hpp"
#include "oplist.hpp"
#include "redefine_yylex.hpp"

YY_DECL;

#define SET_ERROR(msg) set_error(msg, current_token)

std::optional<oplist> parse();

token get_token();

template <typename... Args>
std::optional<token_type> match_and_advance(Args... types);
template <typename... Args>
std::optional<token_type> match(Args... types);
template <typename... Args>
std::optional<token> consume(Args... types);

oplist* program();

oplist* block();

oplist* statement();
oplist* assignment();
oplist* call();
oplist* get();
oplist* post();
oplist* begin();
oplist* condition();
oplist* loop();

expr_tree* comparison();
expr_tree* expression();
expr_tree* term();
expr_tree* factor();

oplist* get_last_entry_in_list(oplist& list);