#include <parser.h>

bool parse() {
	current_token = get_token();
	program();
	if(!match(END_OF_FILE))
		set_error("Found additional characters after the end of the program");
	return has_error;
}

void set_error(char* message) {
	has_error = true;
	printf("[Line %d] %s\n", current_token.line_number, message);
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

	char* error_msg = (char*)malloc(sizeof(char) * (30 + strlen(token_names) + 10));
	sprintf(error_msg, "Expected token %s, but got %s", token_names, get_name_for_type(current_token.type));
	set_error(error_msg);

	free(token_names);
	free(error_msg);
	va_end(args);
	return false;
}