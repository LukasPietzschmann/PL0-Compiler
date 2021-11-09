#include <lexer.h>

int main(){
	int r;
	do{
		r = yylex();
		printf("%d\n", r);
	} while (r != 0);
	return 0;
}