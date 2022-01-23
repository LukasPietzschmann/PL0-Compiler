#pragma once

#include <ostream>
#include <utility>

#include "context.hpp"
#include "token_type.hpp"

class expr_tree {
public:
	using ptr = std::shared_ptr<expr_tree>;
	enum expr_type { t_ident, t_value, t_oper };

	explicit expr_tree(int int_val);
	expr_tree(int delta, int id);
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
	const context::context_position& get_position() const;
	int get_int_val() const;

	friend std::ostream& operator<<(std::ostream& os, const expr_tree& tree);

private:
	expr_type m_type;
	expr_tree::ptr m_lhs;
	expr_tree::ptr m_rhs;
	union {
		token_type m_oper;
		context::context_position m_pos_in_context;
		int m_int_val;
	};
};

class stmt_list {
public:
	using ptr = std::shared_ptr<stmt_list>;
	using entry_type = enum { t_end, t_assignment, t_call, t_get, t_post, t_cond_jmp, t_jmp, t_nop };

	explicit stmt_list(entry_type type = t_nop, int delta = -1, int id = -1);

	template <typename... Args>
	static stmt_list::ptr make_ptr(Args... args) {
		return std::make_shared<stmt_list>(args...);
	}

	void set_next(stmt_list::ptr next);
	void set_jmp(stmt_list::ptr jmp);
	void set_expr(expr_tree::ptr expr);
	stmt_list::ptr get_next() const;
	stmt_list::ptr get_jmp() const;
	expr_tree::ptr get_expr() const;
	entry_type get_type() const;
	const context::context_position& get_position() const;

	friend std::ostream& operator<<(std::ostream& os, const stmt_list& oplist);

private:
	entry_type m_type;
	context::context_position m_pos_in_context;
	stmt_list::ptr m_next{nullptr};
	stmt_list::ptr m_jmp{nullptr};
	expr_tree::ptr m_expr{nullptr};
};