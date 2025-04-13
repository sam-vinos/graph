#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "get_tokens.h"
#include "structs.h"

#include "config.h"

/*
 * нужно убрать случай обработки: si n; и случай когда -.0 1---1 правильно обрабатывается
 */


/*
enum type_token {SIGN, NUMBER, NUMBER_FLOAT, FUNC, OPENING_BLACKET, CLOSING_BLACKET, UNDEFINED_TYPE, END_ARR_TOKENS,
CONSTANT, SEPARATOR_IN_FUNC };
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
static char __preproces(void);
static unsigned __is_token(unsigned ind, unsigned ind_token);
static char __data_correctness(void);

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
		puts("JJJ");
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
		if (ind_token) _free_arr_tokens(arr_tokens, ind_token);
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
#define IS_SEPARATOR_IN_FUNC(s) (s == ',')
static char
__preproces(void)
{
	unsigned offset = 0, ind = 0;
	char last_frame_alpha = 0;
	for (; string[ind]; ind++) {
		string[ind - offset] = string[ind];
		if (IS_SEPARATOR(string[ind])) {
			if (last_frame_alpha == 1) last_frame_alpha = 2;
			offset++;
		} else if (!IS_ALPHA(string[ind]) && !IS_SIGN(string[ind]) && !IS_SEPARATOR(string[ind]) &&\
					!IS_CLOSING_BLACKET(string[ind]) && !IS_OPENING_BLACKET(string[ind])\
					&& !IS_NUMBER(string[ind]) && string[ind] != '.' &&\
					!IS_SEPARATOR_IN_FUNC(string[ind])) { // '.' - для натуральных
			return 1;
		
		}  else if (IS_ALPHA(string[ind])) {
			if (last_frame_alpha == 2) return 1;
			last_frame_alpha = 1;
		} else last_frame_alpha = 0;
	}
	string[ind - offset] = '\0';
	return 0;
}


#define IS_NEGATIVE_NUMBER(ind, arr, ind_str, string) ((!ind || (arr[ind - 1].type == SIGN ||\
			(arr[ind - 1].type == OPENING_BLACKET))) && IS_NUMBER(string[ind_str + 1]))
static unsigned
__is_token(unsigned ind, unsigned ind_token)
{
	unsigned offset = 1;
	char symbol = string[ind];
	if (IS_NUMBER(symbol) || (symbol == '-' && IS_NEGATIVE_NUMBER(ind_token, arr_tokens, ind, string))) {
		unsigned char t_symbol = 0;
		unsigned ind_end = ind + 1;
		char int_or_float = 1;
		for (; IS_NUMBER(string[ind_end]) || string[ind_end] == '.'; ind_end++, offset++) {
			if (string[ind_end] == '.') {
				if (!int_or_float) goto ERROR_IS_TOKEN;
				int_or_float = 0;
			}
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
		for (; IS_ALPHA(string[ind_end]); ind_end++, offset++);
		arr_tokens[ind_token].type = FUNC;
		arr_tokens[ind_token].data.str = (unsigned char *)calloc(offset, sizeof(unsigned char));
		if (!(arr_tokens[ind_token].data.str)) goto ERROR_IS_TOKEN;
		for (unsigned ind_start = ind; ind_start != ind_end; ind_start++)
			arr_tokens[ind_token].data.str[ind_start - ind] = string[ind_start];
		arr_tokens[ind_token].data.str[offset] = '\0';
	} else if (IS_SEPARATOR_IN_FUNC(symbol)) {
#ifdef TEST
		puts("IS_SEPARATOR_IN_FUNC");
#endif
		arr_tokens[ind_token].type = SEPARATOR_IN_FUNC;
		arr_tokens[ind_token].data.symbol = ',';
	} else {
		ERROR_IS_TOKEN:
#ifdef TEST
		puts("\tERROR_IS_TOKEN");
#endif
		arr_tokens[ind_token].type = UNDEFINED_TYPE;
		arr_tokens[ind_token].data.str = NULL;
	}
	return offset + ind;
}


static char
__data_correctness(void) //проверяет на наличие ошибок с (), реальными названифми функций и констатнт
{
	//unsigned char list_name_funcs[] = LIST_NAMES_FUNC;
	//unsigned char list_name_constant[] = LIST_NAMES_CONSTANT;
	unsigned char *point_list = NULL;
	unsigned correctnes_blacket = 0;
	for (unsigned ind = 0, ind_start = 0; arr_tokens[ind].type != END_ARR_TOKENS; ind++) {
		switch (arr_tokens[ind].type) {
			case OPENING_BLACKET:
				correctnes_blacket++;
				break;
			case CLOSING_BLACKET:
				if (!correctnes_blacket) goto NOT_CORRECT;
				correctnes_blacket--;
				break;
			case FUNC:
				point_list = list_names_funcs;
				TWO_ITERATION:
				while (point_list[ind_start]) {
					if (!strcmp((char *)&point_list[ind_start], (char *)arr_tokens[ind].data.str))
						break;
					while (point_list[ind_start++]);
				}
				if (!point_list[ind_start]) {
					if (point_list == list_names_constants) goto NOT_CORRECT;
					point_list = list_names_constants;
					ind_start = 0;
					goto TWO_ITERATION;
				} else if (point_list == list_names_constants) {
					arr_tokens[ind].type = CONSTANT;
				}
				ind_start = 0;
				break;
			default:
				break;
		}
	}
	if (correctnes_blacket) {
		NOT_CORRECT:
		return 1;
	}
	return 0;
}


void
_free_arr_tokens(Token *arr_tokens, unsigned ind_end)
{
#ifdef TEST
	puts("FREE_ARR_TOKENS");
	printf("ind_end = %d\n", ind_end);
#endif
	if (!arr_tokens) return;
	for (unsigned ind = 0; (ind_end && ind != ind_end) || (!ind_end &&\
				arr_tokens[ind].type != END_ARR_TOKENS); ind++) {
#ifdef TEST
		printf("ind = %d\n", ind);
#endif
		if ((arr_tokens[ind].type == FUNC || arr_tokens[ind].type == CONSTANT) &&\
				!arr_tokens[ind].data.str) {
#ifdef TEST
			puts("HHH");
#endif
			free(arr_tokens[ind].data.str);
#ifdef TEST
			puts("hhh");
#endif
		}
	}
#ifdef TEST
	puts("FREE_ARR_TOKENS");
#endif
	free(arr_tokens);
}


//enum type_token {SIGN, NUMBER, NUMBER_FLOAT, FUNC, OPENING_BLACKET, CLOSING_BLACKET, UNDEFINED_TYPE, END_ARR_TOKENS,
//CONSTANT, SEPARATOR_IN_FUNC};
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
			case CONSTANT:
				printf("CONSTANT\t\t%s\n", arr_tokens[ind].data.str);
				break;
			case OPENING_BLACKET:
				printf("OPENING_BLACKET\t\t%c\n", arr_tokens[ind].data.symbol);
				break;
			case CLOSING_BLACKET:
				printf("CLOSING_BLACKET\t\t%c\n", arr_tokens[ind].data.symbol);
				break;
			case SEPARATOR_IN_FUNC:
				printf("SEPARATOR_IN_FUNC\t%c\n", arr_tokens[ind].data.symbol);
				break;
			case UNDEFINED_TYPE:
				puts("UNDEFINED_TYPE!!!");
				break;
		}
	}
}


unsigned
get_len_arr_tokens(Token *arr_tokens)
{
	unsigned len = 0;
	for (; arr_tokens[len].type != END_ARR_TOKENS; len++);
	return len;
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
	//_free_arr_tokens(arr_tokens, len_arr_tokens);
	return 0;
}
#endif
