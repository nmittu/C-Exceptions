#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "exceptions.h"

#define CUSTOM_EXCEPTION 15

struct custom_exception{
	int secondary_code;
	int line_num;
	char* file_name;
};

void bad_func() {
	throw(1, "bad_func: Exception Thrown!!");
}

void bad_func_with_code(int code) {
	char* msg = malloc(sizeof(char) * 100);
	sprintf(msg, "bad_func: Exception Thrown!! code = %d", code);
	throw_and_free(code, msg);
}

int main() {
	init_exception_handling();

	{
		exception* e;
		if ((e = try(setjmp(jmp))) == NULL) {
			exception* e2;
			if ((e2 = try(setjmp(jmp))) == NULL){
				exception* e3;
				if ((e3 = try(setjmp(jmp))) == NULL){
					printf("%s\n", "No exception here");
					end_try();
				}else{
					throw_existing(e3);
				}
				bad_func();
				end_try();
			}else{
				throw_existing(e2);
			}
			end_try();
		}else {
			printf("%s: code: %d\n", e->message, e->code);
			destroy_exception(e);
		}
	}

	{
		exception* e;
		if ((e = try(setjmp(jmp))) == NULL){
			bad_func_with_code(10);
			end_try();
		}else{
			printf("%s: code: %d\n", e->message, e->code);
			destroy_exception(e);
		}
	}

	{
		exception* e;
		if ((e = try(setjmp(jmp))) == NULL){
			struct custom_exception* exc = malloc(sizeof(struct custom_exception));
			exc->secondary_code = 1;
			exc->line_num = 69;
			exc->file_name = "main.c";
			throw_with_data(CUSTOM_EXCEPTION, "Reason for exception", exc);
			end_try();
		}else{
			if (e->code == CUSTOM_EXCEPTION){
				struct custom_exception* exc = (struct custom_exception*) e->data;
				printf("[%s:%d] Code: %d Secondary Code: %d Message: %s\n", exc->file_name, exc->line_num, e->code, exc->secondary_code, e->message);
				free(exc);
			}
			destroy_exception(e);
		}
	}

	throw(100, "Uncaught Exception With Custom Code");

	printf("This won't print.\n");

	deinit_exception_handling();
}
