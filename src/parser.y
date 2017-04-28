%{
#include <stdio.h>
#include <lexer.h>
#include <symbol.h>

int yylex();
%}

%union {
	struct string_literal s;
	struct number n;
	struct identifier id;
	unsigned char charlit;
}

%type<id.name> IDENT

%token TOKEOF IDENT CHARLIT STRING NUMBER INDSEL PLUSPLUS MINUSMINUS SHL SHR LTEQ GTEQ EQEQ NOTEQ LOGAND LOGOR ELLIPSIS TIMESEQ DIVEQ MODEQ PLUSEQ MINUSEQ SHLEQ SHREQ ANDEQ OREQ XOREQ AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO DOUBLE ELSE ENUM EXTERN FLOAT FOR GOTO IF INLINE INT LONG REGISTER RESTRICT RETURN SHORT SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE _BOOL _COMPLEX _IMAGINARY

%%

prog: decl
	| prog decl
	;

decl: INT IDENT ';' { new_sym($2, NS_NAME, 0); print_table(); }
	;

%%

int main(){
	init_sym_table();
	return yyparse();
}

