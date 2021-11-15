#include <redefine_yylex.h>
#include <lexer.h>
#include <stdarg.h>
#include <stdbool.h>
#include <token.h>
#include <token_type.h>

YY_DECL;

token current_token;
bool has_error;

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

int main(int argc, char** argv) {
	if(argc > 1) {
		FILE* input = fopen(*(argv + 1), "r");
		if(!input)
			return -1;
		yyin = input;
	}
	current_token = get_token();
	program();
	has_error = has_error || !match(END_OF_FILE);
	return has_error;
}

void set_error(char* message) {
	has_error = true;
	printf("%s\n", message);
}

token get_token() { return yylex(); }

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
		if(match_one_of(6, EQUAL, HASH, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL))
			expression();
	}
}

void expression() {
	match_one_of(2, PLUS, MINUS);
	term();
	while(match_one_of(2, PLUS, MINUS))
		term();
}

void term() {
	factor();
	while(match_one_of(2, STAR, SLASH))
		factor();
}

void factor() {
	switch(current_token.type) {
		case IDENT:
		case NUMBER: consume_one_of(2, NUMBER, IDENT); break;
		case PAREN_OPEN:
			consume(PAREN_OPEN);
			expression();
			consume(PAREN_CLOSE);
		default: set_error("Unexpected Token"); break;
	}
}

bool inline match(token_type type) { return match_one_of(1, type); }

bool match_one_of(int argc, ...) {
	va_list args;
	va_start(args, argc);

	for(int i = 0; i < argc; ++i) {
		if(current_token.type != va_arg(args, token_type))
			continue;
		current_token = get_token();
		va_end(args);
		return true;
	}
	va_end(args);
	return false;
}

bool inline consume(token_type type) { return consume_one_of(1, type); }

bool consume_one_of(int argc, ...) {
	va_list args;
	va_start(args, argc);

	for(int i = 0; i < argc; ++i) {
		if(current_token.type != va_arg(args, token_type))
			continue;
		current_token = get_token();
		va_end(args);
		return true;
	}

	char* token_names = (char*)malloc(sizeof(char) * 10 * argc);
	va_start(args, argc);
	for(int i = 0; i < argc; ++i) {
		strcat(token_names, get_name_for_type(va_arg(args, token_type)));
		strncat(token_names, ", or ", 5);
	}
	token_names[strlen(token_names) - 5] = '\0';

	char* error_msg = (char*)malloc(sizeof(char) * (40 + 10 + strlen(token_names)));
	sprintf(error_msg,
			"[line %d] Expected token %s, but got %s",
			current_token.line_number,
			token_names,
			get_name_for_type(current_token.type));
	set_error(error_msg);

	free(token_names);
	free(error_msg);
	va_end(args);
	return false;
}