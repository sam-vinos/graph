#include <stdio.h>

#include "get_tokens.h"
#include "structs.h"
#include "input_check.h"
#include "config.h"
#include "get_polish_notation.h"


int
main()
{
	if (config_check()) goto ERROR;
	Token *arr_tokens = get_tokens();
	int max_nesting = 0;
	if (!arr_tokens || (max_nesting = input_check(arr_tokens)) == -1) {
		ERROR:
		fprintf(stderr, "n/a\n");
		return 1;
	}

	//arr_tokens = get_polich_notation(arr_tokens, max_nesting);

	unsigned len_arr_tokens = get_len_arr_tokens(arr_tokens);
	print_arr_tokens(arr_tokens);
	printf("len = %u\n", len_arr_tokens);
	free_arr_tokens(arr_tokens);

	return 0;
}
