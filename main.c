#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "exceptions.h"

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

	throw(100, "Uncaught Exception With Custom Code");

	deinit_exception_handling();
}
