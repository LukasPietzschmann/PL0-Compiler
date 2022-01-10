#include "parser.hpp"

token current_token;

std::optional<oplist> parse() {
	context::the().level_up();
	current_token = get_token();
	auto list = program();
	if(!match_and_advance(END_OF_FILE))
		SET_ERROR("Found additional characters after the ^ of the program");
	if(has_error)
		return {};
	return *list;
}

token inline get_token() { return yylex(); }

oplist::ptr program() {
	auto list = block();
	consume(DOT);
	return list;
}

oplist::ptr block() {
	auto start = oplist::make_ptr();
	auto current_list_entry = start;
	if(match(CONST)) {
		const auto& parse_const_assignment = [&current_list_entry]() {
			const auto& ident = consume(IDENT);
			consume(EQUAL);
			const auto& number = consume(NUMBER);
			if(ident.has_value() && number.has_value()) {
				auto const_assignment = oplist::make_ptr(oplist::t_assignment, ident->lexeme.as_string());
				const_assignment->set_expr(expr_tree::make_ptr(number->lexeme.as_int()));
				get_last_entry_in_list(current_list_entry)->set_next(const_assignment);
				current_list_entry = const_assignment;
				if(context::the().insert(ident->lexeme.as_string(), context::t_const, number->lexeme.as_int()) ==
						context::c_already_declared)
					CONST_ALREADY_DECLARED(ident->lexeme.as_string());
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
				auto var_decl = oplist::make_ptr(oplist::t_assignment, ident->lexeme.as_string());
				get_last_entry_in_list(current_list_entry)->set_next(var_decl);
				current_list_entry = var_decl;
				if(context::the().insert(ident->lexeme.as_string(), context::t_var) == context::c_already_declared)
					VAR_ALREADY_DECLARED(ident->lexeme.as_string());
			}
		};
		parse_var_decl();
		while(match_and_advance(COMMA))
			parse_var_decl();
		consume(SEMICOLON);
	}
	while(match_and_advance(PROCEDURE)) {
		auto proc_decl = oplist::make_ptr();
		const auto& ident = consume(IDENT);
		consume(SEMICOLON);
		if(ident.has_value()) {
			const auto& ident_str = ident->lexeme.as_string();
			proc_decl = oplist::make_ptr(oplist::t_assignment, ident_str);
			if(context::the().insert(ident_str, context::t_procedure) == context::c_already_declared)
				PROCEDURE_ALREADY_DECLARED(ident_str);
		}
		context::the().level_up();
		proc_decl->set_next(block());
		consume(SEMICOLON);
		context::the().level_down();
		get_last_entry_in_list(current_list_entry)->set_next(proc_decl);
		current_list_entry = proc_decl;
	}

	get_last_entry_in_list(current_list_entry)->set_next(statement());

	return start;
}

oplist::ptr statement() {
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
	return oplist::make_ptr();
}

oplist::ptr assignment() {
	const auto& ident = consume(IDENT);
	if(ident.has_value()) {
		consume(ASSIGNMENT);
		auto a = oplist::make_ptr(oplist::t_assignment, ident->lexeme.as_string());
		CHECK_TYPE(ident->lexeme.as_string(), context::t_var);
		a->set_expr(expression());
		return a;
	}
	return oplist::make_ptr();
}

oplist::ptr call() {
	// TODO context in den parser packen
	// TODO CALL ist dann ein unbedingter sprint auf den BEGIN Knoten der Proc.
	consume(CALL);
	const auto& ident = consume(IDENT);
	if(ident.has_value()) {
		const auto& ident_str = ident->lexeme.as_string();
		CHECK_TYPE(ident_str, context::t_procedure);
		return oplist::make_ptr(oplist::t_call, ident_str);
	}
	return oplist::make_ptr();
}

oplist::ptr get() {
	consume(GET);
	const auto& ident = consume(IDENT);
	if(ident.has_value()) {
		const auto& ident_str = ident->lexeme.as_string();
		CHECK_TYPE(ident_str, context::t_var);
		return oplist::make_ptr(oplist::t_get, ident_str);
	}
	return oplist::make_ptr();
}

oplist::ptr post() {
	consume(POST);
	auto post = oplist::make_ptr(oplist::t_post);
	post->set_expr(expression());
	return post;
}

oplist::ptr begin() {
	consume(BEG);
	auto stmt = statement();
	auto stmt_to_set_next_on = stmt;
	while(match_and_advance(SEMICOLON)) {
		auto next = statement();
		get_last_entry_in_list(stmt_to_set_next_on)->set_next(next);
		stmt_to_set_next_on = next;
	}
	consume(END);
	return stmt;
}

oplist::ptr condition() {
	consume(IF);
	auto if_stmt = oplist::make_ptr(oplist::t_cond_jmp);
	auto nop = oplist::make_ptr(oplist::t_nop);
	if_stmt->set_expr(comparison());
	consume(THEN);
	auto stmt = statement();
	if_stmt->set_next(stmt);
	stmt->set_next(nop);
	if_stmt->set_jmp(nop);
	return if_stmt;
}

oplist::ptr loop() {
	consume(WHILE);
	auto while_loop = oplist::make_ptr(oplist::t_cond_jmp);
	auto jmp = oplist::make_ptr(oplist::t_jmp);
	auto nop = oplist::make_ptr(oplist::t_nop);
	while_loop->set_expr(comparison());
	consume(DO);
	auto stmt = statement();
	while_loop->set_next(stmt);
	jmp->set_jmp(while_loop);
	stmt->set_next(jmp);
	stmt->set_next(nop);
	while_loop->set_jmp(nop);
	return while_loop;
}

expr_tree::ptr comparison() {
	if(match_and_advance(ODD))
		return expr_tree::make_ptr(nullptr, expression(), ODD);
	else {
		auto lhs = expression();
		if(const auto& token = match_and_advance(EQUAL, HASH, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL))
			lhs = expr_tree::make_ptr(lhs, expression(), token.value());
		return lhs;
	}
}

expr_tree::ptr expression() {
	match_and_advance(PLUS, MINUS);
	auto lhs = term();
	while(const auto& token = match_and_advance(PLUS, MINUS))
		lhs = expr_tree::make_ptr(lhs, term(), token.value());

	return lhs;
}

expr_tree::ptr term() {
	auto lhs = factor();
	while(const auto& token = match_and_advance(STAR, SLASH))
		lhs = expr_tree::make_ptr(lhs, factor(), token.value());

	return lhs;
}

expr_tree::ptr factor() {
	expr_tree::ptr expr;
	switch(current_token.type) {
		case IDENT:
			if(const auto& ident = consume(IDENT); ident.has_value()) {
				const auto& ident_str = ident->lexeme.as_string();
				CHECK_TYPE(ident_str, context::t_const | context::t_var);
				return expr_tree::make_ptr(ident_str);
			}
		case NUMBER:
			if(const auto& ident = consume(NUMBER); ident.has_value())
				return expr_tree::make_ptr(ident->lexeme.as_int());
		case PAREN_OPEN:
			consume(PAREN_OPEN);
			expr = expression();
			consume(PAREN_CLOSE);
			return expr;
		default: SET_ERROR("Unexpected Token");
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

inline oplist::ptr get_last_entry_in_list(oplist::ptr list) {
	assert(list != nullptr);
	if(list->get_next() == nullptr)
		return list;
	auto next = list->get_next();
	while(next->get_next() != nullptr)
		next = next->get_next();
	return next;
}