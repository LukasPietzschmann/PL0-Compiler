#include <getopt.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "opcodes.hpp"
#include "stack.hpp"

void debug();

#define max 256
int ram[max], d_flag = 0, pc = 0, cmd[2];
char txt[256];
stack ops = {{0}, 0}, pcs = {{0}, 0};

int main(int argc, char* argv[]) {
	char filename[100];
	int rc, opt;
	int op_li, op_re;
	FILE* f;

	while((opt = getopt(argc, argv, "d:")) != -1) {
		switch(opt) {
			case 'd': d_flag = atoi(optarg); break;
			default: fprintf(stderr, "Usage: %s [-d debug-level] name\n", argv[0]); return -1;
		}
	}

	if(optind >= argc) {
		fprintf(stderr, "Expected argument after options\n");
		return -2;
	}

	strcpy(filename, argv[argc - 1]);
	if(strlen(filename) < 4 || strcmp(filename + strlen(filename) - 4, ".bin"))
		strcat(filename, ".bin");
	if((f = fopen(filename, "r")) == NULL) {
		printf("Unable to open %s\n", filename);
		return -1;
	}

	while(fseek(f, 2 * pc * sizeof(int), SEEK_SET) == 0 && fread(cmd, sizeof(int), 2, f) == 2) {
		if(d_flag > 0)
			printf("%03d: %-10s %4d\n", pc, opct[cmd[0]], cmd[1]);
		switch(cmd[0]) {
			case c_loadc: push(&ops, cmd[1]); break;
			case c_inc: pop(&ops, &op_re), push(&ops, op_re + cmd[1]); break;
			case c_dec: pop(&ops, &op_re), push(&ops, op_re - cmd[1]); break;
			case c_op_add: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li + op_re); break;
			case c_op_sub: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li - op_re); break;
			case c_op_mult: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li * op_re); break;
			case c_op_div: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li / op_re); break;
			case c_op_mod: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li % op_re); break;
			case c_op_cmpeq: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li == op_re); break;
			case c_op_cmpne: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li != op_re); break;
			case c_op_cmpgt: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li > op_re); break;
			case c_op_cmpge: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li >= op_re); break;
			case c_op_cmplt: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li < op_re); break;
			case c_op_cmple: pop(&ops, &op_re), pop(&ops, &op_li), push(&ops, op_li <= op_re); break;
			case c_op_chs: pop(&ops, &op_re), push(&ops, -op_re); break;
			case c_write:
				pop(&ops, &op_re);
				printf("Ausgabe: %d\n", op_re);
				break;
			case c_read:
				printf("Eingabe: ");
				scanf("%d", &op_re);
				push(&ops, op_re);
				fgetc(stdin);  // Wegen debug-fgetc
				break;
			case c_jump:  // Bei allen jump-Befehlen:
				pc = cmd[1] - 1;  // Das -1 wegen "pc++" am Ende der Schleife
				break;
			case c_jumpnz:
				pop(&ops, &op_re);
				if(op_re != 0)
					pc = cmd[1] - 1;
				break;
			case c_jumpz:
				pop(&ops, &op_re);
				if(op_re == 0)
					pc = cmd[1] - 1;
				break;
			case c_loadr: push(&ops, ram[cmd[1]]); break;
			case c_storer:
				pop(&ops, &op_re);
				ram[cmd[1]] = op_re;
				break;
			case c_nop: break;
			case c_call:
				push(&pcs, pc), pc = cmd[1] - 1;  // -1 wegen pc++
				break;
			case c_return:
				if(pop(&pcs, &pc))	// programcounter-stack leer?
					pc = -10;  // Raus hier! exit statt return!
				break;
			case c_loads:
				pop(&ops, &op_re);
				push(&ops, ram[op_re]);
				break;
			case c_stores:
				pop(&ops, &op_re);
				pop(&ops, &op_li);
				ram[op_re] = op_li;
				break;
			case c_dup:
				pop(&ops, &op_re);
				push(&ops, op_re);
				push(&ops, op_re);
				break;
			case c_swap:
				pop(&ops, &op_re);
				pop(&ops, &op_li);
				push(&ops, op_re);
				push(&ops, op_li);
				break;
			case c_drop: pop(&ops, &op_re); break;
			default: printf("Error 0 pc=%d %4d %4d", pc, cmd[0], cmd[1]), pc = -10;
		}
		debug();
		pc++;
	}
	return 0;
}

void debug() {
	int i;
	if(d_flag > 1) {
		for(i = 32 - 1; i >= 0; i--) {
			printf("%2d:%15d", i, ram[i]);
			if(i < ops.h)
				printf("%15d", ops.s[i]);
			else
				printf("%15s", "-");
			if(i < pcs.h)
				printf("%15d", pcs.s[i]);
			else
				printf("%15s", "-");
			printf("\n");
		}
		if(d_flag > 2)
			fgetc(stdin);
	}
}