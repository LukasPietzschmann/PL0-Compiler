#include "code_gen.hpp"

void dump_init(stmt_list::ptr start, std::ostream& os) {
	COUT << "loadc " << context::the().lookup_procedure(0).number_of_variables + 2 << " # Init Start" << std::endl;
	COUT << "storer 0" << std::endl;
	COUT << "loadc 0" << std::endl;
	COUT << "jump " << start.get() << " # Init End" << std::endl;
}

/**
 * Number of variables on stack
 */
void dump_ram_up(std::ostream& os) {
	os << "ram_up	nop" << std::endl;

	COUT << "loadr 0" << std::endl;
	COUT << "add" << std::endl;
	COUT << "inc 2" << std::endl;
	COUT << "dup" << std::endl;
	COUT << "dec 1" << std::endl;
	COUT << "loadr 0" << std::endl;
	COUT << "swap" << std::endl;
	COUT << "stores" << std::endl;
	COUT << "dup" << std::endl;
	COUT << "storer 0" << std::endl;
	COUT << "stores" << std::endl;
	COUT << "return" << std::endl;
}

void dump_ram_down(std::ostream& os) {
	os << "ram_down	nop" << std::endl;

	COUT << "loadr 0" << std::endl;
	COUT << "dec 1" << std::endl;
	COUT << "loads" << std::endl;
	COUT << "storer 0" << std::endl;
	COUT << "return" << std::endl;
}

/**
 * Read: Call `loads` afterwards
 * Write: Put new value on stack before and call `stores` afterwards
 */
void dump_var_address(int delta, int id, std::ostream& os) {
	COUT << "loadr 0 # Get Var Address Begin" << std::endl;
	for(int i = 0; i < delta; ++i)
		COUT << "loads" << std::endl;
	COUT << "dec " << 2 + id << " # Get Var Address End" << std::endl;
}

void gen(stmt_list::ptr stmt, std::ostream& os) {
	assert(stmt != nullptr);

	os << stmt.get() << "	nop" << std::endl;

	switch(stmt->get_type()) {
		case stmt_list::t_assignment:
			gen(stmt->get_expr(), os);
			dump_var_address(stmt->get_position().delta, stmt->get_position().id, os);
			COUT << "stores" << std::endl;
			break;
		case stmt_list::t_call:
			COUT << "loadc " << context::the().lookup_procedure(stmt->get_position().id).number_of_variables
				 << std::endl;
			COUT << "call ram_up" << std::endl;
			COUT << "call " << context::the().lookup_procedure(stmt->get_position().id).procedure.get() << std::endl;
			COUT << "call ram_down" << std::endl;
			break;
		case stmt_list::t_get:
			COUT << "read" << std::endl;
			dump_var_address(stmt->get_position().delta, stmt->get_position().id, os);
			COUT << "stores" << std::endl;
			break;
		case stmt_list::t_post:
			gen(stmt->get_expr(), os);
			COUT << "write" << std::endl;
			break;
		case stmt_list::t_cond_jmp:
			gen(stmt->get_expr(), os);
			COUT << "jumpz " << stmt->get_jmp().get() << std::endl;
			break;
		case stmt_list::t_jmp: COUT << "jump " << stmt->get_jmp().get() << std::endl; break;
		case stmt_list::t_end: COUT << "return" << std::endl; break;
		case stmt_list::t_nop: COUT << "nop" << std::endl; break;
	}
	if(stmt_list::ptr next = stmt->get_next(); next != nullptr && stmt->get_type() != stmt_list::t_jmp)
		gen(next, os);
}

void gen(expr_tree::ptr expr, std::ostream& os) {
	assert(expr != nullptr);
	if(expr->get_type() == expr_tree::t_value) {
		COUT << "loadc " << expr->get_int_val() << std::endl;
		return;
	}
	if(expr->get_type() == expr_tree::t_ident) {
		dump_var_address(expr->get_position().delta, expr->get_position().id, os);
		COUT << "loads" << std::endl;
		return;
	}

	if(expr_tree::ptr lhs = expr->get_lhs(); lhs != nullptr)
		gen(lhs, os);
	gen(expr->get_rhs(), os);
	switch(expr->get_oper()) {
		case EQUAL: COUT << "cmpeq" << std::endl; break;
		case HASH: COUT << "cmpne" << std::endl; break;
		case LESS: COUT << "cmplt" << std::endl; break;
		case LESS_EQUAL: COUT << "cmple" << std::endl; break;
		case GREATER: COUT << "cmpgt" << std::endl; break;
		case GREATER_EQUAL: COUT << "cmpge" << std::endl; break;
		case PLUS: COUT << "add" << std::endl; break;
		case MINUS: COUT << "sub" << std::endl; break;
		case STAR: COUT << "mult" << std::endl; break;
		case SLASH: COUT << "div" << std::endl; break;
		case ODD:
			COUT << "loadc 2" << std::endl;
			COUT << "swap" << std::endl;
			COUT << "mod" << std::endl;
			break;
		default: assert(false);
	}
}