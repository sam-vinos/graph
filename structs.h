#ifndef STRUCTS
#define STRUCTS


enum type_token {SIGN, NUMBER, NUMBER_FLOAT, FUNC, BLACKETS};
typedef struct token_t {
	union {
		unsigned char s;
		signed number_int;
		float number_float;
		unsigned char *str;
	} data;
	char type;
} Token;


/*
typedef struct stack_t {
	Token data;
	struct stack_t *pointer;
} Stack;
*/

void pusch(Token *data);
Token *pop(void);


#endif
