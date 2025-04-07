#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "get_tokens.h"
#include "structs.h"


/*

enum type_token {SIGN, NUMBER, NUMBER_FLOAT, FUNC, BLACKETS, UNDEFINED_TYPE};
typedef struct token_t {
	union data {
		unsigned char s;
		signed number_int;
		float number_float;
		unsigned char *str;
	};
	char type;
} Token;

*/


static unsigned char *__input(void);
static char __data_correctness(const Token *arr_tokens);
static unsigned __is_token(const unsigned char *string, unsigned ind);


Token *
get_tokens(void)
{
	//const unsigned char allowed_charaters[] = "1234567890-+*/^()sincos\t ";
	unsigned char *string = __input();
#define MIN_LEN_ARR_TOKENS 10
	unsigned len_arr_tokens = MIN_LEN_ARR_TOKENS;
	Token *arr_tokens = (Token *)calloc(MIN_LEN_ARR_TOKENS, sizeof(Token));
	if (!arr_tokens) goto ERROR_GET_TOKENS;

	unsigned ind_tokens = 0;
	for (unsigned ind = 0; string[ind]; ind_tokens++) {
		if (ind_tokens == len_arr_tokens) {
			len_arr_tokens += MIN_LEN_ARR_TOKENS;
			arr_tokens = (Token *)realloc(arr_tokens, sizeof(Token) * len_arr_tokens);
			if (!arr_tokens) goto ERROR_GET_TOKENS;
		}
		ind += __is_token(string, ind);
		if (arr_tokens[ind_tokens].type == UNDEFINED_TYPE) goto ERROR_GET_TOKENS;
	}

	free(string);//!!!
	if (__data_correctness(arr_tokens)) {
		ERROR_GET_TOKENS:
		free(arr_tokens);
		return NULL;
	}
	return arr_tokens;
}




static unsigned char *
__input(void)
{
#define MIN_SIZE_STRING 10
	unsigned char *res = (unsigned char *)calloc(MIN_SIZE_STRING, sizeof(char));
	if (!res) goto ERROR_INPUT;
	unsigned counter = 0;
	unsigned len = MIN_SIZE_STRING;
	for (unsigned char s = 0; (s = getchar()) != '\n';) {
		res[counter++] = s;
		if (counter == len) {
			len += MIN_SIZE_STRING;
			res = (unsigned char *)realloc(res, len * sizeof(char));
			if (!res) goto ERROR_INPUT;
		}
	}
	res = (unsigned char *)realloc(res, (counter + 1) * sizeof(char));
	if (!res) {
		ERROR_INPUT:
		free(res);
		return NULL;
	}
	res[counter] = 0;
	return res;
}


static unsigned
__is_token(const unsigned char *string, unsigned ind)
{
	return 1;
}


static char
__data_correctness(const Token *arr_tokens)
{
	fprintf(stdin, "%p", arr_tokens);
	return 1;
}


int
main()
{
	unsigned char *string = __input();
	printf("%s\nlen = %ld\n", string, strlen((char *)string));
	return 0;
}
