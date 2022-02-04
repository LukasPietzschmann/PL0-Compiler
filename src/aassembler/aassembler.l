%{
	#include <string>
	#include <map>
	#include <iostream>
	#include "opcodes.hpp"
	using namespace std;
	int tmp, error = 0;

	void emit(int, int = 0);
	FILE * f_bin, * f_json;
	using namespace std;
	map<string,int> label;
%}
%option caseless
int	[0-9]+
wht [ \t]+
label	[A-Za-z0-9_]+
comment [ \t]*(#.*)?
start ^{label}?[ \t]+
%start INT LABEL
%x REST
%%
<REST>.*\n								BEGIN 0;
<INT>{int}/{comment}\n					emit(tmp, atoi(yytext)); BEGIN REST;
<LABEL>{label}/{comment}\n				{
	if (label.find(string(yytext)) == label.end()) {
		error = -1;
		cout << "Undefined label '" << yytext << " " << error << "'\n";
	}
	emit(tmp, label[yytext]); BEGIN REST;
}
{start}add{comment}\n					emit(c_op_add);
{start}sub{comment}\n					emit(c_op_sub);
{start}mult{comment}\n					emit(c_op_mult);
{start}div{comment}\n					emit(c_op_div);
{start}mod{comment}\n					emit(c_op_mod);
{start}chs{comment}\n					emit(c_op_chs);
{start}cmpeq{comment}\n					emit(c_op_cmpeq);
{start}cmpne{comment}\n					emit(c_op_cmpne);
{start}cmple{comment}\n					emit(c_op_cmple);
{start}cmplt{comment}\n					emit(c_op_cmplt);
{start}cmpge{comment}\n					emit(c_op_cmpge);
{start}cmpgt{comment}\n					emit(c_op_cmpgt);
{start}write{comment}\n					emit(c_write);
{start}read{comment}\n					emit(c_read);
{start}inc{wht}/{int}{comment}\n		tmp = c_inc; BEGIN INT;
{start}dec{wht}/{int}{comment}\n		tmp = c_dec; BEGIN INT;
{start}loadc{wht}/{int}{comment}\n		tmp = c_loadc; BEGIN INT;
{start}loadr{wht}/{int}{comment}\n		tmp = c_loadr; BEGIN INT;
{start}storer{wht}/{int}{comment}\n		tmp = c_storer; BEGIN INT;
{start}loads{comment}\n					emit(c_loads);
{start}stores{comment}\n				emit(c_stores);
{start}dup{comment}\n					emit(c_dup);
{start}swap{comment}\n					emit(c_swap);
{start}drop{comment}\n					emit(c_drop);
{start}jump{wht}/{label}{comment}\n		tmp = c_jump; BEGIN LABEL;
{start}jumpz{wht}/{label}{comment}\n	tmp = c_jumpz; BEGIN LABEL;
{start}jumpnz{wht}/{label}{comment}\n	tmp = c_jumpnz; BEGIN LABEL;
{start}call{wht}/{label}{comment}\n		tmp = c_call; BEGIN LABEL;
{start}(nop)?{comment}\n				emit(c_nop, 0);
^{comment}\n							emit(c_nop, 0);
{start}return{comment}\n				emit(c_return);
^.*\n									cout << "KO: '" << yytext << "'\n"; error = -2;
\n										cout << "NL?!?!?!\n"; error = -3;;
%%
int main (int argc, char * argv[]) {
	string filename;
	char line[100], l[20];
	int lnr = 0;

	if (argc != 2) {
		cout << "No asm-file specified" << endl;
		return -4;
	}
	filename = string(argv[argc - 1]);
	if (filename.length() > 4  && filename.substr(filename.length() - 4) == ".asm")
		filename = filename.substr(0, filename.length() - 4);

	if ((yyin = fopen((filename + ".asm").c_str(), "r")) == NULL) {
		cout << "Unable to open '" << filename << "'" << endl;
		return -4;
	}

	while (fgets(line, 100, yyin) != NULL) {
		if (line[0] > ' ') {
			sscanf(line, "%s", l);
			if (label.find(string(l)) == label.end())
				label[l] = lnr;
			else {
				cout << "Duplicate label '" << l << "' at lnr " << lnr << endl;
				error = -1;
			}
		}
		lnr++;
	}
	if (error)
		return error;

	rewind(yyin);
	f_bin = fopen((filename + ".bin").c_str(), "w");
	f_json = fopen((filename + ".json").c_str(), "w");
	fprintf(f_json,"[");
	yylex();
	fprintf(f_json,"\n]\n");
	fclose(f_bin);
	fclose(f_json);
	return error;
}

void emit(int c, int a) {
	static int lnr;
	#ifdef AASSEMBLER_OUTPUT
		printf("%03d %4d %4d\n", lnr, c, a);
	#endif
	fprintf(f_json, "%s[\"%s\",%d]", lnr?",\n\t":"\n\t", opct[c], a);
	fwrite(&c, sizeof(c), 1, f_bin);
	fwrite(&a, sizeof(a), 1, f_bin);
	lnr++;
}
int yywrap() {
	return 1;
}