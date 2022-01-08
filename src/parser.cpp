#include "parser.hpp"

token current_token;

std::optional<oplist> parse() {
	current_token = get_token();
	auto* list = program();
	if(!match_and_advance(END_OF_FILE))
		SET_ERROR("Found additional characters after the ^ of the program");
	if(has_error)
		return {};
	return *list;
}

token inline get_token() { return yylex(); }

oplist* program() {
	auto* list = block();
	consume(DOT);
	return list;
}

oplist* block() {
	auto* start = new oplist();
	auto* current_list_entry = start;
	if(match(CONST)) {
		const auto& parse_const_assignment = [&current_list_entry]() {
			const auto& ident = consume(IDENT);
			consume(EQUAL);
			const auto& number = consume(NUMBER);
			if(ident.has_value() && number.has_value()) {
				auto* const_assignment = new oplist(oplist::t_assignment, ident->lexeme.as_string());
				const_assignment->set_expr(new expr_tree(number->lexeme.as_int()));
				get_last_entry_in_list(*current_list_entry)->set_next(const_assignment);
				current_list_entry = const_assignment;
			}
		};
		parse_const_assignment();
		while(match_and_advance(COMMA))
			parse_const_assignment();
		consume(SEMICOLON);
	}
	if(match_and_advance(VAR)) {
		const auto& parse_var_decl = [&current_list_entry]() {
			if(const auto& ident = consume(IDENT); ident.has_value()) {
				auto* var_decl = new oplist(oplist::t_assignment, ident->lexeme.as_string());
				get_last_entry_in_list(*current_list_entry)->set_next(var_decl);
				current_list_entry = var_decl;
			}
		};
		parse_var_decl();
		while(match_and_advance(COMMA))
			parse_var_decl();
		consume(SEMICOLON);
	}
	while(match_and_advance(PROCEDURE)) {
		auto* proc_decl = new oplist();
		if(const auto& ident = consume(IDENT); ident.has_value()) {
			proc_decl = new oplist(oplist::t_assignment, ident->lexeme.as_string());
		}
		consume(SEMICOLON);
		proc_decl->set_next(block());
		consume(SEMICOLON);
		get_last_entry_in_list(*current_list_entry)->set_next(proc_decl);
		current_list_entry = proc_decl;
	}

	get_last_entry_in_list(*current_list_entry)->set_next(statement());

	std::cout << *start << std::endl;
	return start;
}

oplist* statement() {
	if(match(IDENT))
		return assignment();
	else if(match(CALL))
		return call();
	else if(match(GET))
		return get();
	else if(match(POST))
		return post();
	else if(match(BEG))
		return begin();
	else if(match(IF))
		return condition();
	else if(match(WHILE))
		return loop();
	return new oplist();
}

oplist* assignment() {
	const auto& ident = consume(IDENT);
	if(ident.has_value()) {
		consume(ASSIGNMENT);
		auto* a = new oplist(oplist::t_assignment, ident->lexeme.as_string());
		a->set_expr(expression());
		return a;
	}
	return new oplist();
}

oplist* call() {
	// TODO context in den parser packen
	// TODO CALL ist dann ein unbedingter sprint auf den BEGIN Knoten der Proc.
	consume(CALL);
	const auto& ident = consume(IDENT);
	if(ident.has_value())
		return new oplist(oplist::t_call, ident->lexeme.as_string());
	return new oplist();
}

oplist* get() {
	consume(GET);
	const auto& ident = consume(IDENT);
	if(ident.has_value())
		return new oplist(oplist(oplist::t_get, ident->lexeme.as_string()));
	return new oplist();
}

oplist* post() {
	consume(POST);
	auto* post = new oplist((oplist(oplist::t_post)));
	post->set_expr(expression());
	return post;
}

oplist* begin() {
	consume(BEG);
	auto* stmt = statement();
	auto* stmt_to_set_next_on = stmt;
	while(match_and_advance(SEMICOLON)) {
		auto* next = statement();
		get_last_entry_in_list(*stmt_to_set_next_on)->set_next(next);
		stmt_to_set_next_on = next;
	}
	consume(END);
	return stmt;
}

oplist* condition() {
	consume(IF);
	auto* if_stmt = new oplist(oplist::t_cond_jmp);
	auto* nop = new oplist(oplist::t_nop);
	if_stmt->set_expr(comparison());
	consume(THEN);
	auto* stmt = statement();
	if_stmt->set_next(stmt);
	stmt->set_next(nop);
	if_stmt->set_jmp(nop);
	return if_stmt;
}

oplist* loop() {
	consume(WHILE);
	auto* while_loop = new oplist(oplist::t_cond_jmp);
	auto* jmp = new oplist(oplist::t_jmp);
	auto* nop = new oplist(oplist::t_nop);
	while_loop->set_expr(comparison());
	consume(DO);
	auto* stmt = statement();
	while_loop->set_next(stmt);
	jmp->set_jmp(while_loop);
	stmt->set_next(jmp);
	stmt->set_next(nop);
	while_loop->set_jmp(nop);
	return while_loop;
}

expr_tree* comparison() {
	if(match_and_advance(ODD))
		return new expr_tree(nullptr, expression(), ODD);
	else {
		auto* lhs = expression();
		if(const auto& token = match_and_advance(EQUAL, HASH, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL))
			lhs = new expr_tree(lhs, expression(), token.value());
		return lhs;
	}
}

expr_tree* expression() {
	match_and_advance(PLUS, MINUS);
	auto* lhs = term();
	while(const auto& token = match_and_advance(PLUS, MINUS))
		lhs = new expr_tree(lhs, term(), token.value());

	return lhs;
}

expr_tree* term() {
	auto* lhs = factor();
	while(const auto& token = match_and_advance(STAR, SLASH))
		lhs = new expr_tree(lhs, factor(), token.value());

	return lhs;
}

expr_tree* factor() {
	expr_tree* expr;
	switch(current_token.type) {
		case IDENT:
			if(const auto& ident = consume(IDENT); ident.has_value())
				return new expr_tree(ident->lexeme.as_string());
		case NUMBER:
			if(const auto& ident = consume(NUMBER); ident.has_value())
				return new expr_tree(ident->lexeme.as_int());
		case PAREN_OPEN:
			consume(PAREN_OPEN);
			expr = expression();
			consume(PAREN_CLOSE);
			return expr;
		default: SET_ERROR("Unexpected Token"); break;
	}
}

template <typename... Args>
std::optional<token_type> match_and_advance(Args... types) {
	if(const auto& res = match(types...); res.has_value()) {
		current_token = get_token();
		return res;
	}
	return {};
}

template <typename... Args>
std::optional<token_type> match(Args... types) {
	for(const auto& type : {types...}) {
		if(current_token.type != type)
			continue;
		return type;
	}
	return {};
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
	token_names = token_names.substr(0, token_names.size() - 4);

	std::string error_msg = "Expected token " + token_names + ", but got " + get_name_for_type(current_token.type);
	SET_ERROR(error_msg);

	return {};
}

inline oplist* get_last_entry_in_list(oplist& list) {
	if(list.get_next() == nullptr)
		return &list;
	auto* next = list.get_next();
	while(next->get_next() != nullptr)
		next = next->get_next();
	return next;
}