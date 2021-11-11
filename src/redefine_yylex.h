#undef YY_DECL
#undef YY_NULL
#define YY_DECL struct token yylex(void)
#define YY_NULL (struct token){END_OF_FILE, line}