#include <glib.h>
#include "../includes/interpreter.h"

void* exec_add(GPtrArray* args, returnType* return_type) {
	if (args->len != 2) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	void* result = NULL;
	returnType first_param_return_type;
	void* first_param = eval_expr(args->pdata[0], &first_param_return_type);
	returnType second_param_return_type;
	void* second_param = eval_expr(args->pdata[1], &second_param_return_type);

	if (first_param_return_type == RTRN_EMPTY ||
		second_param_return_type == RTRN_EMPTY) {
		*return_type = RTRN_FAIL;
		safe_free(first_param);
		safe_free(second_param);
	} else if (first_param_return_type == RTRN_INT &&
		second_param_return_type == RTRN_INT) {
		int* int1 = (int*) first_param;
		int* int2 = (int*) second_param;
		*int1 += *int2;
		result = int1;
		*return_type = RTRN_INT;
		free(int2);
	} else if (first_param_return_type == RTRN_FLOAT &&
		second_param_return_type == RTRN_INT) {
		double* dbl1 = (double*) first_param;
		int* int2 = (int*) second_param;
		*dbl1 += *int2;
		result = dbl1;
		free(int2);
	} else if (first_param_return_type == RTRN_INT &&
		second_param_return_type == RTRN_FLOAT) {
		int* int1 = (int*) first_param;
		double* dbl2 = (double*) first_param;
		*dbl2 += *int1;
		result = dbl2;
		free(int1);
	} else if (first_param_return_type == RTRN_FLOAT &&
		second_param_return_type == RTRN_FLOAT) {
		double* dbl1 = (double*) first_param;
		double* dbl2 = (double*) first_param;
		*dbl2 += *dbl1;
		result = dbl2;
		free(dbl1);
	} else {
		*return_type = RTRN_FAIL;
		safe_free(first_param);
		safe_free(second_param);
	}
	return result;
}

void* exec_sub(GPtrArray* args, returnType* return_type) {
	if (args->len != 2) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	void* result = NULL;
	returnType first_param_return_type;
	void* first_param = eval_expr(args->pdata[0], &first_param_return_type);
	returnType second_param_return_type;
	void* second_param = eval_expr(args->pdata[1], &second_param_return_type);

	if (first_param_return_type == RTRN_EMPTY ||
		second_param_return_type == RTRN_EMPTY) {
		*return_type = RTRN_FAIL;
		safe_free(first_param);
		safe_free(second_param);
	} else if (first_param_return_type == RTRN_INT &&
		second_param_return_type == RTRN_INT) {
		int* int1 = (int*) first_param;
		int* int2 = (int*) second_param;
		*int1 -= *int2;
		result = int1;
		*return_type = RTRN_INT;
		free(int2);
	} else if (first_param_return_type == RTRN_FLOAT &&
		second_param_return_type == RTRN_INT) {
		double* dbl1 = (double*) first_param;
		int* int2 = (int*) second_param;
		*dbl1 -= *int2;
		result = dbl1;
		free(int2);
	} else if (first_param_return_type == RTRN_INT &&
		second_param_return_type == RTRN_FLOAT) {
		int* int1 = (int*) first_param;
		double* dbl2 = (double*) first_param;
		*dbl2 = *int1 - *dbl2;
		result = dbl2;
		free(int1);
	} else if (first_param_return_type == RTRN_FLOAT &&
		second_param_return_type == RTRN_FLOAT) {
		double* dbl1 = (double*) first_param;
		double* dbl2 = (double*) first_param;
		*dbl2 -= *dbl1;
		result = dbl2;
		free(dbl1);
	} else {
		*return_type = RTRN_FAIL;
		safe_free(first_param);
		safe_free(second_param);
	}
	return result;
}

funcType find_math_func(char* func_name, returnType* return_type) {
	if (!strcmp(func_name, "add")) {
		*return_type = RTRN_FLOAT;
		return exec_add;
	}
	if (!strcmp(func_name, "sub")) {
		*return_type = RTRN_FLOAT;
		return exec_sub;
	}
	*return_type = RTRN_FAIL;
	return FAILED_FUNC_EXEC;
}
