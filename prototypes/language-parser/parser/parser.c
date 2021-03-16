#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "../includes/parser.h"
#include "../includes/interpreter.h"

char* find_end_of_str(char* start_of_str) {
	char* end = start_of_str + 1;
	while (*end != '"') {
		switch (*end) {
		case '\0':	
			fprintf(stderr, "malformed str\n");
			return NULL;
		case '/':
			end++;
			break;
		}
		if (!end) return NULL;
		end++;
	}
	return end;
}

char* find_end_of_bracket(char* start_of_bracket) {
	char* end = start_of_bracket + 1;
	while (*end != ')') {
		switch (*end) {
		case '\0':	
			fprintf(stderr, "malformed brackets\n");
			return NULL;
		case '/':
			end++;
			break;
		case '"':
			end = find_end_of_str(end);
			break;
		case '(':
			end = find_end_of_bracket(end);
			//break;
		}
		if (!end) return NULL;
		end++;
	}
	return end;
}

char* find_end_of_arg(char* start_of_var) {
	char* end = start_of_var;
	if (*end == '(') end++;
	while (*end != ',' && *end != ')') {
		switch (*end) {
		case '\0':	
			fprintf(stderr, "malformed args\n");
			return NULL;
		case '/':
			end++;
			break;
		case '"':
			end = find_end_of_str(end);
			break;
		case '(':
			end = find_end_of_bracket(end);
			//break;
		}
		if (!end) return NULL;
		end++;
	}
	return end;	
}

char* find_end_of_expr(char* start_of_expr) {
	char* end = start_of_expr;
	while (*end != ';') {
		switch (*end) {
		case '\0':	
			fprintf(stderr, "malformed expr\n");
			return NULL;
		case '/':
			end++;
			break;
		case '"':
			end = find_end_of_str(end);
			break;
		case '(':
			end = find_end_of_bracket(end);
			//break;
		}
		if (!end) return NULL;
		end++;
	}
	return end;
}

exprType find_expr_type(char* expr) {
	while (*expr) {
		switch (*expr) {
		case '"':
			return STR_EXPR;
			break;
		case '(':
			return FUNC_EXPR;
			//break;
		}
		expr++;
	}
	return LIT_EXPR;
}

void* eval_str(char* expr, returnType* return_type) {
	*return_type = RTRN_STR;
	char* end = find_end_of_str(expr++);
	*end = '\0';
	char* result = malloc(strlen(expr) + 1);
	strcpy(result, expr);
	*end = '"';
	fprintf(stdout, "str:  %s\n", result);
	return result;
}

void* eval_lit(char* expr, returnType* return_type) {
	if(!strcmp(expr, "true")) {
		*return_type = RTRN_BOOL;
		gboolean* result = malloc(sizeof(gboolean));
		*result = 1;
		return result;
	}
	if(!strcmp(expr, "false")) {
		*return_type = RTRN_BOOL;
		gboolean* result = malloc(sizeof(gboolean));
		*result = 0;
		return result;
	}
	//account for floats
	*return_type = RTRN_INT;
	if (!strlen(expr)) return NULL;
	int* result = malloc(sizeof(int));
	*result = atoi(expr);
	fprintf(stdout, "lit:  %d\n", *result);
	return result;
}

void* eval_func(char* expr, returnType* return_type) {
	char* bracket;
	for (bracket = expr; *bracket != '('; bracket++);

	*bracket = '\0';
	char* func_name = malloc(strlen(expr) + 1);
	strcpy(func_name, expr);
	*bracket = '(';

	char* end_bracket = find_end_of_bracket(bracket);
	GPtrArray* args = g_ptr_array_new();
	char* arg_parser = bracket + 1;
	while (arg_parser < end_bracket) {
		char* end_arg = find_end_of_arg(arg_parser);
		char symbol = *end_arg;
		*end_arg = '\0';
		char* arg = malloc(strlen(arg_parser) + 1);
		strcpy(arg, arg_parser);
		g_ptr_array_insert(args, -1, arg);
		*end_arg = symbol;
		arg_parser = end_arg + 1;
	}

	fprintf(stdout, "func: %s\n", func_name);
	for (int i = 0; i < args->len; i++) {
		fprintf(stdout, "    arg: %s\n", (char*) (args->pdata[i]));
	}

	void* return_val = interpret_func(func_name, args, return_type);

	free(func_name);
	for (int i = 0; i < args->len; i++) {
		free(args->pdata[i]);
	}
	g_ptr_array_free(args, TRUE);

	return return_val;
}

void* eval_expr(char* expr, returnType* return_type) {
	fprintf(stdout, "%s\n", expr);
	void* result = NULL;
	char* end_of_expr = find_end_of_expr(expr);
	*end_of_expr = '\0';
	exprType type = find_expr_type(expr);
	switch(type) {
	case STR_EXPR:
		result = eval_str(expr, return_type);
		break;
	case LIT_EXPR:
		result = eval_lit(expr, return_type);
		break;
	case FUNC_EXPR:
		result = eval_func(expr, return_type);
		//break;
	}
	*end_of_expr = ';';
	return result;
}

void parse_sequence(char* sequence) {
	char* start_of_expr = sequence;
	returnType ignore;
	while (*start_of_expr) {
		char* end_of_expr = find_end_of_expr(start_of_expr);
		if (!end_of_expr) return;
		char holder = *(++end_of_expr);
		*end_of_expr = '\0';
		void* result = eval_expr(start_of_expr, &ignore);
		if(result > (void*) 5) free(result);
		*end_of_expr = holder;
		start_of_expr = end_of_expr;
		while (*start_of_expr == ' '  || //ignores trailing whitespace
			 *start_of_expr == '\n' ||
			 *start_of_expr == '\t' ) {
			start_of_expr++; }
	}
}

