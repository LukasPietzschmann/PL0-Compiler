%{
#include <token_type.h>
%}
%start COMMENT
%%
"(*" BEGIN COMMENT;
"." return DOT;
"CONST" return CONST;
"=" return EQUAL;
"," return COMMA;
";" return SEMICOLON;
"VAR" return VAR;
"PROCEDURE" return PROCEDURE;
":=" return ASSIGNMENT;
"CALL" return CALL;
"?" return GET;
"!" return POST;
"BEGIN" return BEG;
"END" return END;
"IF" return IF;
"THEN" return THEN;
"WHILE" return WHILE;
"DO" return DO;
"ODD" return ODD;
"#" return HASH;
"<" return LESS;
"<=" return LESS_EQUAL;
">" return GREATER;
">=" return GREATER_EQUAL;
"+" return PLUS;
"-" return MINUS;
"*" return STAR;
"/" return SLASH;
"(" return PAREN_OPEN;
")" return PAREN_CLOSE;
[0-9]+ return NUMBER;
[a-zA-Z][0-9a-zA-Z]* return IDENT;
<COMMENT>"*)" BEGIN 0;
<COMMENT>[^"*)"]+ ;
. ;
%%
int yywrap(){
	return 1;
}
