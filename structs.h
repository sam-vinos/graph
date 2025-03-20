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


#endif
