%{
#include "redefine_yylex.hpp"
#include "token.hpp"
#include "token_type.hpp"
unsigned int line = 1;
%}
%start COMMENT
%%
"(*" BEGIN COMMENT;
"." return (token){DOT, line};
"CONST"|"const" return (token){CONST, line};
"=" return (token){EQUAL, line};
"," return (token){COMMA, line};
";" return (token){SEMICOLON, line};
"VAR"|"var" return (token){VAR, line};
"PROCEDURE"|"procedure" return (token){PROCEDURE, line};
":=" return (token){ASSIGNMENT, line};
"CALL"|"call" return (token){CALL, line};
"?" return (token){GET, line};
"!" return (token){POST, line};
"BEGIN"|"begin" return (token){BEG, line};
"END"|"end" return (token){END, line};
"IF"|"if" return (token){IF, line};
"THEN"|"then" return (token){THEN, line};
"WHILE"|"while" return (token){WHILE, line};
"DO"|"do" return (token){DO, line};
"ODD"|"odd" return (token){ODD, line};
"#" return (token){HASH, line};
"<" return (token){LESS, line};
"<=" return (token){LESS_EQUAL, line};
">" return (token){GREATER, line};
">=" return (token){GREATER_EQUAL, line};
"+" return (token){PLUS, line};
"-" return (token){MINUS, line};
"*" return (token){STAR, line};
"/" return (token){SLASH, line};
"(" return (token){PAREN_OPEN, line};
")" return (token){PAREN_CLOSE, line};
[0-9]+ return (token){NUMBER, line};
[a-zA-Z][0-9a-zA-Z]* return (token){IDENT, line};
<COMMENT>"*)" BEGIN 0;
<COMMENT>[^"*)"]+ ;
<COMMENT>"\n" ++line;
"\n" ++line;
[" ""\t"] ;
. return (token){UNDEFINED_CHAR, line};
%%
int yywrap(){
	return 1;
}