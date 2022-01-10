#include "oplist.hpp"

expr_tree::expr_tree(int int_val) : m_type(t_value), m_int_val(int_val) {}
expr_tree::expr_tree(std::string ident) : m_type(t_ident), m_ident(std::move(ident)) {}
expr_tree::expr_tree(expr_tree::ptr lhs, expr_tree::ptr rhs, token_type oper) :
	m_type(t_oper), m_lhs(lhs), m_rhs(rhs), m_oper(oper) {}

expr_tree::~expr_tree() {
	switch(m_type) {
		case t_ident:
			using std::string;
			m_ident.~string();
			break;
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
const std::string& expr_tree::get_ident() const { return m_ident; }
int expr_tree::get_int_val() const { return m_int_val; }

std::ostream& operator<<(std::ostream& os, const expr_tree& tree) { return os; }

oplist::oplist(oplist::entry_type type, std::string identifier) : m_type(type), m_identifier(std::move(identifier)) {}

void oplist::set_next(oplist::ptr next) { m_next = next; }
void oplist::set_jmp(oplist::ptr jmp) { m_jmp = jmp; }
void oplist::set_expr(expr_tree::ptr expr) { m_expr = expr; }

oplist::ptr oplist::get_next() const { return m_next; }
oplist::ptr oplist::get_jmp() const { return m_jmp; }
expr_tree::ptr oplist::get_expr() const { return m_expr; }
oplist::entry_type oplist::get_type() const { return m_type; }

std::ostream& operator<<(std::ostream& os, const oplist& oplist) {
	os << oplist.m_type << " " << oplist.m_identifier;
	if(oplist.m_next != nullptr)
		os << " -> " << *oplist.m_next;
	return os;
}