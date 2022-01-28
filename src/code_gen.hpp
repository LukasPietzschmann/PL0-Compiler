#pragma once

#include "context.hpp"
#include "stack.hpp"
#include "stmt_list.hpp"

#define COUT os << "\t"

void dump_init(const stmt_list::ptr& start, std::ostream& os);
void dump_ram_up(std::ostream& os);
void dump_ram_down(std::ostream& os);
void dump_calc_new_sl(int delta, std::ostream& os);
void dump_var_address(int delta, int id, std::ostream& os);

void gen(const stmt_list::ptr& stmt, std::ostream& os);
void gen(const expr_tree::ptr& expr, std::ostream& os);