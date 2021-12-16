#include "token.hpp"
#undef YY_DECL
#undef YY_NULL
#define YY_DECL token yylex(void)
#define YY_NULL (token){END_OF_FILE, line, yytext}