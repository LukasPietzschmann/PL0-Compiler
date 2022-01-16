#include "oplist.hpp"

expr_tree::expr_tree(int int_val) : m_type(t_value), m_int_val(int_val) {}
expr_tree::expr_tree(int delta, int id) : m_type(t_ident), m_pos_in_context({delta, id}){}
expr_tree::expr_tree(expr_tree::ptr lhs, expr_tree::ptr rhs, token_type oper) :
	m_type(t_oper), m_lhs(lhs), m_rhs(rhs), m_oper(oper) {}

expr_tree::~expr_tree() {
	switch(m_type) {
		case t_ident: m_pos_in_context.~context_position(); break;
		case t_oper:
			using std::shared_ptr;
			m_lhs.~shared_ptr();
			m_rhs.~shared_ptr();
			m_oper.~token_type();
			break;
		case t_value: break;
	}
}

expr_tree::expr_type expr_tree::get_type() const { return m_type; }
expr_tree::ptr expr_tree::get_lhs() const { return m_lhs; }
expr_tree::ptr expr_tree::get_rhs() const { return m_rhs; }
token_type expr_tree::get_oper() const { return m_oper; }
const context::context_position& expr_tree::get_position() const { return m_pos_in_context; }
int expr_tree::get_int_val() const { return m_int_val; }

std::ostream& operator<<(std::ostream& os, const expr_tree& tree) { return os; }

oplist::oplist(oplist::entry_type type, int delta, int id) :
	m_type(type), m_pos_in_context({delta, id}) {}

void oplist::set_next(oplist::ptr next) { m_next = next; }
void oplist::set_jmp(oplist::ptr jmp) { m_jmp = jmp; }
void oplist::set_expr(expr_tree::ptr expr) { m_expr = expr; }

oplist::ptr oplist::get_next() const { return m_next; }
oplist::ptr oplist::get_jmp() const { return m_jmp; }
expr_tree::ptr oplist::get_expr() const { return m_expr; }
oplist::entry_type oplist::get_type() const { return m_type; }
const context::context_position& oplist::get_position() const { return m_pos_in_context; }

std::ostream& operator<<(std::ostream& os, const oplist& oplist) {
	os << oplist.m_type << " " << oplist.m_pos_in_context.delta << ":" << oplist.m_pos_in_context.id;
	if(oplist.m_next != nullptr)
		os << " -> " << *oplist.m_next;
	return os;
}