#include "parser.hpp"
#include "lexer.hpp"

int main(int argc, char** argv) {
	if(argc > 1) {
		FILE* input = fopen(*(argv + 1), "r");
		if(!input)
			return -1;
		yyin = input;
	}
	return parse();
}