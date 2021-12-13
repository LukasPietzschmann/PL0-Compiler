#pragma once

#include <initializer_list>
#include <iostream>
#include <string>

#include "redefine_yylex.hpp"

YY_DECL;

bool parse();

void set_error(std::string_view message);
token get_token();

template <typename... Args>
bool match(Args... types);
template <typename... Args>
bool consume(Args... types);

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