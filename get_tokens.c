#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "get_tokens.h"
#include "structs.h"



/*
enum type_token {SIGN, NUMBER, NUMBER_FLOAT, FUNC, OPENING_BLACKET, CLOSING_BLACKET, UNDEFINED_TYPE, END_ARR_TOKENS};
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


static void __input(void);
static char __data_correctness(void);
static unsigned __is_token(unsigned ind, unsigned ind_token);
static char __preproces(void);

static  unsigned char *string;
static Token *arr_tokens;


#define MIN_LEN_ARR_TOKENS 10
Token *
get_tokens(void)
{
	__input();
	unsigned len_arr_tokens = MIN_LEN_ARR_TOKENS;
	arr_tokens = (Token *)calloc(len_arr_tokens, sizeof(Token));
	unsigned ind_token = 0;
	if (!arr_tokens || !string || __preproces()) {
		free(string);
		goto ERROR_GET_TOKENS;
	}
	for (unsigned ind = 0; string[ind]; ind_token++) {
		if (ind_token == len_arr_tokens) {
			len_arr_tokens += MIN_LEN_ARR_TOKENS;
			arr_tokens = (Token *)realloc(arr_tokens, sizeof(Token) * len_arr_tokens);
			if (!arr_tokens) {
				free(string);
				goto ERROR_GET_TOKENS;
			}
		}
		ind = __is_token(ind, ind_token);
		if (arr_tokens[ind_token].type == UNDEFINED_TYPE) {
			free(string);
			goto ERROR_GET_TOKENS;
		}
#ifdef TEST
		printf("\nind = %u\nsymbol = |%c| ascii number symbol = %d\n\n", ind, string[ind], (int)string[ind]);
#endif
	}
	if (ind_token == len_arr_tokens) {
		len_arr_tokens += MIN_LEN_ARR_TOKENS;
		arr_tokens = (Token *)realloc(arr_tokens, sizeof(Token) * len_arr_tokens);
		if (!arr_tokens) {
			free(string);
			goto ERROR_GET_TOKENS;
		}
	}
	arr_tokens[ind_token].type = END_ARR_TOKENS;
	arr_tokens[ind_token].data.str = NULL;
	free(string);
	if (__data_correctness()) {
		ERROR_GET_TOKENS:
		//free(arr_tokens);
		_free_arr_tokens(arr_tokens, ind_token);
		arr_tokens = NULL;
#ifdef TEST
		puts("ERROR");
#endif
	}
	return arr_tokens;
}


#define MIN_SIZE_STRING 10
static void
__input(void)
{
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


#define IS_ALPHA(s) ((s >= 'a' && s <= 'z') || (s >= 'A' && s <= 'Z') || s == '_')
#define IS_SIGN(s) (s == '-' || s == '+' || s == '*' || s == '/' || s == '^')
#define IS_OPENING_BLACKET(s) (s == '(')
#define IS_CLOSING_BLACKET(s) (s == ')')
#define IS_NUMBER(s) (s >= '0' && s <= '9')
#define IS_SEPARATOR(s) (s == ' ' || s == '\t')
static char
__preproces(void)
{
	unsigned offset = 0, ind = 0;
	for (; string[ind]; ind++) {
		string[ind - offset] = string[ind];
		if (string[ind] == ' ' || string[ind] == '\t') offset++;
		if (!IS_ALPHA(string[ind]) && !IS_SIGN(string[ind]) && !IS_SEPARATOR(string[ind]) &&\
					!IS_CLOSING_BLACKET(string[ind]) && !IS_OPENING_BLACKET(string[ind])
					&& !IS_NUMBER(string[ind]) && string[ind] != '.') { // '.' - для натуральных
			return 1;
		}
	}
	string[ind - offset] = '\0';
	return 0;
}


#define IS_NEGATIVE_NUMBER(ind, arr) (!ind || (arr[ind - 1].type == SIGN ||\
			(arr[ind - 1].type == OPENING_BLACKET)))
static unsigned
__is_token(unsigned ind, unsigned ind_token)
{
	unsigned offset = 1;
	char symbol = string[ind];
	if (IS_NUMBER(symbol) || (symbol == '-' && IS_NEGATIVE_NUMBER(ind_token, arr_tokens))) {
		unsigned char t_symbol = 0;
		unsigned ind_end = ind + 1;
		char int_or_float = 1;
		for (; IS_NUMBER(string[ind_end]) || string[ind_end] == '.'; ind_end++, offset++) {
			if (string[ind_end] == '.') int_or_float = 0;
		}
		t_symbol = string[ind_end];
		string[ind_end] = '\0';
		if (int_or_float) {
#ifdef TEST
			puts("\tIS_NUMBER");
#endif
			arr_tokens[ind_token].type = NUMBER;
			arr_tokens[ind_token].data.number_int = atoi((char *)&string[ind]);
		} else {
#ifdef TEST
			puts("\tIS_NUMBER_FLOAT");
#endif
			arr_tokens[ind_token].type = NUMBER_FLOAT;
			arr_tokens[ind_token].data.number_float = atof((char *)&string[ind]);
		}
		string[ind_end] = t_symbol;
		
	} else if (IS_SIGN(symbol)) {
#ifdef TEST
		puts("\tIS_SIGN");
#endif
		arr_tokens[ind_token].type = SIGN;
		arr_tokens[ind_token].data.symbol = symbol;
	} else if (IS_OPENING_BLACKET(symbol)) {
#ifdef TEST
		puts("\tIS_OPENING_BLACKET");
#endif
		arr_tokens[ind_token].type = OPENING_BLACKET;
		arr_tokens[ind_token].data.symbol = symbol;
	} else if (IS_CLOSING_BLACKET(symbol)) {
#ifdef TEST		
		puts("\tIS_CLOSING_BLACKET");
#endif
		arr_tokens[ind_token].type = CLOSING_BLACKET;
		arr_tokens[ind_token].data.symbol = symbol;
	} else if (IS_ALPHA(symbol)) {
#ifdef TEST
		puts("\tIS_FUNC");
#endif
		unsigned ind_end = ind + 1;
		for (; IS_ALPHA(string[ind_end]) || IS_NUMBER(string[ind_end]); ind_end++, offset++);
		arr_tokens[ind_token].type = FUNC;
		arr_tokens[ind_token].data.str = (unsigned char *)calloc(offset, sizeof(unsigned char));
		if (!(arr_tokens[ind_token].data.str)) goto ERROR_IS_TOKEN;
		for (unsigned ind_start = ind; ind_start != ind_end; ind_start++)
			arr_tokens[ind_token].data.str[ind_start - ind] = string[ind_start];
		arr_tokens[ind_token].data.str[offset] = '\0';
	} else {
#ifdef TEST
		puts("\tERROR_IS_TOKEN");
#endif
		ERROR_IS_TOKEN:
		arr_tokens[ind_token].type = UNDEFINED_TYPE;
		arr_tokens[ind_token].data.str = NULL;
	}
	return offset + ind;
}


static char
__data_correctness(void)
{
	return 0;
	/*
	static const unsigned char arr_list_string[] = "sin cos s"; // между функциями обязательно 1 пробел
	unsigned char t_symbol = 0;
	for (unsigned ind = 0, ind_end = 0, ind_start = 0; arr_tokens[ind].type != END_ARR_TOKENS; ind++) {
		if (arr_tokens[ind].type == FUNC) {
			for (ind_start = ind_end = 0; arr_list_string[ind_start]; ind_start = ind_end) {
				for (; arr_list_string[ind_end] != ' ' && arr_list_string[ind_end]; ind_end++);
				t_symbol = arr_list_string[ind_end];
				arr_list_string[ind_end] = '\0';
				if (strstr(  )) break;
				arr_list_string[ind_end] = t_symbol;
				ind_end++;
			}
			if (!arr_list_string[ind_start]) return 1;
		}
	}
	*/
	return 0;
}


