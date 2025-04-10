#ifndef STRUCTS
#define STRUCTS


enum type_token {
	SIGN,
	NUMBER,
	NUMBER_FLOAT,
	CONSTANT,
	FUNC,
	OPENING_BLACKET,
	CLOSING_BLACKET,
	

	UNDEFINED_TYPE,
	END_ARR_TOKENS
};
typedef struct token_t {
	char type;
	union {
		unsigned char symbol;
		signed number_int;
		float number_float;
		unsigned char *str;
	} data;
} Token;


/*
typedef struct stack_t {
	Token data;
	struct stack_t *pointer;
} Stack;


void pusch(Token *data);
Token *pop(void);
*/

#endif
