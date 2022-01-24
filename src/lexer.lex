%{
#include "redefine_yylex.hpp"
#include "token.hpp"
#include "token_type.hpp"
unsigned int line = 1;
%}
%start COMMENT
%%
"(*" BEGIN COMMENT;
"." return (token){DOT, line, yytext};
"CONST"|"const" return (token){CONST, line, yytext};
"=" return (token){EQUAL, line, yytext};
"," return (token){COMMA, line, yytext};
";" return (token){SEMICOLON, line, yytext};
"VAR"|"var" return (token){VAR, line, yytext};
"PROCEDURE"|"procedure" return (token){PROCEDURE, line, yytext};
":=" return (token){ASSIGNMENT, line, yytext};
"CALL"|"call" return (token){CALL, line, yytext};
"?" return (token){GET, line, yytext};
"!" return (token){POST, line, yytext};
"BEGIN"|"begin" return (token){BEG, line, yytext};
"END"|"end" return (token){END, line, yytext};
"IF"|"if" return (token){IF, line, yytext};
"THEN"|"then" return (token){THEN, line, yytext};
"WHILE"|"while" return (token){WHILE, line, yytext};
"DO"|"do" return (token){DO, line, yytext};
"ODD"|"odd" return (token){ODD, line, yytext};
"#" return (token){HASH, line, yytext};
"<" return (token){LESS, line, yytext};
"<=" return (token){LESS_EQUAL, line, yytext};
">" return (token){GREATER, line, yytext};
">=" return (token){GREATER_EQUAL, line, yytext};
"+" return (token){PLUS, line, yytext};
"-" return (token){MINUS, line, yytext};
"*" return (token){STAR, line, yytext};
"/" return (token){SLASH, line, yytext};
"(" return (token){PAREN_OPEN, line, yytext};
")" return (token){PAREN_CLOSE, line, yytext};
[0-9]+ return (token){NUMBER, line, yytext};
[a-zA-Z_][0-9a-zA-Z_]* return (token){IDENT, line, yytext};
<COMMENT>"*)" BEGIN 0;
<COMMENT>[^"*)"]+ ;
<COMMENT>"\n" ++line;
"\n" ++line;
[" ""\t"] ;
. return (token){UNDEFINED_CHAR, line, yytext};
%%
int yywrap(){
	return 1;
}