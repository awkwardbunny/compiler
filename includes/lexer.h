#ifndef LEXER_H
#define LEXER_H

extern int yylineno;
extern char filename[];

int yylex(void);
void yyerror(char *s, ...);
void yywarn(char *s, ...);
#endif
