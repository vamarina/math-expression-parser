#ifndef MATH_EXPR_PARSER_POSTFIX_H
#define MATH_EXPR_PARSER_POSTFIX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *get_str_wo_spaces(char *str);
int is_operator(const char c);
int is_digit(const char c);
int precedence(char op);
double apply_op(double a, double b, char op);
double calc(char *str);

#endif
