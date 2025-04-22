#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"

/*
#define LIST_NAMES_FUNCS "log\0arcsin\0sin\0cos\0\0"
#define LIST_NUMBER_ARGS_FUNCS {2, 1, 1, 1}

#define LIST_NAMES_CONSTANTS "pi\0x\0X\0\0"
*/

char
config_check(void)
{
	unsigned char list_names_funcs[] = LIST_NAMES_FUNCS;
	signed char list_number_args_funcs[] = LIST_NUMBER_ARGS_FUNCS;
	unsigned char list_names_constants[] = LIST_NAMES_CONSTANTS;
	unsigned ind_list_funcs = 0;
	unsigned ind_list_args = 0;
	unsigned ind_list_constants = 0;
	for (; list_number_args_funcs[ind_list_args] != -1; ind_list_args++) {
		if (!list_number_args_funcs[ind_list_args]) goto ERROR;
		if (!list_names_funcs[ind_list_funcs]) break;
		while (list_names_funcs[++ind_list_funcs]);
		ind_list_funcs++;
#ifdef TEST
		printf("list_names_funcs[%d] = %d\n", ind_list_funcs, list_names_funcs[ind_list_funcs]);
#endif
	}
	if (!(!list_names_funcs[ind_list_funcs] && list_number_args_funcs[ind_list_args] == -1)) {
		ERROR:
		fprintf(stderr, "%s\n", "Configuration error");
		return 1;
	}
	for (; list_names_constants[ind_list_constants]; ind_list_constants++) {
		while (list_names_constants[++ind_list_constants]);
#ifdef TEST
		printf("list_names_constants[%d] = %d\n", ind_list_constants, list_names_constants[ind_list_constants]);
#endif
	}
	if (list_names_constants[ind_list_constants + 1]) goto ERROR;
	return 0;
}


signed char
number_args_func(unsigned char *name_func)
{
	unsigned char list_name_func[] = LIST_NAMES_FUNCS;
	signed char list_number_args_funcs[] = LIST_NUMBER_ARGS_FUNCS;
	signed char result_number = -1;
	for (unsigned ind_list_args = 0, ind_name_funcs = 0; list_number_args_funcs[ind_list_args] != -1;
			ind_list_args++) {
		if (!strcmp((char *)name_func, (char *)&list_name_func[ind_name_funcs])) {
			result_number = list_number_args_funcs[ind_list_args];
			break;
		}
		while (list_name_func[++ind_name_funcs]);
		ind_name_funcs++;
	}
	return result_number;
}


Token *
get_copy_arr_tokens(Token *arr_tokens)
{
	Token *copy_arr_tokens = (Token *)malloc(sizeof(Token) * (get_len_arr_tokens(arr_tokens) + 1));
	unsigned ind = 0;
	for (; arr_tokens[ind].type != END_ARR_TOKENS; ind++)
		copy_arr_tokens[ind] = arr_tokens[ind];
	copy_arr_tokens[ind] = arr_tokens[ind];
	return copy_arr_tokens;
}
