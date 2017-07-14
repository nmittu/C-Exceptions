#include <stddef.h>
#include <stdio.h>
#include <setjmp.h>
#include "exceptions.h"

void bad_func() {
	throw(1, "bad_func: Exception Throws!!");
}

int main() {
	init_exception_handling();

	{
		exception* e;
		if ((e = try(setjmp(jmp))) == NULL) {
			exception* e2;
			if ((e2 = try(setjmp(jmp))) == NULL){
				bad_func();
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
