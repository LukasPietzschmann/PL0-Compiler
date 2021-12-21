#include "parser.hpp"

token current_token;

bool parse() {
	current_token = get_token();
	program();
	if(!match_and_advance(END_OF_FILE))
		SET_ERROR("Found additional characters after the end of the program");
	return has_error;
}

token inline get_token() { return yylex(); }

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
	if(match(IDENT))
		assignment();
	else if(match(CALL))
		call();
	else if(match(GET))
		get();
	else if(match(POST))
		post();
	else if(match(BEG))
		begin();
	else if(match(IF))
		condition();
	else if(match(WHILE))
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
	while(match_and_advance(SEMICOLON))
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
	if(match_and_advance(ODD))
		expression();
	else {
		expression();
		if(match_and_advance(EQUAL, HASH, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL))
			expression();
	}
}

void expression() {
	match_and_advance(PLUS, MINUS);
	term();
	while(match_and_advance(PLUS, MINUS))
		term();
}

void term() {
	factor();
	while(match_and_advance(STAR, SLASH))
		factor();
}

void factor() {
	switch(current_token.type) {
		case IDENT:
		case NUMBER: consume(NUMBER, IDENT); break;
		case PAREN_OPEN:
			consume(PAREN_OPEN);
			expression();
			consume(PAREN_CLOSE);
		default: SET_ERROR("Unexpected Token"); break;
	}
}

template <typename... Args>
bool match_and_advance(Args... types) {
	if(!match(types...))
		return false;
	current_token = get_token();
	return true;
}

template <typename... Args>
bool match(Args... types) {
	for(const auto& type : {types...}) {
		if(current_token.type != type)
			continue;
		return true;
	}
	return false;
}

template <typename... Args>
std::optional<token> consume(Args... types) {
	token old_token = current_token;
	for(const auto& type : {types...}) {
		if(current_token.type != type)
			continue;
		current_token = get_token();
		return old_token;
	}

	std::string token_names;
	for(const auto& type : {types...}) {
		token_names += get_name_for_type(type);
		token_names += ", or";
	}
	token_names = token_names.substr(token_names.size() - 6, 5);

	std::string error_msg = "Expected token " + token_names + ", but got " + get_name_for_type(current_token.type);
	SET_ERROR(error_msg);

	return {};
}