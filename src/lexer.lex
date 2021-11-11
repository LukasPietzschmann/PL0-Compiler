%{
#include <token.h>
unsigned int line = 1;

#include <redefine_yylex.h>
%}
%start COMMENT
%%
"(*" BEGIN COMMENT;
"." return (token){DOT, line};
"CONST" return (token){CONST, line};
"=" return (token){EQUAL, line};
"," return (token){COMMA, line};
";" return (token){SEMICOLON, line};
"VAR" return (token){VAR, line};
"PROCEDURE" return (token){PROCEDURE, line};
":=" return (token){ASSIGNMENT, line};
"CALL" return (token){CALL, line};
"?" return (token){GET, line};
"!" return (token){POST, line};
"BEGIN" return (token){BEG, line};
"END" return (token){END, line};
"IF" return (token){IF, line};
"THEN" return (token){THEN, line};
"WHILE" return (token){WHILE, line};
"DO" return (token){DO, line};
"ODD" return (token){ODD, line};
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
. ;
%%
int yywrap(){
	return 1;
}
