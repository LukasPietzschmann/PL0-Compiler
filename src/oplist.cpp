#include "oplist.hpp"

expr_tree::expr_tree(int int_val) : m_type(t_value), m_int_val(int_val) {}
expr_tree::expr_tree(std::string ident) : m_type(t_ident), m_ident(std::move(ident)) {}
expr_tree::expr_tree(expr_tree* lhs, expr_tree* rhs, token_type oper) :
	m_type(t_oper), m_lhs(lhs), m_rhs(rhs), m_oper(oper) {}

expr_tree::~expr_tree() {
	delete m_rhs;
	delete m_lhs;
}

expr_tree::expr_type expr_tree::get_type() const { return m_type; }
expr_tree* expr_tree::get_lhs() const { return m_lhs; }
expr_tree* expr_tree::get_rhs() const { return m_rhs; }
token_type expr_tree::get_oper() const { return m_oper; }
const std::string& expr_tree::get_ident() const { return m_ident; }
int expr_tree::get_int_val() const { return m_int_val; }

std::ostream& operator<<(std::ostream& os, const expr_tree& tree) { return os; }

oplist::oplist(oplist::entry_type type, std::string identifier) : m_type(type), m_identifier(std::move(identifier)) {}

oplist::~oplist() {
	delete m_next;
	delete m_jmp;
	delete m_expr;
}

void oplist::set_next(oplist* next) { m_next = next; }
void oplist::set_jmp(oplist* jmp) { m_jmp = jmp; }
void oplist::set_expr(expr_tree* expr) { m_expr = expr; }

oplist* oplist::get_next() const { return m_next; }
oplist* oplist::get_jmp() const { return m_jmp; }
expr_tree* oplist::get_expr() const { return m_expr; }
oplist::entry_type oplist::get_type() const { return m_type; }

std::ostream& operator<<(std::ostream& os, const oplist& oplist) {
	os << oplist.m_type << " " << oplist.m_identifier;
	if(oplist.m_next != nullptr)
		os << " -> " << *oplist.m_next;
	return os;
}