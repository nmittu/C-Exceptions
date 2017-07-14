#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

jmp_buf jmp;

typedef struct{
  int code;
  char* message;
} exception;

void init_exception_handling();

exception* try(int code);

void throw(int code, char* message);

void catch();

void end_try();

void deinit_exception_handling();

void destroy_exception(exception* e);

#endif
