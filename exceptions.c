#include <setjmp.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "exceptions.h"

int depth;
int max_depth;
jmp_buf* jumps;
exception e = { 0 };

void init_exception_handling() {
	depth = 0;
	max_depth = 10;
	jumps = malloc(sizeof(jmp_buf) * max_depth);
}

exception* try(int code) {
	if (depth == max_depth) {
		max_depth += 10;
		realloc(jumps, sizeof(jmp_buf) * max_depth);
	}

	if (code == 0) {
		memcpy(jumps + depth, jmp, sizeof(jmp_buf));
		depth++;
	}else {
		exception* ret = malloc(sizeof(exception));
		memcpy(ret, &e, sizeof(exception));
		return ret;
	}

	return NULL;
}

void throw(int code, char* message){
	e.code = code;
	e.message = malloc(strlen(message) + 1);
	memcpy(e.message, message, strlen(message) + 1);
	longjmp(jumps[--depth], code);
}

void end_try() {
	depth--;
}

void deinit_exception_handling() {
	free(jumps);
}

void destroy_exception(exception* e) {
	free(e->message);
}
