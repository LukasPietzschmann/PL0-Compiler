#pragma once

#include <fstream>
#include <sstream>

#include "context.hpp"
#include "stack.hpp"
#include "stmt_list.hpp"
#include "vm_emitter.hpp"

class code_gen {
public:
	code_gen(const stmt_list::ptr& start, std::ofstream os);

	void dump_calc_new_sl(int delta);
	void dump_var_address(int delta, int id);

	void gen(const stmt_list::ptr& stmt);
	void gen(const expr_tree::ptr& expr);

private:
	vm_emitter m_emitter;

	void dump_init(const stmt_list::ptr& start);
	void dump_ram_up();
	void dump_ram_down();
};