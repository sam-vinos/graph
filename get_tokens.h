#ifndef INPUT
#define INPUT

#include "structs.h"

Token *get_tokens(void);
void print_arr_tokens(Token *arr_tokens);
void free_arr_tokens(Token *arr_tokens, unsigned ind_end);

#endif
