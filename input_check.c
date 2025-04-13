#include "structs.h"
#include <stdio.h>


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

char
input_check(Token *arr_tokens)
{
	unsigned nesting = 0;
	unsigned fixten_nesting = 1;
	unsigned max_nesting = 0;
	unsigned ind = 0;
	for (; arr_tokens[ind].type == END_ARR_TOKENS; ind++) {
		switch (arr_tokens[ind].type) {
			case UNDEFINED_TYPE:
				fprintf(stderr, "%s\n", "Incorrect token sequence");
				return 1;
		}
	}
	return 0;
}
