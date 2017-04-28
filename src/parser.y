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
	| func
	| prog func
	;

/* Declaration */
decl: INT ident_list ';' { }
	;

ident_list: IDENT { new_sym($1, 0, NS_NAME); }
		  | ident_list ',' IDENT { new_sym($3, 0, NS_NAME); }
		  ;

/* Functions */
/* TODO Empty code blocks? */
func: IDENT '(' ')' code_block
	;

func_lines: func_line
		  | func_lines func_line
		  ;

func_line: decl
		 | stmt
		 | code_block
		 ;

code_block: '{' { new_scope(""); } func_lines '}' { exit_scope(); } ;

/* Statements */
/* TODO Add expressions and print value with filename and lineno */
stmt: IDENT '+' IDENT ';' { printf("STATEMENT\n"); }
	;

%%

int main(){
	init_sym_table();
	return yyparse();
}

