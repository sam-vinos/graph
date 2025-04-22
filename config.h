#ifndef CONFIG_FUNCS_AND_CONSTANT
#define CONFIG_FUNCS_AND_CONSTANT

//числа в названиях не могут присутствовать ['a';'z'] & ['A';'Z'] & '_' END: \0\0
#define LIST_NAMES_FUNCS "log\0arcsin\0sin\0cos\0\0"
#define LIST_NUMBER_ARGS_FUNCS {3, 1, 2, 1, -1} // функции с нулевым колличеством аргументов не может быть

#define LIST_NAMES_CONSTANTS "pi\0x\0X\0\0"

#include "structs.h"
#include "get_tokens.h"

char config_check(void);
signed char number_args_func(unsigned char *name_func);
Token *get_copy_arr_tokens(Token *arr_tokens);

#endif
