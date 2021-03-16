#include <stdio.h>
#include <glib.h>
#include "../includes/interpreter.h"
#include "../includes/parser.h"

#define SUCCESSFUL_EMPTY_FUNC_EXEC (void*) 1
#define FAILED_EMPTY_FUNC_EXEC 0

typedef void* (*funcType)(GPtrArray*);

void* exec_if(GPtrArray* args) {
	returnType return_type;
	gboolean* holder = eval_expr(args->pdata[0], &return_type);
	gboolean result = *holder;
	free(holder);

	if (return_type != RTRN_BOOL) {
		return FAILED_EMPTY_FUNC_EXEC;
	}

	if (result) {
		parse_sequence(args->pdata[1]);
	} else {
		parse_sequence(args->pdata[2]);
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

funcType find_func(char* func_name, returnType* return_type) {
	if (!strcmp(func_name, "if")) {
		*return_type = RTRN_EMPTY;
		return exec_if;
	}
	*return_type = RTRN_FAIL;
	return NULL;
}

void* interpret_func(char* func_name, GPtrArray* args,
				returnType* return_type) {
	funcType exec_func = find_func(func_name, return_type);
	if (!exec_func) {
		fprintf(stdout, "invalid func name: %s\n", func_name);
		return NULL;
	}
	return exec_func(args);
}
