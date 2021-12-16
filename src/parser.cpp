#include "parser.hpp"

token current_token;

bool parse() {
	context::the().level_up();
	current_token = get_token();
	program();
	if(!match(END_OF_FILE))
		SET_ERROR("Found additional characters after the end of the program");
	return has_error;
}

token get_token() { return yylex(); }

void program() {
	block();
	consume(DOT);
}

void block() {
	if(match(CONST)) {
		const auto& verify_ident = []() {
			const auto& ident = consume(IDENT);
			consume(EQUAL);
			const auto& value = consume(NUMBER);
			if(ident.has_value() && value.has_value()) {
				if(context::the().insert(ident->read_value.as_string(), context::t_const, value->read_value.as_int()) ==
						context::c_already_declared)
					SET_ERROR("Constant " + ident->read_value.as_string() + " already declared");
			}
		};

		verify_ident();
		while(match(COMMA))
			verify_ident();
		consume(SEMICOLON);
	}
	if(match(VAR)) {
		const auto& verify_ident = []() {
			const auto& ident = consume(IDENT);
			if(ident.has_value()) {
				if(context::the().insert(ident->read_value.as_string(), context::t_var) == context::c_already_declared)
					SET_ERROR("Variable " + ident->read_value.as_string() + " already declared");
			}
		};

		verify_ident();
		while(match(COMMA))
			verify_ident();
		consume(SEMICOLON);
	}
	while(match(PROCEDURE)) {
		const auto& ident = consume(IDENT);
		if(ident.has_value()) {
			if(context::the().insert(ident->read_value.as_string(), context::t_procedure, -1) ==
					context::c_already_declared)
				SET_ERROR("Procedure " + ident->read_value.as_string() + " already declared");
		}
		consume(SEMICOLON);
		context::the().level_up();
		block();
		consume(SEMICOLON);
		context::the().level_down();
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
	const auto& ident = consume(IDENT);
	if(ident.has_value()) {
		int level_delta;
		int value;
		if(context::the().lookup(ident->read_value.as_string(), context::t_var, level_delta, value) &
				(context::c_wrong_type | context::c_not_found))
			SET_ERROR("Could not find variable " + ident->read_value.as_string());
	}
	consume(ASSIGNMENT);
	expression();
}

void call() {
	consume(CALL);
	const auto& ident = consume(IDENT);
	if(ident.has_value()) {
		int level_delta;
		int value;
		if(context::the().lookup(ident->read_value.as_string(), context::t_procedure, level_delta, value) &
				(context::c_wrong_type | context::c_not_found))
			SET_ERROR("Could not find procedure " + ident->read_value.as_string());
	}
}

void get() {
	consume(GET);
	const auto& ident = consume(IDENT);
	if(ident.has_value()) {
		int level_delta;
		int value;
		if(context::the().lookup(ident->read_value.as_string(), context::t_var, level_delta, value) &
				(context::c_wrong_type | context::c_not_found))
			SET_ERROR("Could not find variable " + ident->read_value.as_string());
	}
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
		if(match(EQUAL, HASH, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL))
			expression();
	}
}

void expression() {
	match(PLUS, MINUS);
	term();
	while(match(PLUS, MINUS))
		term();
}

void term() {
	factor();
	while(match(STAR, SLASH))
		factor();
}

void factor() {
	static const auto verify_ident = [](const std::optional<token>& ident) {
		if(ident.has_value()) {
			int level_delta;
			int value;
			if(context::the().lookup(ident->read_value.as_string(),
					   context::t_var | context::t_const,
					   level_delta,
					   value) &
					(context::c_wrong_type | context::c_not_found))
				SET_ERROR("Could not find variable or constant " + ident->read_value.as_string());
		}
	};

	switch(current_token.type) {
		case IDENT: verify_ident(consume(IDENT)); break;
		case NUMBER: consume(NUMBER); break;
		case PAREN_OPEN:
			consume(PAREN_OPEN);
			expression();
			consume(PAREN_CLOSE);
		default: SET_ERROR("Unexpected Token"); break;
	}
}

template <typename... Args>
bool match(Args... types) {
	for(const auto& type : {types...}) {
		if(current_token.type != type)
			continue;
		current_token = get_token();
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