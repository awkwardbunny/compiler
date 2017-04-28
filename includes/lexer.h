#ifndef LEXER_H
#define LEXER_H
#include <tokens-manual.h>

extern int yylineno;
extern char filename[];

struct string_literal {
	unsigned char *characters;
	int length;
};

#define NT_INT 2
#define NT_UINT 3
#define NT_L 4
#define NT_UL 5
#define NT_LL 6
#define NT_ULL 7
#define NT_FP 8
#define NT_DBL 9
#define NT_LDBL 10

struct number {
	char type;
	unsigned int i;
	unsigned long l;
	unsigned long long ll;
	float f;
	double d;
	long double ld;
};

struct identifier {
	char *name;
};

typedef union {
	struct string_literal s;
	struct number n;
	struct identifier id;
	unsigned char charlit;
} YYSTYPE;

extern YYSTYPE yylval;

int yylex(void);
void yyerror(char *s, ...);
void yywarn(char *s, ...);
#endif
