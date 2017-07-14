#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include "exceptions.h"

void bad_func() {
	throw(1, "bad_func: Exception Thrown!!");
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
					throw(e3->code, e3->message);
				}
				bad_func();
				end_try();
			}else{
				throw(e2->code, e2->message);
			}
			end_try();
		}else {
			printf("%s: code: %d\n", e->message, e->code);
			destroy_exception(e);
		}
	}

	deinit_exception_handling();
}
