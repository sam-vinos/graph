#include <stdio.h>

#include "get_tokens.h"
#include "structs.h"


int
main()
{
	Token *arr_tokens = get_tokens();
	if (!arr_tokens) {
		printf("Error: arr_tokens = NULL\n");
		return 1;
	}
	print_arr_tokens(arr_tokens);
	return 0;
}
