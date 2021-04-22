#include <glib.h>
#include "../includes/basic_bool_interpreter.h"

void* exec_not(GPtrArray* args, returnType* return_type) {
	if (args->len != 1) {
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

	*holder = !(*holder);
	return holder;
}

void* exec_and(GPtrArray* args, returnType* return_type) {
	if (!args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	gboolean* holder = NULL;
	for (int i = 0; i < args->len; i++) {
		returnType sub_return_type;
		free(holder);
		holder = eval_expr(args->pdata[i], &sub_return_type);
		

		if (sub_return_type != RTRN_BOOL) {
			safe_free(holder);
			*return_type = RTRN_FAIL;
			return FAILED_FUNC_EXEC;
		}
		if (!(*holder)) {
			return holder;
		}
	}
	return holder;
}

void* exec_or(GPtrArray* args, returnType* return_type) {
	if (!args->len) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	gboolean* holder = NULL;
	for (int i = 0; i < args->len; i++) {
		returnType sub_return_type;
		free(holder);
		holder = eval_expr(args->pdata[i], &sub_return_type);
		

		if (sub_return_type != RTRN_BOOL) {
			safe_free(holder);
			*return_type = RTRN_FAIL;
			return FAILED_FUNC_EXEC;
		}
		if (*holder) {
			return holder;
		}
	}
	return holder;
}

void* exec_gt(GPtrArray* args, returnType* return_type) {
	if (args->len != 2) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	gboolean* result = malloc(sizeof(gboolean*));
	returnType first_param_return_type;
	void* first_param = eval_expr(args->pdata[0], &first_param_return_type);
	returnType second_param_return_type;
	void* second_param = eval_expr(args->pdata[1], &second_param_return_type);

	if (first_param_return_type == RTRN_EMPTY ||
		second_param_return_type == RTRN_EMPTY) {
		*return_type = RTRN_FAIL;
	} else if (first_param_return_type == RTRN_INT &&
		second_param_return_type == RTRN_INT) {
		int* int1 = (int*) first_param;
		int* int2 = (int*) second_param;
		*result = *int1 > *int2;
	} else if (first_param_return_type == RTRN_FLOAT &&
		second_param_return_type == RTRN_INT) {
		double* dbl1 = (double*) first_param;
		int* int2 = (int*) second_param;
		*result = *dbl1 > *int2;
	} else if (first_param_return_type == RTRN_INT &&
		second_param_return_type == RTRN_FLOAT) {
		int* int1 = (int*) first_param;
		double* dbl2 = (double*) first_param;
		*result = *int1 > *dbl2;
	} else if (first_param_return_type == RTRN_FLOAT &&
		second_param_return_type == RTRN_FLOAT) {
		double* dbl1 = (double*) first_param;
		double* dbl2 = (double*) first_param;
		*result = *dbl1 > *dbl2;
	} else {
		*return_type = RTRN_FAIL;
	}
	safe_free(first_param);
	safe_free(second_param);
	return result;
}

void* exec_eq(GPtrArray* args, returnType* return_type) {
	if (args->len != 2) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	gboolean* result = malloc(sizeof(gboolean));
	returnType first_param_return_type;
	void* first_param = eval_expr(args->pdata[0], &first_param_return_type);
	returnType second_param_return_type;
	void* second_param = eval_expr(args->pdata[1], &second_param_return_type);

	
	if (first_param_return_type == RTRN_EMPTY ||
		second_param_return_type == RTRN_EMPTY) {
		*return_type = RTRN_FAIL;
	} else if (first_param_return_type == RTRN_FLOAT &&
		second_param_return_type == RTRN_INT) {
		double* dbl1 = (double*) first_param;
		int* int2 = (int*) second_param;
		*dbl1 -= *int2;
		*result = (-FLOAT_EQ_TOLERANCE < *dbl1 && *dbl1 < FLOAT_EQ_TOLERANCE);
	} else if (first_param_return_type == RTRN_INT &&
		second_param_return_type == RTRN_FLOAT) {
		int* int1 = (int*) first_param;
		double* dbl2 = (double*) first_param;
		*dbl2 = *int1 - *dbl2;
		*result = (-FLOAT_EQ_TOLERANCE < *dbl2 && *dbl2 < FLOAT_EQ_TOLERANCE);
	} else if (first_param_return_type == RTRN_FLOAT &&
		second_param_return_type == RTRN_FLOAT) {
		double* dbl1 = (double*) first_param;
		double* dbl2 = (double*) first_param;
		*dbl2 -= *dbl1;
		*result = (-FLOAT_EQ_TOLERANCE < *dbl2 && *dbl2 < FLOAT_EQ_TOLERANCE);
	} else if (first_param_return_type == second_param_return_type){
		int* int1 = (int*) first_param;
		int* int2 = (int*) second_param;
		*result = (int1 == int2);
	} else {
		*result = FALSE;
	}
	safe_free(first_param);
	safe_free(second_param);
	return result;
}

void* exec_has_all_flags(GPtrArray* args, returnType* return_type) {
	if (args->len < 2) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	returnType sub_return_type;
	int* flags_ptr = eval_expr(args->pdata[0], &sub_return_type);
	if (sub_return_type != RTRN_FLAG) {
		*return_type = RTRN_FAIL;
		return FAILED_FUNC_EXEC;
	}
	int flags = *flags_ptr;
	free(flags_ptr);
	gboolean* result = malloc(sizeof(gboolean));
	*result = FALSE;

	for(int i = i; i < args->len; i++) {
		int* flag_ptr = eval_expr(args->pdata[i], &sub_return_type);
		if (sub_return_type != RTRN_FLAG) {
			free(result);
			free(flag_ptr);
			*return_type = RTRN_FAIL;
			return FAILED_FUNC_EXEC;
		}
		int flag = *flag_ptr;
		free(flag_ptr);
		if(!(flags & flag)) {
			return result;
		}
	}

	*result = TRUE;
	return result;
}

funcType find_bool_func(char* func_name, returnType* return_type) {
	*return_type = RTRN_BOOL;
	if (!strcmp(func_name, "and")) {
		return exec_and;
	}
	if (!strcmp(func_name, "or")) {
		return exec_or;
	}
	if (!strcmp(func_name, "not")) {
		return exec_not;
	}
	if (!strcmp(func_name, "gt")) {
		return exec_gt;
	}
	if (!strcmp(func_name, "eq")) {
		return exec_eq;
	}
	if (!strcmp(func_name, "has_all_flags")) {
		return exec_has_all_flags;
	}
	*return_type = RTRN_FAIL;
	return FAILED_FUNC_EXEC;
}
