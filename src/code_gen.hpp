#pragma once

#include "context.hpp"
#include "stack.hpp"
#include "stmt_list.hpp"

#define COUT std::cout << "                "

void dump_ram_up();
void dump_ram_down();
void dump_var_addr(int delta, int id);

void gen(stmt_list::ptr stmt);
void gen(expr_tree::ptr expr);