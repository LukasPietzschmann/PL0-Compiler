#pragma once

#include <string>
#include <utility>
#include <ostream>

#include "token_type.hpp"

class expr_tree {
public:
	expr_tree(int int_val);
	expr_tree(std::string ident);
	expr_tree(expr_tree* lhs, expr_tree* rhs, token_type oper);
	~expr_tree();

	friend std::ostream& operator<<(std::ostream& os, const expr_tree& tree);

private:
	expr_tree* m_lhs;
	expr_tree* m_rhs;
	union {
		token_type m_oper;
		std::string m_ident;
		int m_int_val;
	};
};

class oplist {
public:
	using entry_type = enum { t_end, t_assignment, t_call, t_get, t_post, t_cond_jmp, t_jmp, t_nop };

	explicit oplist(entry_type type = t_nop, std::string identifier = "");
	~oplist();

	void set_next(oplist* next);
	void set_jmp(oplist* jmp);
	void set_expr(expr_tree* expr);
	oplist* get_next() const;
	oplist* get_jmp() const;
	expr_tree* get_expr() const;

	friend std::ostream& operator<<(std::ostream& os, const oplist& oplist);

private:
	entry_type m_type;
	std::string m_identifier;
	oplist* m_next{nullptr};
	oplist* m_jmp{nullptr};
	expr_tree* m_expr{nullptr};
};