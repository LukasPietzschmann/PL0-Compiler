#include <lexer.h>
#include <stdbool.h>
#include <token_type.h>

enum token_type current_token;
bool has_error;

void set_error(char* message);
enum token_type get_token();

bool match(enum token_type type);

void program();

void block();
void const_declaration();
void var_decalaration();
void procedure();

void statement();
void assignment();
void call();
void get();
void post();
void begin();
void condition();
void loop();

void comparison();
void expression();
void term();
void factor();

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

bool match(enum token_type type){
	if(current_token == type)
		return true;

	set_error("Unexpected token\n");
	return false;
}

