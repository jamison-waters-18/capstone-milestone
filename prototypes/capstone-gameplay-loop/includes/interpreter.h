#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

#define SUCCESSFUL_EMPTY_FUNC_EXEC (void*) 1	//TODO: get rid of this
#define FAILED_FUNC_EXEC 0

typedef void* (*funcType)(GPtrArray*, returnType*);

void* interpret_func(char* func_name, GPtrArray* args,
                        returnType* return_type);

#endif
