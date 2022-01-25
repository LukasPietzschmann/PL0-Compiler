#pragma once

#define stack_max_height 256

typedef struct {
	int s[stack_max_height];
	int h;
} stack;

int push(stack*, int);
int pop(stack*, int*);
int tos(stack*, int*);