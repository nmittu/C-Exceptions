#include <stddef.h>
#include <stdio.h>
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
		jmp_buf* jumps_tmp;
		max_depth += 10;
		jumps_tmp = realloc(jumps, sizeof(jmp_buf) * max_depth);
		if (jumps_tmp == NULL){
			throw(BAD_ALLOC, "exceptions.c:21 realloc failed");
		}
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
	if (depth < 1){
		char* exc_type = code_to_string(code);
		if (!exc_type){
			fprintf(stderr, "Custom Exception Code: %d - %s\n", code, message);
			exit(-1);
		}
		fprintf(stderr, "%s - %s\n", exc_type, message);
		exit(-1);
	}
	e.code = code;
	e.message = malloc(strlen(message) + 1);
	e.data = NULL;
	memcpy(e.message, message, strlen(message) + 1);
	longjmp(jumps[--depth], code);
}

void throw_existing(exception* ex){
	if (depth < 1){
		char* exc_type = code_to_string(ex->code);
		if (!exc_type){
			fprintf(stderr, "Custom Exception Code: %d - %s\n", ex->code, ex->message);
			exit(-1);
		}
		fprintf(stderr, "%s - %s\n", exc_type, ex->message);
		exit(-1);
	}
	e.code = ex->code;
	e.message = ex->message;
	e.data = ex->data;
	free(ex);
	longjmp(jumps[--depth], e.code);
}

void throw_and_free(int code, char* message){
	if (depth < 1){
		char* exc_type = code_to_string(code);
		if (!exc_type){
			fprintf(stderr, "Custom Exception Code: %d - %s\n", code, message);
			exit(-1);
		}
		fprintf(stderr, "%s - %s\n", exc_type, message);
		exit(-1);
	}
	e.code = code;
	e.message = malloc(strlen(message) + 1);
	e.data = NULL;
	memcpy(e.message, message, strlen(message) + 1);
	free(message);
	longjmp(jumps[--depth], code);
}

void throw_with_data(int code, char* message, void* data){
	if (depth < 1){
		char* exc_type = code_to_string(code);
		if (!exc_type){
			fprintf(stderr, "Custom Exception Code: %d - %s\n", code, message);
			exit(-1);
		}
		fprintf(stderr, "%s - %s\n", exc_type, message);
		exit(-1);
	}
	e.code = code;
	e.message = malloc(strlen(message) + 1);
	e.data = data;
	memcpy(e.message, message, strlen(message) + 1);
	longjmp(jumps[--depth], code);
}

void throw_with_data_and_free(int code, char* message, void* data){
	if (depth < 1){
		char* exc_type = code_to_string(code);
		if (!exc_type){
			fprintf(stderr, "Custom Exception Code: %d - %s\n", code, message);
			exit(-1);
		}
		fprintf(stderr, "%s - %s\n", exc_type, message);
		exit(-1);
	}
	e.code = code;
	e.message = malloc(strlen(message) + 1);
	e.data = data;
	memcpy(e.message, message, strlen(message) + 1);
	free(message);
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
	free(e);
}

char* code_to_string(int code){
	char* ret;
	switch(code){
	case BAD_ALLOC:
		ret = "BAD_ALLOCATION";
		break;
	case NULL_PTR:
		ret = "NULL_POINTER_EXCEPTION";
		break;
	case ILLEGAL_ARG:
		ret = "ILLEGAL_ARGUMENT_EXCEPTION";
		break;
	case OUT_OF_RANGE:
		ret = "OUT_OF_RANGE_EXCEPTION";
		break;
	case OVERFLOW:
		ret = "OVERFLOW_EXCEPTION";
		break;
	case UNDERFLOW:
		ret = "UNDERFLOW_EXCEPTION";
		break;
	case ARITHMETIC_EXC:
		ret = "ARITHMETIC_EXCEPTION";
		break;
	case SYSTEM_ERROR:
		ret = "SYSTEM_ERROR";
		break;
	default:
		ret = NULL;
	}

	return ret;
}
