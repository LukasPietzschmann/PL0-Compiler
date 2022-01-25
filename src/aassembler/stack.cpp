#include "stack.hpp"

int push(stack* s, int val) {
	int rc = -1;
	if(s->h < stack_max_height)
		s->s[(s->h)++] = val, rc = 0;
	return rc;
}

int pop(stack* s, int* val) {
	int rc = -1;
	if(s->h > 0)
		*val = s->s[--(s->h)], rc = 0;
	return rc;
}

int tos(stack* s, int* val) {
	int rc = -1;
	if(s->h > 0)
		*val = s->s[s->h], rc = 0;
	return rc;
}