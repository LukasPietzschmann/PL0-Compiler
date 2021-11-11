%{
#include <token.h>
unsigned int line = 1;

#include <redefine_yylex.h>
%}
%start COMMENT
%%
"(*" BEGIN COMMENT;
"." return (struct token){DOT, line};
"CONST" return (struct token){CONST, line};
"=" return (struct token){EQUAL, line};
"," return (struct token){COMMA, line};
";" return (struct token){SEMICOLON, line};
"VAR" return (struct token){VAR, line};
"PROCEDURE" return (struct token){PROCEDURE, line};
":=" return (struct token){ASSIGNMENT, line};
"CALL" return (struct token){CALL, line};
"?" return (struct token){GET, line};
"!" return (struct token){POST, line};
"BEGIN" return (struct token){BEG, line};
"END" return (struct token){END, line};
"IF" return (struct token){IF, line};
"THEN" return (struct token){THEN, line};
"WHILE" return (struct token){WHILE, line};
"DO" return (struct token){DO, line};
"ODD" return (struct token){ODD, line};
"#" return (struct token){HASH, line};
"<" return (struct token){LESS, line};
"<=" return (struct token){LESS_EQUAL, line};
">" return (struct token){GREATER, line};
">=" return (struct token){GREATER_EQUAL, line};
"+" return (struct token){PLUS, line};
"-" return (struct token){MINUS, line};
"*" return (struct token){STAR, line};
"/" return (struct token){SLASH, line};
"(" return (struct token){PAREN_OPEN, line};
")" return (struct token){PAREN_CLOSE, line};
[0-9]+ return (struct token){NUMBER, line};
[a-zA-Z][0-9a-zA-Z]* return (struct token){IDENT, line};
<COMMENT>"*)" BEGIN 0;
<COMMENT>[^"*)"]+ ;
<COMMENT>"\n" ++line;
"\n" ++line;
. ;
%%
int yywrap(){
	return 1;
}
