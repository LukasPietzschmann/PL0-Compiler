%{
enum TOKEN{
	DOT=1, CONST=2, IDENT=3, EQUAL=4, NUMBER=5,
	COMMA=6, SEMICOLON=7, VAR=8, PROCEDURE=9, ASSIGNMENT=10,
	CALL=11, GET=12, POST=13, BEG=14, END=15, IF=16,
	THEN=17, WHILE=18, DO=19, ODD=20, HASH=21, LESS=22,
	LESS_EQUAL=23, GREATER=24, GREATER_EQUAL=25, PLUS=26,
	MINUS=27, STAR=28, SLASH=29, PAREN_OPEN=30, PAREN_CLOSE=31
};
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
int main(){
	int r;
	do{
		r = yylex();
		printf("%d\n", r);
	}
	while(r != 0);
	return 0;
}

int yywrap(){
	return 1;
}
