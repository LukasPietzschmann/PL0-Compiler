#pragma once

#include <ostream>
#include <string>
#include <utility>

#include "token_type.hpp"

class expr_tree {
public:
	using ptr = std::shared_ptr<expr_tree>;
	enum expr_type { t_ident, t_value, t_oper };

	explicit expr_tree(int int_val);
	explicit expr_tree(std::string ident);
	expr_tree(expr_tree::ptr lhs, expr_tree::ptr rhs, token_type oper);

	~expr_tree();

	template <typename... Args>
	static expr_tree::ptr make_ptr(Args... args) {
		return std::make_shared<expr_tree>(args...);
	}

	expr_type get_type() const;
	expr_tree::ptr get_lhs() const;
	expr_tree::ptr get_rhs() const;
	token_type get_oper() const;
	const std::string& get_ident() const;
	int get_int_val() const;

	friend std::ostream& operator<<(std::ostream& os, const expr_tree& tree);

private:
	expr_type m_type;
	expr_tree::ptr m_lhs;
	expr_tree::ptr m_rhs;
	union {
		token_type m_oper;
		std::string m_ident;
		int m_int_val;
	};
};

class oplist {
public:
	using ptr = std::shared_ptr<oplist>;
	using entry_type = enum { t_end, t_assignment, t_call, t_get, t_post, t_cond_jmp, t_jmp, t_nop };

	explicit oplist(entry_type type = t_nop, std::string identifier = "");

	template <typename... Args>
	static oplist::ptr make_ptr(Args... args) {
		return std::make_shared<oplist>(args...);
	}

	void set_next(oplist::ptr next);
	void set_jmp(oplist::ptr jmp);
	void set_expr(expr_tree::ptr expr);
	oplist::ptr get_next() const;
	oplist::ptr get_jmp() const;
	expr_tree::ptr get_expr() const;
	entry_type get_type() const;

	friend std::ostream& operator<<(std::ostream& os, const oplist& oplist);

private:
	entry_type m_type;
	std::string m_identifier;
	oplist::ptr m_next{nullptr};
	oplist::ptr m_jmp{nullptr};
	expr_tree::ptr m_expr{nullptr};
};