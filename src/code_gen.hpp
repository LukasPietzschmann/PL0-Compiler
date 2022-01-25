#pragma once

#include "context.hpp"
#include "stack.hpp"
#include "stmt_list.hpp"

#define COUT os << "\t"

void dump_init(stmt_list::ptr start, std::ostream& os);
void dump_ram_up(std::ostream& os);
void dump_ram_down(std::ostream& os);
void dump_var_address(int delta, int id, std::ostream& os);

void gen(stmt_list::ptr stmt, std::ostream& os);
void gen(expr_tree::ptr expr, std::ostream& os);