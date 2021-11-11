#include <redefine_yylex.h>
#include <lexer.h>
#include <stdbool.h>
#include <token.h>
#include <token_type.h>

YY_DECL;

token current_token;
bool has_error;

void set_error(char* message);
token get_token();

bool match(token_type type);
bool consume(token_type type);

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

int main(int argc, char** argv) {
	if(argc > 1) {
		FILE* input = fopen(*(argv + 1), "r");
		if(!input)
			return -1;
		yyin = input;
	}
	current_token = get_token();
	program();
	return 0;
}

void set_error(char* message) {
	has_error = true;
	printf("%s\n", message);
}

token get_token() {
	return yylex();
}

bool match(token_type type) {
	if(current_token.type == type) {
		current_token = get_token();
		return true;
	}
	return false;
}

bool consume(token_type type) {
	if(match(type))
		return true;

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
	consume(GET);
	consume(IDENT);
}

void post() {
	consume(POST);
	expression();
}

void begin() {
	consume(BEG);
	statement();
	while(match(SEMICOLON))
		statement();
	consume(END);
}

void condition() {
	consume(IF);
	comparison();
	consume(THEN);
	statement();
}

void loop() {
	consume(WHILE);
	comparison();
	consume(DO);
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
	if(current_token.type == PLUS || current_token.type == MINUS)
		current_token = get_token();
	term();
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
		case IDENT: consume(IDENT); break;
		case NUMBER: consume(NUMBER); break;
		case PAREN_OPEN:
			consume(PAREN_OPEN);
			expression();
			consume(PAREN_CLOSE);
		default: set_error("Unexpected Token"); break;
	}
}
void program() {
	block();
	consume(DOT);
}

void block() {
	if(match(CONST)) {
		consume(IDENT);
		consume(EQUAL);
		consume(NUMBER);
		while(match(COMMA)) {
			consume(IDENT);
			consume(EQUAL);
			consume(NUMBER);
		}
		consume(SEMICOLON);
	}
	if(match(VAR)) {
		consume(IDENT);
		while(match(COMMA))
			consume(IDENT);
		consume(SEMICOLON);
	}
	while(match(PROCEDURE)) {
		consume(IDENT);
		consume(SEMICOLON);
		block();
		consume(SEMICOLON);
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
}

void assignment() {
	consume(IDENT);
	consume(ASSIGNMENT);
	expression();
}

void call() {
	consume(CALL);
	consume(IDENT);
}