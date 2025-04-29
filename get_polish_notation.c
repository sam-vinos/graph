#include <stdio.h>

#include "config.h"
#include "get_tokens.h"
#include "structs.h"

/*
enum type_token { SIGN, NUMBER, NUMBER_FLOAT, CONSTANT, FUNC, OPENING_BLACKET, CLOSING_BLACKET, SEPARATOR_IN_FUNC,
UNDEFINED_TYPE, END_ARR_TOKENS
};
*/

unsigned char __get_max_priority_sign(Token *arr_tokens, unsigned ind_start, unsigned ind_end);
unsigned __simple_case(Token *arr_tokens, Token *res_arr_tokens, int ind);
//func case func (create!!!)
unsigned __token_distribution(Token *arr_tokens, Token *res_arr_tokens, unsigned ind_start, unsigned ind_end);




Token *
get_polich_notation(Token *arr_tokens, unsigned max_nesting)
{
	unsigned nesting = 0;
	Token *res_arr_tokens = get_copy_arr_tokens(arr_tokens);
	if (!res_arr_tokens) {
		fprintf(stderr, "Error\n");
		return NULL;
	}
	free_arr_tokens(arr_tokens);
	return res_arr_tokens;
}
