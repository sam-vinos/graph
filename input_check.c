#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "config.h"
#include "get_tokens.h"

/*
enum type_token {SIGN, NUMBER, NUMBER_FLOAT, FUNC, OPENING_BLACKET, CLOSING_BLACKET, UNDEFINED_TYPE, END_ARR_TOKENS,
CONSTANT, SEPARATOR_IN_FUNC };
*/

static int __get_max_nesting(Token *arr_tokens); //так же проверяет синтаксис функций
static char __is_not_true_func(Token *arr_tokens, unsigned ind);
static char __is_not_true_separator(Token *arr_tokens, signed ind);
static char __main_analysis_syntax(Token *arr_tokens, int max_nesting);

static signed __true_arg_func(Token *arr_tokens, signed ind_start, unsigned char *name_func);
static signed __true_syntax_expression(Token *arr_tokens, signed ind_start);
static signed __expression_checking(Token *arr_tokens, signed ind_start, signed ind_end);


char
input_check(Token *arr_tokens)
{
	int max_nesting = __get_max_nesting(arr_tokens);
	if (max_nesting == -1) {
		ERROR_INPUT_CHECK:
		free_arr_tokens(arr_tokens);
		return 1;
	}
	printf("max_nesting = %d\n", max_nesting);
	if (__main_analysis_syntax(arr_tokens, max_nesting))
			goto ERROR_INPUT_CHECK;
	return 0;
}


static int
__get_max_nesting(Token *arr_tokens) //добавить еще функцию для проверки праивльного нахождения разделителя аргументов
{
	int max_nesting = 0, nesting = 0;
	//unsigned char number_separator = 0;
	for (unsigned ind = 0/*, ind_func = 0*/; arr_tokens[ind].type != END_ARR_TOKENS; ind++) {
		switch (arr_tokens[ind].type) {
			case OPENING_BLACKET:
				nesting++;
				if (max_nesting < nesting) max_nesting++;
				break;
			case CLOSING_BLACKET:
				nesting--;
				if (ind && arr_tokens[ind - 1].type == OPENING_BLACKET) {
					max_nesting = -1;
					fprintf(stderr, "%s\n", "Empty nesting");
					goto ERROR_GET_MAX_NESTING;
				}
				break;
			case FUNC:
				if (__is_not_true_func(arr_tokens, ind)) {
					max_nesting = -1;
					fprintf(stderr, "%s\n", "Incorrect function syntax");
					goto ERROR_GET_MAX_NESTING;
				}
				break;
			case SEPARATOR_IN_FUNC:
				if (__is_not_true_separator(arr_tokens, (signed)ind)) {
					max_nesting = -1;
					fprintf(stderr, "%s\n", "The divider is not in the right place");
					goto ERROR_GET_MAX_NESTING;
				}
				break;
		}
	}
	if (nesting) max_nesting = -1;
	ERROR_GET_MAX_NESTING: /// нужно лишь для того что бы выйти из цикла
	return max_nesting;
}


/*
#define LIST_NAMES_FUNCS "log\0arcsin\0sin\0cos\0\0"
#define LIST_NUMBER_ARGS_FUNCS {2, 1, 1, 1, -1}

#define LIST_NAMES_CONSTANTS "pi\0x\0X\0\0"
*/
static char
__is_not_true_func(Token *arr_tokens, unsigned ind)
{
	if (arr_tokens[ind + 1].type != OPENING_BLACKET) goto ERROR_IS_NOT_TRUE_FUNC;
	unsigned char list_names_funcs[] = LIST_NAMES_FUNCS;
	signed char list_number_args_funcs[] = LIST_NUMBER_ARGS_FUNCS;
	signed char number_args = 0;
	unsigned ind_func = 0;
	///search func
	for (signed ind_start_name_func = -1; list_number_args_funcs[ind_func] != -1; ind_func++) {
		ind_start_name_func++;
		if (!strcmp((char *)&list_names_funcs[ind_start_name_func], (char *)arr_tokens[ind].data.str)) {
			number_args = list_number_args_funcs[ind_func];
			ind += 2;
			break;
		}
		while (list_names_funcs[++ind_start_name_func]);
	}
	if (list_number_args_funcs[ind_func] == -1) goto ERROR_IS_NOT_TRUE_FUNC;
	///end search
	number_args--;
	for (signed nesting = 1; nesting && arr_tokens[ind].type != END_ARR_TOKENS; ind++) {
		switch (arr_tokens[ind].type) {
			case OPENING_BLACKET:
				nesting++;
				break;
			case CLOSING_BLACKET:
				nesting--;
				break;
			case SEPARATOR_IN_FUNC:
				if (nesting == 1) number_args--;
				else goto ERROR_IS_NOT_TRUE_FUNC;
				break;
		}
	}
	if (number_args) {
		ERROR_IS_NOT_TRUE_FUNC:
		return 1;
	}
	return 0;
}


