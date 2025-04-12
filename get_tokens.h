#ifndef INPUT
#define INPUT

#include "structs.h"

Token *get_tokens(void);
void print_arr_tokens(Token *arr_tokens);
void _free_arr_tokens(Token *arr_tokens, unsigned ind_end);
#define free_arr_tokens(arr_tokens) _free_arr_tokens(arr_tokens, 0)
unsigned get_len_arr_tokens(Token *arr_tokens);

#endif
