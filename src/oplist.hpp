#pragma once

#include <ostream>
#include <string>
#include <utility>

#include "token_type.hpp"

class expr_tree {
public:
	enum expr_type { t_ident, t_value, t_oper };

	expr_tree(int int_val);
	expr_tree(std::string ident);
	expr_tree(expr_tree* lhs, expr_tree* rhs, token_type oper);
	~expr_tree();

	expr_type get_type() const;
	expr_tree* get_lhs() const;
	expr_tree* get_rhs() const;
	token_type get_oper() const;
	const std::string& get_ident() const;
	int get_int_val() const;

	friend std::ostream& operator<<(std::ostream& os, const expr_tree& tree);

private:
	expr_type m_type;
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
	entry_type get_type() const;

	friend std::ostream& operator<<(std::ostream& os, const oplist& oplist);

private:
	entry_type m_type;
	std::string m_identifier;
	oplist* m_next{nullptr};
	oplist* m_jmp{nullptr};
	expr_tree* m_expr{nullptr};
};