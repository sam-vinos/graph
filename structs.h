#ifndef STRUCTS
#define STRUCTS


typedef struct token_t {
	union data {
		char s;
		signed number_int;
		float number_float;
	};
	char type;
} Token;



typedef struct stack_t {
	Token data;
	struct stack_t *pointer;
} Stack;

void pusch(Token data);
Token pop(void);


#endif
