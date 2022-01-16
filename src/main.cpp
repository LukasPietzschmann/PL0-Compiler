#include "context.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "runtime.hpp"
#include "stack.hpp"

int main(int argc, char** argv) {
	if(argc > 1) {
		FILE* input = fopen(*(argv + 1), "r");
		if(!input)
			return -1;
		yyin = input;
	}
	const auto& pars_res = parse();

	if(pars_res == nullptr)
		return 1;

	stack::the().push_sf(context::the().lookup_procedure(0).number_of_variables, 0);
	exec(pars_res);

	return 0;
}