#include <stdio.h>
#include <glib.h>
#include "../includes/interpreter.h"
#include "../includes/parser.h"
#include "../includes/blob.h"

funcType find_bool_func(char* func_name, returnType* return_type);

funcType find_data_manip_func(char* func_name, returnType* return_type);

funcType find_const_func(char* func_name, returnType* return_type);

funcType find_math_func(char* func_name, returnType* return_type);

funcType find_ui_func(char* func_name, returnType* return_type);


void* exec_nop(GPtrArray* args, returnType* return_type) {
	if (args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

void* exec_if(GPtrArray* args, returnType* return_type) {
	if (args->len != 3) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	gboolean* holder = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_BOOL) {
		safe_free(holder);
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	gboolean result = *holder;
	free(holder);


	if (result) {
		parse_sequence(args->pdata[1]);
	} else {
		parse_sequence(args->pdata[2]);
	}
	return SUCCESSFUL_EMPTY_FUNC_EXEC;
}

funcType find_func(char* func_name, returnType* return_type) {
	if (!strcmp(func_name, "nop")) {
		*return_type = RTRN_EMPTY;
		return exec_nop;
	}
	if (!strcmp(func_name, "if")) {
		*return_type = RTRN_EMPTY;
		return exec_if;
	}

	funcType f = find_bool_func(func_name, return_type);
	if (f) return f;

	f = find_math_func(func_name, return_type);
	if (f) return f;
	
	f = find_const_func(func_name, return_type);
	if (f) return f;

	f = find_data_manip_func(func_name, return_type);
	if (f) return f;

	f = find_ui_func(func_name, return_type);
	if (f) return f;

	*return_type = RTRN_FAIL;
	return FAILED_FUNC_EXEC;
}

void* interpret_func(char* func_name, GPtrArray* args,
				returnType* return_type) {
	funcType exec_func = find_func(func_name, return_type);
	if (!exec_func) {
		fprintf(stdout, "invalid func name: %s\n", func_name);
		return FAILED_FUNC_EXEC;
	}
	void* result = exec_func(args, return_type);
	if (!result) {
		fprintf(stdout, "failed execution: %s\n", func_name);
	}
	return result;
}
