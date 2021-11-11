#include <redefine_yylex.h>

#include <lexer.h>
#include <stdbool.h>
#include <token.h>
#include <token_type.h>

YY_DECL;

struct token current_token;
bool has_error;

void set_error(char* message);
struct token get_token();

bool match(enum token_type type);

void program();

void block();
void const_declaration();
void var_decalaration();
void procedure();

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

int main() {
	current_token = get_token();
	program();
	return 0;
}

void set_error(char* message) {
	has_error = true;
	printf("%s\n", message);
}

struct token get_token() {
	return yylex();
}

bool match(enum token_type type) {
	if(current_token.type == type) {
		current_token = get_token();
		return true;
	}

	char error_msg[60];
	sprintf(error_msg,
			"[line %d] Expected token %s but got %s",
			current_token.line_number,
			get_name_for_type(type),
			get_name_for_type(current_token.type));
	set_error(error_msg);
	return false;
}

void get() {
	match(GET);
	assignment();
}

void post() {
	match(POST);
	expression();
}

void begin() {
	match(BEG);
	statement();
	while(match(SEMICOLON)) {
		statement();
	}
	match(END);
}

void condition() {
	match(IF);
	comparison();
	match(THEN);
	statement();
}

void loop() {
	match(WHILE);
	condition();
	match(DO);
	statement();
}

void comparison() {
	if(match(ODD))
		expression();
	else {
		expression();
		if(current_token.type == EQUAL || current_token.type == HASH || current_token.type == LESS ||
				current_token.type == LESS_EQUAL || current_token.type == GREATER ||
				current_token.type == GREATER_EQUAL) {
			current_token = get_token();
			expression();
		}
	}
}

void expression() {
	if(current_token.type == PLUS || current_token.type == MINUS) {
		current_token = get_token();
		term();
	}
	while(current_token.type == PLUS || current_token.type == MINUS) {
		current_token = get_token();
		term();
	}
}

void term() {
	factor();
	while(current_token.type == STAR || current_token.type == SLASH) {
		current_token = get_token();
		factor();
	}
}

void factor() {
	switch(current_token.type) {
		case IDENT: match(IDENT); break;
		case NUMBER: match(NUMBER); break;
		case PAREN_OPEN: expression(); match(PAREN_CLOSE);
		default: set_error("Unexpected Token"); break;
	}
}
void program() {
	block();
	match(DOT);
}

void block() {
	if(match(CONST)) {
		match(IDENT);
		match(EQUAL);
		match(NUMBER);
		while(match(COMMA)) {
			match(IDENT);
			match(EQUAL);
			match(NUMBER);
		}
		match(SEMICOLON);
	}
	if(match(VAR)) {
		match(IDENT);
		while(match(COMMA))
			match(IDENT);
		match(SEMICOLON);
	}
	while(match(PROCEDURE)) {
		match(IDENT);
		match(SEMICOLON);
		block();
		match(SEMICOLON);
	}
	statement();
}

void statement() {
	if(current_token.type == IDENT)
		assignment();
	else if(current_token.type == CALL)
		call();
	else if(current_token.type == GET)
		get();
	else if(current_token.type == POST)
		post();
	else if(current_token.type == BEG)
		begin();
	else if(current_token.type == IF)
		condition();
	else if(current_token.type == WHILE)
		loop();
	else
		set_error("No matching Statement");
}

void assignment() {
	match(IDENT);
	match(ASSIGNMENT);
	expression();
}

void call() {
	match(CALL);
	match(IDENT);
}
