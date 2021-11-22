#include <redefine_yylex.h>
#include <lexer.h>
#include <parser.h>
#include <stdio.h>

int main(int argc, char** argv) {
	if(argc > 1) {
		FILE* input = fopen(*(argv + 1), "r");
		if(!input)
			return -1;
		yyin = input;
	}
	return parse();
}