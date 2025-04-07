#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "get_tokens.h"
#include "structs.h"


/*

enum type_token {SIGN, NUMBER, NUMBER_FLOAT, FUNC, BLACKETS, UNDEFINED_TYPE};
typedef struct token_t {
	union data {
		unsigned char symbol;
		signed number_int;
		float number_float;
		unsigned char *str;
	};
	char type;
} Token;

*/

#define LIST_FUNC_STRING "sin cos tng ctng"

static void __input(void);
static char __data_correctness(void);
static unsigned __is_token(unsigned ind, unsigned ind_token, const unsigned char *list_funcs_string);

static  unsigned char *string;
static Token *arr_tokens;


Token *
get_tokens(void)
{
	const unsigned char list_funcs_string[] = LIST_FUNC_STRING;
	__input();
#define MIN_LEN_ARR_TOKENS 10
	unsigned len_arr_tokens = MIN_LEN_ARR_TOKENS;
	arr_tokens = (Token *)calloc(MIN_LEN_ARR_TOKENS, sizeof(Token));
	if (!arr_tokens || !string) {
		free(string);
		goto ERROR_GET_TOKENS;
	}

	unsigned ind_token = 0;
	for (unsigned ind = 0; string[ind]; ind_token++) {
		if (ind_token == len_arr_tokens) {
			len_arr_tokens += MIN_LEN_ARR_TOKENS;
			arr_tokens = (Token *)realloc(arr_tokens, sizeof(Token) * len_arr_tokens);
			if (!arr_tokens) {
				free(string);
				goto ERROR_GET_TOKENS;
			}
		}
		ind += __is_token(ind, ind_token, list_funcs_string);
		if (arr_tokens[ind_token].type == UNDEFINED_TYPE) {
			free(string);
			goto ERROR_GET_TOKENS;
		}
	}

	free(string);
	if (__data_correctness()) {
		ERROR_GET_TOKENS:
		free(arr_tokens);
		arr_tokens = NULL;
	}
	return arr_tokens;
}


static void
__input(void)
{
#define MIN_SIZE_STRING 10
	string = (unsigned char *)calloc(MIN_SIZE_STRING, sizeof(char));
	if (!string) goto ERROR_INPUT;
	unsigned counter = 0;
	unsigned len = MIN_SIZE_STRING;
	for (unsigned char s = 0; (s = getchar()) != '\n';) {
		string[counter++] = s;
		if (counter == len) {
			len += MIN_SIZE_STRING;
			string = (unsigned char *)realloc(string, len * sizeof(char));
			if (!string) goto ERROR_INPUT;
		}
	}
	string = (unsigned char *)realloc(string, (counter + 1) * sizeof(char));
	if (!string) {
		ERROR_INPUT:
		free(string);
		string = NULL;
	} else string[counter] = 0;
}


#define IS_NUMBER(s) (s >= '0' && s <= '9')
#define IS_NEGATIVE_NUMBER(ind, arr) (!ind || (arr[ind - 1].type == SIGN ||\
			(arr[ind - 1].type == BLACKETS && arr[ind - 1].data.symbol == '(')))
#define IS_SIGN(s) (s == '-' || s == '+' || s == '*' || s == '/' || s == '^')
#define IS_BLACKETS(s) (s == '(' || s == ')')
#define IS_FUNC(s) ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z'))
static unsigned
__is_token(unsigned ind, unsigned ind_token, const unsigned char *list_funcs_string)
{
	//const unsigned char allowed_charaters[] = "1234567890-+*/^()sincos\t ";
	unsigned char symbol = string[ind];
	unsigned offset = 0;
	if (IS_NUMBER(symbol) || (symbol == '-' && IS_NEGATIVE_NUMBER(ind_token, arr_tokens))) {

		//number or number_float

	} else if (IS_SIGN(symbol)) {
		arr_tokens[ind_token].type = SIGN;
		arr_tokens[ind_token].data.symbol = symbol;
		offset = 1;
	} else if (IS_BLACKETS(symbol)) {
		arr_tokens[ind_token].type = BLACKETS;
		arr_tokens[ind_token].data.symbol = symbol;
		offset = 1;
	} else if (IS_FUNC(symbol)) {
		unsigned char t_symbol = 0;
		for (unsigned ind = 0, ind_start, ind_end; list_func_string[ind];) {
			ind_start = ind;
			for (ind_end = ind_start + 1; !IS_FUNC(list_func_string[ind_end]); ind_end++);
			t_sumbol = list_func_string[ind_end];
			list_func_string[ind_end] = '\0';
			if (&stringi[ind] == strstr(&string[ind], &list_func_string[ind_start])) {
				list_func_string[ind_end] = t_symbol;
				/*
				
				 */
			}
			list_func_string[ind_end] = t_symbol;
			if (!list_func_string[ind_end]) ind = ind_end;
			else {
			}
		}
		goto ERROR_IS_TOKEN;
	} else {
		ERROR_IS_TOKEN:
		arr_tokens[ind_token].type = UNDEFINED_TYPE;
		arr_tokens[ind_token].data.str = NULL;
	}
	return offset;
}


static char
__data_correctness(void)
{
	//fprintf(stdin, "%p", arr_tokens);
	return 1;
}


int
main()
{
	__input();
	printf("%s\nlen = %ld\n", string, strlen((char *)string));
	return 0;
}
