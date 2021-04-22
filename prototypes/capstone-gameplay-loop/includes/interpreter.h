#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

void* interpret_func(char* func_name, GPtrArray* args,
                        returnType* return_type);

#endif
