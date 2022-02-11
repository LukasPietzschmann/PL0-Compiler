#include "context.hpp"
// clang-format off
#include "parser.hpp"
#include "lexer.hpp"
// clang-format on
#include "optimization.hpp"
#ifdef BUILD_COMPILER
#include <fstream>

#include "code_gen.hpp"
#else
#include "runtime.hpp"
#include "stack.hpp"
#endif

int main(int argc, char** argv) {
	if(argc <= 1)
		return 1;

	std::string raw_input_file_name(*(argv + 1));
	std::string adjusted_input_file_name;
	if(!raw_input_file_name.ends_with(".pl0"))
		adjusted_input_file_name = raw_input_file_name + ".pl0";
	FILE* input_file = fopen(adjusted_input_file_name.c_str(), "r");
	if(!input_file)
		return 1;
	yyin = input_file;

	stmt_list::ptr pars_res = parse();

	if(pars_res == nullptr)
		return 1;

	optimize_nops(pars_res);
#ifndef BUILD_COMPILER
	stack::the().push_sf(context::the().lookup_procedure(0).number_of_variables, 0);
	exec(pars_res);
#else
	std::ofstream asm_out(raw_input_file_name + ".asm");
	code_gen cg(pars_res, std::move(asm_out));
	for(unsigned long i = 1; i < context::the().get_proc_count(); ++i)
		cg.gen(context::the().lookup_procedure(i).procedure);
	cg.gen(pars_res);
#endif

	return 0;
}