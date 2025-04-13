#ifndef CONFIG_FUNCS_AND_CONSTANT
#define CONFIG_FUNCS_AND_CONSTANT

//числа в названиях не могут присутствовать ['a';'z'] & ['A';'Z'] & '_'
//#define LIST_NAMES_FUNC "log\0arcsin\0sin\0cos\0\0"
unsigned char list_names_funcs[] = "log\0arcsin\0sin\0cos\0\0";
signed char list_number_args_funcs[] = {2, 1, 1, 1};

//#define LIST_NAMES_CONSTANT "pi\0x\0X\0\0"
unsigned char list_names_constants[] = "pi\0x\0X\0\0";

#endif
