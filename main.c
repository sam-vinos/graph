#include <stdio.h>

#include "get_tokens.h"
#include "structs.h"
#include "input_check.h"
#include "config.h"


int
main()
{
	if (config_check()) goto ERROR;
	Token *arr_tokens = get_tokens();
	if (!arr_tokens || input_check(arr_tokens)) {
		ERROR:
		fprintf(stderr, "n/a\n");
		return 1;
	}
	print_arr_tokens(arr_tokens);
	printf("len = %u\n", get_len_arr_tokens(arr_tokens));
	free_arr_tokens(arr_tokens);
	return 0;
}
