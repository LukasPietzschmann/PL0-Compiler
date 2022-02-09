#include "code_gen.hpp"

code_gen::code_gen(const stmt_list::ptr& start, std::ofstream os) : m_emitter(std::forward<std::ofstream>(os)) {
	dump_init(start);
	dump_ram_up();
	dump_ram_down();
}

void code_gen::dump_init(const stmt_list::ptr& start) {
	m_emitter.emit(emitter::t_loadc, context::the().lookup_procedure(0).number_of_variables + 2, {}, {}, "Init Start");
	m_emitter.emit(emitter::t_storer, 0);
	m_emitter.emit(emitter::t_loadc, 0);
	std::stringstream ss;
	ss << start.get();
	m_emitter.emit(emitter::t_jmp, {}, ss.str(), {}, "Init End");
}

/**
 * New static link below of number of variables on stack
 */
void code_gen::dump_ram_up() {
	m_emitter.emit(emitter::t_nop, {}, {}, "ram_up");

	m_emitter.emit(emitter::t_loadr, 0);
	m_emitter.emit(emitter::t_add);
	m_emitter.emit(emitter::t_inc, 2);
	m_emitter.emit(emitter::t_dup);
	m_emitter.emit(emitter::t_dec, 1);
	m_emitter.emit(emitter::t_loadr, 0);
	m_emitter.emit(emitter::t_swap);
	m_emitter.emit(emitter::t_stores);
	m_emitter.emit(emitter::t_dup);
	m_emitter.emit(emitter::t_storer, 0);
	m_emitter.emit(emitter::t_stores);
	m_emitter.emit(emitter::t_return);
}

void code_gen::dump_ram_down() {
	m_emitter.emit(emitter::t_nop, {}, {}, "ram_down");

	m_emitter.emit(emitter::t_loadr, 0);
	m_emitter.emit(emitter::t_dec, 1);
	m_emitter.emit(emitter::t_loads);
	m_emitter.emit(emitter::t_storer, 0);
	m_emitter.emit(emitter::t_return);
}

void code_gen::dump_calc_new_sl(int delta) {
	m_emitter.emit(emitter::t_loadr, 0, {}, {}, "Calc new SL begin");
	for(int i = 0; i < delta; ++i)
		m_emitter.emit(emitter::t_loads);
	m_emitter.emit(emitter::t_nop, {}, {}, {}, "Calc new SL end");
}

/**
 * Read: Call `loads` afterwards
 * Write: Put new value on stack before and call `stores` afterwards
 */
void code_gen::dump_var_address(int delta, int id) {
	m_emitter.emit(emitter::t_nop, {}, {}, {}, "Get var address begin");
	dump_calc_new_sl(delta);
	m_emitter.emit(emitter::t_dec, 2 + id, {}, {}, "Get var address end");
}

void code_gen::gen(const stmt_list::ptr& stmt) {
	assert(stmt != nullptr);

	std::stringstream ss;
	ss << stmt.get();
	m_emitter.emit(emitter::t_nop, {}, {}, ss.str());
	ss.str("");

	switch(stmt->get_type()) {
		case stmt_list::t_assignment:
			gen(stmt->get_expr());
			code_gen::dump_var_address(stmt->get_position().delta, stmt->get_position().id);
			m_emitter.emit(emitter::t_stores);
			break;
		case stmt_list::t_call:
			dump_calc_new_sl(stmt->get_position().delta);
			m_emitter.emit(emitter::t_loadc,
					context::the().lookup_procedure(stmt->get_position().id).number_of_variables);
			m_emitter.emit(emitter::t_call, {}, "ram_up");
			ss << context::the().lookup_procedure(stmt->get_position().id).procedure.get();
			m_emitter.emit(emitter::t_call, {}, ss.str());
			m_emitter.emit(emitter::t_call, {}, "ram_down");
			break;
		case stmt_list::t_get:
			m_emitter.emit(emitter::t_read);
			code_gen::dump_var_address(stmt->get_position().delta, stmt->get_position().id);
			m_emitter.emit(emitter::t_stores);
			break;
		case stmt_list::t_post:
			gen(stmt->get_expr());
			m_emitter.emit(emitter::t_write);
			break;
		case stmt_list::t_cond_jmp:
			gen(stmt->get_expr());
			ss << stmt->get_jmp().get();
			m_emitter.emit(emitter::t_jmpz, {}, ss.str());
			break;
		case stmt_list::t_jmp:
			ss << stmt->get_jmp().get();
			m_emitter.emit(emitter::t_jmp, {}, ss.str());
			break;
		case stmt_list::t_end: m_emitter.emit(emitter::t_return); break;
		case stmt_list::t_nop: break;
	}
	if(stmt_list::ptr next = stmt->get_next(); next != nullptr)
		gen(next);
}

void code_gen::gen(const expr_tree::ptr& expr) {
	assert(expr != nullptr);
	if(expr->get_type() == expr_tree::t_value) {
		m_emitter.emit(emitter::t_loadc, expr->get_int_val());
		return;
	}
	if(expr->get_type() == expr_tree::t_ident) {
		code_gen::dump_var_address(expr->get_position().delta, expr->get_position().id);
		m_emitter.emit(emitter::t_loads);
		return;
	}

	if(expr_tree::ptr lhs = expr->get_lhs(); lhs != nullptr)
		gen(lhs);
	gen(expr->get_rhs());
	switch(expr->get_oper()) {
		case EQUAL: m_emitter.emit(emitter::t_cmpeq); break;
		case HASH: m_emitter.emit(emitter::t_cmpne); break;
		case LESS: m_emitter.emit(emitter::t_cmplt); break;
		case LESS_EQUAL: m_emitter.emit(emitter::t_cmple); break;
		case GREATER: m_emitter.emit(emitter::t_cmpgt); break;
		case GREATER_EQUAL: m_emitter.emit(emitter::t_cmpge); break;
		case PLUS: m_emitter.emit(emitter::t_add); break;
		case MINUS: m_emitter.emit(emitter::t_sub); break;
		case STAR: m_emitter.emit(emitter::t_mult); break;
		case SLASH: m_emitter.emit(emitter::t_div); break;
		case ODD:
			m_emitter.emit(emitter::t_loadc, 2);
			m_emitter.emit(emitter::t_mod);
			break;
		default: assert(false);
	}
}