static char
__is_not_true_separator(Token *arr_tokens, signed ind)
{
	signed nesting = 0;
	for (; ind >= 0 && arr_tokens[ind].type != FUNC; ind--) {
		switch (arr_tokens[ind].type) {
			case OPENING_BLACKET:
				nesting--;
				break;
			case CLOSING_BLACKET:
				nesting++;
				break;
		}
	}
	if (arr_tokens[ind].type != FUNC || nesting != -1) return 1;
	return 0;
}


static char
__main_analysis_syntax(Token *arr_tokens, int max_nesting)
{
	signed ind = 0;
	int nesting = 0;
	do {
		for (ind = 0, nesting = 0; arr_tokens[ind].type != END_ARR_TOKENS; ind++) {
			if (nesting == max_nesting) {
				/*if (ind >= 2 && arr_tokens[ind - 2].type == FUNC) {
					ind = __true_arg_func(arr_tokens, ind, arr_tokens[ind - 2].data.str);
					//nesting--;
				} else*/
				if (arr_tokens[ind].type == NUMBER || arr_tokens[ind].type == OPENING_BLACKET ||\
						arr_tokens[ind].type == CONSTANT ||\
						arr_tokens[ind].type == NUMBER_FLOAT)
					ind = __true_syntax_expression(arr_tokens, ind);
				if (ind == -1) {
					fprintf(stderr, "%s\n", "Error");
					return 1;
				}
			}
			switch (arr_tokens[ind].type) {
				case OPENING_BLACKET:
					nesting++;
					break;
				case CLOSING_BLACKET:
					nesting--;
					break;
			}
		}
	} while (max_nesting--);
	return 0;
}


static signed
__true_syntax_expression(Token *arr_tokens, signed ind_start)
{
	signed nesting = 0;
	signed ind_end = ind_start;
	for (; nesting != -1 && arr_tokens[ind_end].type != END_ARR_TOKENS; ind_end++) {
		switch (arr_tokens[ind_end].type) {
			case OPENING_BLACKET:
				nesting++;
				break;
			case CLOSING_BLACKET:
				nesting--;
				break;
		}
	}
	if (nesting > 0) return -1;
	printf("ind_start = %d; ind_end = %d\n", ind_start, ind_end);
	return __expression_checking(arr_tokens, ind_start, ind_end); //?name_func
}


static signed
__true_arg_func(Token *arr_tokens, signed ind_start, unsigned char *name_func)
{
	signed ind_result = 0;
	return ind_result;
}


static signed
__expression_checking(Token *arr_tokens, signed ind_start, signed ind_end)
{
	unsigned nesting = 0;
	char past_token = SIGN;
	unsigned len = 0;
	for (; ind_start != ind_end; ind_start++) {
		switch (arr_tokens[ind_start].type) {
			case FUNC:
			case OPENING_BLACKET:
			case NUMBER:
			case NUMBER_FLOAT:
			case CONSTANT:
				printf("number ind_start = %d\n", ind_start);
				if (past_token != SIGN) {
					puts("Error 1");
					return -1;
				}
				past_token = NUMBER;
				switch (arr_tokens[ind_start].type) {
					case FUNC:
						ind_start++;
					case OPENING_BLACKET:
						ind_start++;
						nesting = 1;
						while (nesting) {
							printf("nesting = %d; ind = %d\n", nesting, ind_start);
							switch (arr_tokens[ind_start].type) {
								case OPENING_BLACKET:
									nesting++;
									break;
								case CLOSING_BLACKET:
									nesting--;
									break;
							}
							ind_start++;
						}
						ind_start--;
						break;
				}
				break;
			case SIGN:
				printf("sign ind_start = %d\n", ind_start);
				if (past_token != NUMBER) {
					puts("Error 2");
					return -1;
				}
				past_token = SIGN;
				break;
			case CLOSING_BLACKET:
				len--;
				break;
		}
		len++;
		printf("LEN = %d\n", len);
	}
	if (~len & 1) return -1;
	puts("End __expression_checking()");
	return ind_end - 1;
}
