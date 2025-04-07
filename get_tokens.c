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


static void __input(void);
static char __data_correctness(void);
static unsigned __is_token(unsigned ind);

static  unsigned char *string;
static Token *arr_tokens;


Token *
get_tokens(void)
{
	__input();
#define MIN_LEN_ARR_TOKENS 10
	unsigned len_arr_tokens = MIN_LEN_ARR_TOKENS;
	arr_tokens = (Token *)calloc(MIN_LEN_ARR_TOKENS, sizeof(Token));
	if (!arr_tokens) {
		free(string);
		goto ERROR_GET_TOKENS;
	}

	unsigned ind_tokens = 0;
	for (unsigned ind = 0; string[ind]; ind_tokens++) {
		if (ind_tokens == len_arr_tokens) {
			len_arr_tokens += MIN_LEN_ARR_TOKENS;
			arr_tokens = (Token *)realloc(arr_tokens, sizeof(Token) * len_arr_tokens);
			if (!arr_tokens) {
				free(string);
				goto ERROR_GET_TOKENS;
			}
		}
		ind += __is_token(ind);
		if (arr_tokens[ind_tokens].type == UNDEFINED_TYPE) {
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
	return string;
}


static unsigned
__is_token(unsigned ind)
{
	//const unsigned char allowed_charaters[] = "1234567890-+*/^()sincos\t ";
	unsigned char symbol = string[ind];
	unsigned offset = 0;
	/*if ((symbol >= '0' && symbol <= '9') || (symbol == '-' && )) {
	} else {
	}*/
	return 1;
}


static char
__data_correctness(void)
{
	fprintf(stdin, "%p", arr_tokens);
	return 1;
}


int
main()
{
	__input();
	printf("%s\nlen = %ld\n", string, strlen((char *)string));
	return 0;
}
