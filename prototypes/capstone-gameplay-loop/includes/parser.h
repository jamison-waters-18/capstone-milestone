#ifndef PARSER_H
#define PARSER_H

#include "utils.h"

typedef enum { LIT_EXPR, STR_EXPR, FUNC_EXPR
} exprType;

char* find_end_of_str(char* start_of_str);

char* find_end_of_bracket(char* start_of_bracket);

char* find_end_of_arg(char* start_of_var);

char* find_end_of_expr(char* start_of_expr);

exprType find_expr_type(char* expr);

void* eval_str(char* expr, returnType* return_type);

void* eval_lit(char* lit, returnType* return_type);

void* eval_func(char* expr, returnType* return_type);

void* eval_expr(char* expr, returnType* return_type);

void parse_sequence(char* sequence);

#endif