void
_free_arr_tokens(Token *arr_tokens, unsigned ind_end)
{
	if (!ind_end) return;
	for (unsigned ind = 0; (ind_end && ind != ind_end) || (arr_tokens[ind].type != END_ARR_TOKENS); ind++) {
		if (arr_tokens[ind].type == FUNC) {
			free(arr_tokens[ind].data.str);
		}
	}
	free(arr_tokens);
}


//enum type_token {SIGN, NUMBER, NUMBER_FLOAT, FUNC, OPENING_BLACKET, CLOSING_BLACKET, UNDEFINED_TYPE, END_ARR_TOKENS};
void
print_arr_tokens(Token *arr_tokens)
{
	for (unsigned ind = 0; arr_tokens[ind].type != END_ARR_TOKENS; ind++) {
		switch (arr_tokens[ind].type) {
			case NUMBER:
				printf("NUMBER_INT\t\t%d\n", arr_tokens[ind].data.number_int);
				break;
			case NUMBER_FLOAT:
				printf("NUMBER_FLOAT\t\t%f\n", arr_tokens[ind].data.number_float);
				break;
			case SIGN:
				printf("SIGN\t\t\t%c\n", arr_tokens[ind].data.symbol);
				break;
			case FUNC:
				printf("FUNC\t\t\t%s\n", arr_tokens[ind].data.str);
				break;
			case OPENING_BLACKET:
				printf("OPENING_BLACKET\t\t%c\n", arr_tokens[ind].data.symbol);
				break;
			case CLOSING_BLACKET:
				printf("CLOSING_BLACKET\t\t%c\n", arr_tokens[ind].data.symbol);
				break;
			case UNDEFINED_TYPE:
				puts("UNDEFINED_TYPE!!!");
				break;
		}
	}
}




#ifdef TEST
int
main()
{
	if (!get_tokens()) {
		return 1;
	}
	unsigned len_arr_tokens = 0;
	for (; arr_tokens[len_arr_tokens].type != END_ARR_TOKENS; len_arr_tokens++);
	printf("len = %u\n", len_arr_tokens);
	print_arr_tokens(arr_tokens);
	_free_arr_tokens(arr_tokens, len_arr_tokens);
	return 0;
}
#endif
