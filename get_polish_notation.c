#include <stdio.h>

#include "config.h"
#include "get_tokens.h"
#include "structs.h"

/*
enum type_token { SIGN, NUMBER, NUMBER_FLOAT, CONSTANT, FUNC, OPENING_BLACKET, CLOSING_BLACKET, SEPARATOR_IN_FUNC, UNDEFINED_TYPE, END_ARR_TOKENS
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
	do {
		for (int ind = 0; arr_tokens[ind].type != END_ARR_TOKENS; ind++) {
			if (nesting == max_nesting) {
				/*
				if (ind >= 2 && arr_tokens[ind].type == FUNC) {
					ind = func();
				} else {
					ind = func();
				}
				*/
				ind = __simple_case(arr_tokens, res_arr_tokens, ind);
				if (ind == -1) return NULL;
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
	return res_arr_tokens;
}


unsigned
__simple_case(Token *arr_tokens, Token *res_arr_tokens, int ind)
{
	signed nesting = 0;
	int ind_end = ind;
	for (; nesting != -1 && arr_tokens[ind].type != END_ARR_TOKENS; ind_end++) {
		switch (arr_tokens[ind].type) {
			case OPENING_BLACKET:
				nesting++;
				break;
			case CLOSING_BLACKET:
				nesting--;
				break;
		}
	}
	if (nesting > 0) return -1;
	return __token_distribution(arr_tokens, res_arr_tokens, ind_start, ind_end);
}


unsigned
__token_distribution(Token *arr_tokens, Token *res_arr_tokens, unsigned ind_start, unsigned ind_end)
{
	static unsigned ind_sign = 0;
	static unsigned ind_number = 0;
	unsigned max_
	return ind_end - 1;
}
