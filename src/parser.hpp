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