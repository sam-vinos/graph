#include <stdio.h>
#include <string.h>

#include "structs.h"
#include "config.h"
#include "get_tokens.h"

//разделитель функции должен находиться в скобках перед которыми находится функция; минимальное колличество
//токенов 1 и при этом это всегда число; если токенов больше чем 1 то последовательностей по типу "число действие число"
//должно быть нечетное колличество, при это "число действие скобка число действие число скобка" на первом уровне будет
//считать эту последовательность так же как "число действие число"; на всех не нулевых уровня она будет считать
//степень вложенности скобок; случай с функцией особый случай который надо обработать по особенному;
//(а нужно ли еще счиитать колличество допустимых аргументов???)

/*
enum type_token {SIGN, NUMBER, NUMBER_FLOAT, FUNC, OPENING_BLACKET, CLOSING_BLACKET, UNDEFINED_TYPE, END_ARR_TOKENS,
CONSTANT, SEPARATOR_IN_FUNC };
*/

static int __get_max_nesting(Token *arr_tokens); //так же проверяет синтаксис функций
static char __is_not_true_func(Token *arr_tokens, unsigned ind);
//static unsigned char __get_number

char
input_check(Token *arr_tokens)
{
	int max_nesting = __get_max_nesting(arr_tokens);
	if (max_nesting == -1) {
		//ERROR_INPUT_CHECK:
		free_arr_tokens(arr_tokens);
		return 1;
	}
	printf("max_nesting = %d\n", max_nesting);
	return 0;
}


static int
__get_max_nesting(Token *arr_tokens) //добавить еще функцию для проверки праивльного нахождения разделителя аргументов
{
	int max_nesting = 0, nesting = 0;
	//unsigned char number_separator = 0;
	for (unsigned ind = 0/*, ind_func = 0*/; arr_tokens[ind].type != END_ARR_TOKENS; ind++) {
		switch (arr_tokens[ind].type) {
			case OPENING_BLACKET:
				nesting++;
				if (max_nesting < nesting) max_nesting++;
				break;
			case CLOSING_BLACKET:
				nesting--;
				break;
			case FUNC:
				if (__is_not_true_func(arr_tokens, ind)) {
					max_nesting = -1;
					fprintf(stderr, "%s\n", "Incorrect function syntax");
					goto ERROR_GET_MAX_NESTING;
				}
				break;
		}
	}
	if (nesting) max_nesting = -1;
	ERROR_GET_MAX_NESTING: /// нужно лишь для того что бы выйти из цикла
	return max_nesting;
}


/*
#define LIST_NAMES_FUNCS "log\0arcsin\0sin\0cos\0\0"
#define LIST_NUMBER_ARGS_FUNCS {2, 1, 1, 1, -1}

#define LIST_NAMES_CONSTANTS "pi\0x\0X\0\0"
*/
static char
__is_not_true_func(Token *arr_tokens, unsigned ind)
{
	if (arr_tokens[ind + 1].type != OPENING_BLACKET) goto ERROR_IS_NOT_TRUE_FUNC;
	unsigned char list_names_funcs[] = LIST_NAMES_FUNCS;
	signed char list_number_args_funcs[] = LIST_NUMBER_ARGS_FUNCS;
	signed char number_args = 0;
	unsigned ind_func = 0;
	///search func
	for (signed ind_start_name_func = -1; list_number_args_funcs[ind_func] != -1; ind_func++) {
		ind_start_name_func++;
		if (!strcmp((char *)&list_names_funcs[ind_start_name_func], (char *)arr_tokens[ind].data.str)) {
			number_args = list_number_args_funcs[ind_func];
			ind += 2;
			break;
		}
		while (list_names_funcs[++ind_start_name_func]);
	}
	if (list_number_args_funcs[ind_func] == -1) goto ERROR_IS_NOT_TRUE_FUNC;
	///end search
	number_args--;
	for (signed nesting = 1; nesting && arr_tokens[ind].type != END_ARR_TOKENS; ind++) {
		switch (arr_tokens[ind].type) {
			case OPENING_BLACKET:
				nesting++;
				break;
			case CLOSING_BLACKET:
				nesting--;
				break;
			case SEPARATOR_IN_FUNC:
				if (nesting == 1) number_args--;
				else goto ERROR_IS_NOT_TRUE_FUNC;
				break;
		}
	}
	if (number_args) {
		ERROR_IS_NOT_TRUE_FUNC:
		return 1;
	}
	return 0;
}
