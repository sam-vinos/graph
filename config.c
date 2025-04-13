#include <stdio.h>

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
		if (!list_names_funcs[ind_list_funcs]) break;
		while (list_names_funcs[++ind_list_funcs]);
		ind_list_funcs++;
		printf("list_names_funcs[%d] = %d\n", ind_list_funcs, list_names_funcs[ind_list_funcs]);
	}
	if (!(!list_names_funcs[ind_list_funcs] && list_number_args_funcs[ind_list_args] == -1)) {
		ERROR:
		fprintf(stderr, "%s\n", "Configuration error");
		return 1;
	}
	for (; list_names_constants[ind_list_constants]; ind_list_constants++) {
		while (list_names_constants[++ind_list_constants]);
		printf("list_names_constants[%d] = %d\n", ind_list_constants, list_names_constants[ind_list_constants]);
	}
	if (list_names_constants[ind_list_constants + 1]) goto ERROR;
	return 0;
}
