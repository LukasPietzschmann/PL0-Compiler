#include <lexer.h>
#include <stdbool.h>
#include <token_type.h>

enum token_type current_token;
bool has_error;

void set_error(char* message);
enum token_type get_token();

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
	int r;
	do {
		r = get_token();
		printf("%d\n", r);
	} while(r != 0);
	return 0;
}

void set_error(char* message) {
	has_error = true;
	printf("%s\n", message);
}

enum token_type get_token() { return yylex(); }

bool match(enum token_type type) {
	if(current_token == type) {
		current_token = get_token();
		return true;
	}

	set_error("Unexpected token\n");
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
		if(current_token == EQUAL || current_token == HASH || current_token == LESS || current_token == LESS_EQUAL ||
				current_token == GREATER || current_token == GREATER_EQUAL) {
			current_token = get_token();
			expression();
		}
	}
}

void expression() {
	if(current_token == PLUS || current_token == MINUS) {
		current_token = get_token();
		term();
	}
	while(current_token == PLUS || current_token == MINUS) {
		current_token = get_token();
		term();
	}
}

void term() {
	factor();
	while(current_token == STAR || current_token == SLASH) {
		current_token = get_token();
		factor();
	}
}

void factor() {
	switch(current_token) {
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
	if(current_token == IDENT)
		assignment();
	else if(current_token == CALL)
		call();
	else if(current_token == GET)
		get();
	else if(current_token == POST)
		post();
	else if(current_token == BEG)
		begin();
	else if(current_token == IF)
		condition();
	else if(current_token == WHILE)
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
