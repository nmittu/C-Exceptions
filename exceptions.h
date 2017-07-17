#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#define BAD_ALLOC 1
#define NULL_PTR 2
#define ILLEGAL_ARG 3
#define OUT_OF_RANGE 4
#define OVERFLOW 5
#define UNDERFLOW 6
#define ARITHMETIC_EXC 7
#define SYSTEM_ERROR 8

#include <setjmp.h>

jmp_buf jmp;

typedef struct{
  int code;
  char* message;
  void* data;
} exception;

void init_exception_handling();
exception* try(int code);
void throw(int code, char* message);
void throw_existing(exception* ex);
void throw_and_free(int code, char* message);
void throw_with_data(int code, char* message, void* data);
void throw_with_data_and_free(int code, char* message, void* data);
void catch();
void end_try();
void deinit_exception_handling();
void destroy_exception(exception* e);
char* code_to_string(int code);

#endif
