#include <stdio.h>
#include <stdlib.h>

#include "gettokens.h"
#include "structs.h"


#define MIN_SIZE_STRING 10


/* typedef struct token_t {
	union data {
		char s;
		signed number_int;
		float number_float;
	};
	char type;
} Token; */


unsigned char *
input(void)
{
	unsigned char *res = (char *)calloc(MIN_SIZE_STRING, sizeof(char));
	unsigned counter = 0;
	unsigned len = MIN_SIZE_STRING;
	for (unsigned char s = 0; (s = getchar()) != '\n';) {
		res[counter++] = s;
		if (counter == len) {
			len += MIN_SIZE_STRING;
			res = (char *)realloc(res, len);
		}
	}
	res = (char *)realloc(res, counter + 1);
	res[counter] = 0;
	return res;
}


Token *
get_arr_tokens(unsigned char *string)
{
	Token *arr = 0;
	free(string);
	return arr;
}
