#include "context.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "optimization.hpp"
#include "runtime.hpp"
#include "stack.hpp"

int main(int argc, char** argv) {
	if(argc > 1) {
		std::string raw_input_file_name(*(argv + 1));
		std::string adjusted_input_file_name;
		if(!raw_input_file_name.ends_with(".pl0"))
			adjusted_input_file_name = raw_input_file_name + ".pl0";
		FILE* input_file = fopen(adjusted_input_file_name.c_str(), "r");
		if(!input_file)
			return -1;
		yyin = input_file;
	}
	oplist::ptr pars_res = parse();
	pars_res = optimize_nops(pars_res);

	if(pars_res == nullptr)
		return 1;

	stack::the().push_sf(context::the().lookup_procedure(0).number_of_variables, 0);
	exec(pars_res);

	return 0;
}