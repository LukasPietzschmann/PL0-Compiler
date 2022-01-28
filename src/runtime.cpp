#include "runtime.hpp"

#include "stack.hpp"

bool operator==(const expr_result& lhs, const expr_result& rhs) { return lhs.num_value == rhs.num_value; }
bool operator!=(const expr_result& lhs, const expr_result& rhs) { return lhs.num_value != rhs.num_value; }
bool operator<(const expr_result& lhs, const expr_result& rhs) { return lhs.num_value < rhs.num_value; }
bool operator<=(const expr_result& lhs, const expr_result& rhs) { return lhs.num_value <= rhs.num_value; }
bool operator>(const expr_result& lhs, const expr_result& rhs) { return lhs.num_value > rhs.num_value; }
bool operator>=(const expr_result& lhs, const expr_result& rhs) { return lhs.num_value >= rhs.num_value; }
int operator+(const expr_result& lhs, const expr_result& rhs) { return lhs.num_value + rhs.num_value; }
int operator-(const expr_result& lhs, const expr_result& rhs) { return lhs.num_value - rhs.num_value; }
int operator*(const expr_result& lhs, const expr_result& rhs) { return lhs.num_value * rhs.num_value; }
int operator/(const expr_result& lhs, const expr_result& rhs) { return lhs.num_value / rhs.num_value; }
int operator%(const expr_result& lhs, int rhs) { return lhs.num_value / rhs; }
std::ostream& operator<<(std::ostream& os, const expr_result& res) { return os << res.num_value; }

void exec(const stmt_list::ptr& list) {
	assert(list != nullptr);
	const auto& set_ident_to_new_value = [&list](int new_value) {
		int sf = stack::the().walk_sl_chain(list->get_position().delta);
		stack::the().set_at(sf - (list->get_position().id + 2), new_value);
	};

	const context::proc_table_entry* proc_info;
	switch(list->get_type()) {
		case stmt_list::t_assignment: set_ident_to_new_value(exec(list->get_expr()).num_value); break;
		case stmt_list::t_call:
			proc_info = &context::the().lookup_procedure(list->get_position().id);
			stack::the().push_sf(proc_info->number_of_variables, list->get_position().delta);
			exec(proc_info->procedure);
			break;
		case stmt_list::t_get:
			int new_val;
			std::cin >> new_val;
			set_ident_to_new_value(new_val);
			break;
		case stmt_list::t_post: std::cout << exec(list->get_expr()) << std::endl; break;
		case stmt_list::t_cond_jmp:
			if(const expr_result& cond = exec(list->get_expr()); !cond.bool_value) {
				exec(list->get_jmp());
				return;
			}
			break;
		case stmt_list::t_jmp: exec(list->get_jmp()); break;
		case stmt_list::t_end: stack::the().pop_sf(); break;
		case stmt_list::t_nop: break;
	}
	if(list->get_type() != stmt_list::t_jmp && list->get_next() != nullptr)
		exec(list->get_next());
}

expr_result exec(const expr_tree::ptr& expr) {
	assert(expr != nullptr);
	if(expr->get_type() == expr_tree::t_value)
		return {expr->get_int_val()};
	if(expr->get_type() == expr_tree::t_ident) {
		int sl = stack::the().walk_sl_chain(expr->get_position().delta);
		return {stack::the().get_at(sl - (expr->get_position().id + 2))};
	}

	std::optional<expr_result> lhs;
	if(expr->get_lhs() == nullptr)
		lhs = {};
	else
		lhs = exec(expr->get_lhs());
	const auto& rhs = exec(expr->get_rhs());

	switch(expr->get_oper()) {
		case EQUAL: return {*lhs == rhs};
		case HASH: return {*lhs != rhs};
		case LESS: return {*lhs < rhs};
		case LESS_EQUAL: return {*lhs <= rhs};
		case GREATER: return {*lhs > rhs};
		case GREATER_EQUAL: return {*lhs >= rhs};
		case PLUS: return {*lhs + rhs};
		case MINUS: return {*lhs - rhs};
		case STAR: return {*lhs * rhs};
		case SLASH: return {*lhs / rhs};
		case ODD: return {rhs % 2 == 0};
		default: assert(false);
	}
}