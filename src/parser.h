#pragma once

#include <redefine_yylex.h>
#include <lexer.h>
#include <stdarg.h>
#include <stdbool.h>
#include <token.h>
#include <token_type.h>

YY_DECL;

token current_token;
bool has_error;

bool parse();

void set_error(char* message);
token get_token();

bool match(token_type type);
bool match_one_of(int argc, ...);
bool consume(token_type type);
bool consume_one_of(int argc, ...);

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