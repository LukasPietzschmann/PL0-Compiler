#include <lexer.h>
#include <stdbool.h>
#include <token_type.h>

enum token_type current_token;
bool has_error;

void set_error(char* message);
enum token_type get_token();

int main() {
	int r;
	do {
		r = get_token();
		printf("%d\n", r);
	} while(r != 0);
	return 0;
}

void set_error(char* message) {
	has_error = true;
	printf("%s\n", message);
}

enum token_type get_token() { return yylex(); }