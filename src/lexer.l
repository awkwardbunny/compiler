%{
#include <errno.h>
#include <stdarg.h>
#include <math.h>
#include <limits.h>
#include <lexer.h>

//delete below line for ltests
#include <parser.tab.h>

#ifndef YY_YY_INCLUDES_PARSER_TAB_H_INCLUDED
#include <tokens-manual.h>
typedef union {
	struct string_literal s;
	struct number n;
	struct identifier id;
	unsigned char charlit;
} YYSTYPE;
YYSTYPE yylval;
#endif

char filename[64] = "<stdin>";
void mystrcat(char *str, int l);
void processint(char *, int, int);
int mode = 0;
%}

%x SC_CHAR
%x SC_STR
%x SC_ESC
%x SC_COMMENT
%%

 /* LINE NUMBERS */
#[ ]?[0-9]+[ ]\"[^ \"]+\"([ ][1-4])*\n { sscanf(yytext, "# %d \"%62[^\"]+\"", &yylineno, filename); }
\n                                    { }

 /* WHITESPACES */
[\t\v\f ]+     { }

 /* COMMENTS */
\/\*            { BEGIN(SC_COMMENT); }
<SC_COMMENT>.      { }
<SC_COMMENT>\n     { }
<SC_COMMENT>\*\/   { BEGIN(0); }
\/\/.*\n        { }

 /* KEYWORDS */
"auto"       { return AUTO; }
"break"      { return BREAK; }
"case"       { return CASE; }
"char"       { return CHAR; }
"const"      { return CONST; }
"continue"   { return CONTINUE; }
"default"    { return DEFAULT; }
"do"         { return DO; }
"double"     { return DOUBLE; }
"else"       { return ELSE; }
"enum"       { return ENUM; }
"extern"     { return EXTERN; }
"float"      { return FLOAT; }
"for"        { return FOR; }
"goto"       { return GOTO; }
"if"         { return IF; }
"inline"     { return INLINE; }
"int"        { return INT; }
"long"       { return LONG; }
"register"   { return REGISTER; }
"restrict"   { return RESTRICT; }
"return"     { return RETURN; }
"short"      { return SHORT; }
"signed"     { return SIGNED; }
"sizeof"     { return SIZEOF; }
"static"     { return STATIC; }
"struct"     { return STRUCT; }
"switch"     { return SWITCH; }
"typedef"    { return TYPEDEF; }
"union"      { return UNION; }
"unsigned"   { return UNSIGNED; }
"void"       { return VOID; }
"volatile"   { return VOLATILE; }
"while"      { return WHILE; }
"_Bool"      { return _BOOL; }
"_Complex"   { return _COMPLEX; }
"_Imaginary" { return _IMAGINARY; }

 /* SEPARATORS */
[()\[\]{},;:] { return yytext[0]; }
"..."        { return ELLIPSIS; }
  /* C99 DIGRAPHS */
"<:"         { return '['; }
":>"         { return ']'; }
"<%"         { return '{'; }
"%>"         { return '}'; }
"%:"         { return '#'; }

 /* OPERATORS */
 /** SIMPLE */
[!%^&*\-~+=|.<>\/?] { return yytext[0]; }
 /** COMPOUND */
"+="         { return PLUSEQ; }
"-="         { return MINUSEQ; }
"*="         { return TIMESEQ; }
"/="         { return DIVEQ; }
"%="         { return MODEQ; }
"<<="        { return SHLEQ; }
">>="        { return SHREQ; }
"&="         { return ANDEQ; }
"^="         { return XOREQ; }
"|="         { return OREQ; }
 /** OTHER */
"->"         { return INDSEL; }
"++"         { return PLUSPLUS; }
"--"         { return MINUSMINUS; }
"<<"         { return SHL; }
">>"         { return SHR; }
"<="         { return LTEQ; }
">="         { return GTEQ; }
"=="         { return EQEQ; }
"!="         { return NOTEQ; }
"&&"         { return LOGAND; }
"||"         { return LOGOR; }

 /* IDENTIFIERS */
[a-zA-Z_][0-9a-zA-Z_]* {
	yylval.id.name = malloc(yyleng);
	memcpy(yylval.id.name, yytext, yyleng);
	return IDENT; 
}

 /* CONSTANTS */
 /** INTEGER */
[1-9][0-9]*(((l|L|ll|LL)[uU]?)|[uU](l|L|ll|LL)?)? {
    processint(yytext, yyleng, 10);
    return NUMBER;
}

0[0-7]*(((l|L|ll|LL)[uU]?)|[uU](l|L|ll|LL)?)? {
    processint(yytext, yyleng, 8);
    return NUMBER;
}

0[xX][0-9a-fA-F]*(((l|L|ll|LL)[uU]?)|[uU](l|L|ll|LL)?)? {
    processint(yytext, yyleng, 16);
    return NUMBER;
}

 /** FLOAT */
 /* TODO [NOT CRITICAL] */
(([0-9]+[eE]|0[xX]([0-9a-fA-F]+\.[0-9a-fA-F]*|\.?[0-9a-fA-F]+)[pP])[+-]?[0-9]+|(\.[0-9]+|[0-9]+\.[0-9]*)([eE][+-]?[0-9]+)?)[fFlL]? {
	switch(yytext[yyleng-1]){
		case 'f':
		case 'F':
			yytext[--yyleng] = '\0';
			yylval.n.f = strtof(yytext, NULL);
			if(yylval.n.f == HUGE_VAL)
				yywarn("Value too large. %s", strerror(errno));
			yylval.n.type = NT_FP;
			break;
		case 'l':
		case 'L':
			yytext[--yyleng] = '\0';
			yylval.n.ld = strtold(yytext, NULL);
			if(yylval.n.ld == HUGE_VAL)
				yywarn("Value too large. %s", strerror(errno));
			yylval.n.type = NT_LDBL;
			break;
		default:
			yylval.n.d = strtod(yytext, NULL);
			if(yylval.n.d == HUGE_VAL)
				yywarn("Value too large. %s", strerror(errno));
			yylval.n.type = NT_DBL;
	}
	return NUMBER;
}

 /** CHAR */
 /* TODO [Review] */
L?\' {
	yylval.charlit = 0;
	mode = CHARLIT;
	BEGIN(SC_CHAR);
}
<SC_CHAR>[^\n\'\\]+ {
	if(!yylval.charlit)
		yylval.charlit = yytext[0];
	if(yyleng > 1)
		yywarn("Unsupported multibyte character literal truncated to first byte");
}
<SC_CHAR>\\ {
	BEGIN(SC_ESC);
}
<SC_CHAR>\n {
	yyerror("Unexpected newline");
	exit(-1);
}
<SC_CHAR>\' {
	BEGIN(0);
	mode = 0;
	return CHARLIT;
}

 /** STRING */
 /* TODO [Review] */
L?\" {
	yylval.s.length = 0;
	mode = STRING;
	BEGIN(SC_STR);
}
<SC_STR>[^\n\"\\]* {
	mystrcat(yytext, yyleng);
}
<SC_STR>\\ {
	BEGIN(SC_ESC);
}
<SC_STR>\n {
	yyerror("Unexpected newline");
	exit(-1);
}
<SC_STR>\" {
	mystrcat(yytext+1, 1);
	yylval.s.length--;
	BEGIN(0);
	mode = 0;
	return STRING;
}

 /* ESCAPE CODES */
 /** CHARACTER */
<SC_ESC>[ntbrfv\\'"a?] {
	char x;
	switch(yytext[0]){
		case 'n': x = '\n'; break;
		case 't': x = '\t'; break;
		case 'b': x = '\b'; break;
		case 'r': x = '\r'; break;
		case 'f': x = '\f'; break;
		case 'v': x = '\v'; break;
		case '\\': x = '\\'; break;
		case '\'': x = '\''; break;
		case '"': x = '"'; break;
		case 'a': x = '\a'; break;
		case '?': x = '?'; break;
	}
	switch(mode){
		case STRING:
			mystrcat(&x, 1);
			BEGIN(SC_STR);
			break;
		case CHARLIT:
			yylval.charlit = x;
			BEGIN(SC_CHAR);
	}
}
 /** OCTAL */
<SC_ESC>[0-7]{1,3} {
	long int l = strtol(yytext, NULL, 8);
	if(l == LONG_MIN || l == LONG_MAX){
		yywarn("Value too large. %s", strerror(errno));
	}
	char o = (char)l;
	switch(mode){
		case STRING:
			mystrcat(&o, 1);
			BEGIN(SC_STR);
			break;
		case CHARLIT:
			yylval.charlit = o;
			BEGIN(SC_CHAR);
	}
}

 /** HEX */
<SC_ESC>[xX][0-9a-fA-F]+ {
	long int l = strtol(yytext+1, NULL, 16);
	unsigned char h = (unsigned char)l;
	if(l > 0xff){
		yywarn("Hex escape sequence \\%s out of range", yytext);
		h = 0xff;
	}
	switch(mode){
		case STRING:
			mystrcat(&h, 1);
			BEGIN(SC_STR);
			break;
		case CHARLIT:
			yylval.charlit = h;
			BEGIN(SC_CHAR);
			break;
	}
}

%%

/* TODO Review */
void mystrcat(char *str, int l){
	if(yylval.s.length){
		char *new = (char *)realloc(yylval.s.characters, yylval.s.length+l);
		if(!new){
			yyerror("Error: realloc failed in mystrcat()! %s", strerror(errno));
			exit(-1);
		}
		yylval.s.characters = new;
	}else{
		yylval.s.characters = (char *)malloc(l);
	}
	memcpy(yylval.s.characters+yylval.s.length, str, l);
	yylval.s.length += l;
}

void processint(char *str, int len, int base){
	int uns = 0;
	int longs = 0;
	for(int i = 0; i < len; i++){
        if(str[i] == 'U' || str[i] == 'u'){
			uns++;
			str[i] = 0;
        }else if(str[i] == 'L' || str[i] == 'l'){
			longs++;
			str[i] = 0;
		}
	}
	
	long long int ll = strtoll(str, NULL, base);
	if(ll == LLONG_MIN || ll == LLONG_MAX){
		 yywarn("Value too large. %s", strerror(errno));
	}

	if(longs == 1){
		yylval.n.type = uns?NT_UL:NT_L;
		yylval.n.l = ll;
	}else if(longs == 2){
		yylval.n.type = uns?NT_ULL:NT_LL;
		yylval.n.ll = ll;
	}else{
		yylval.n.type = uns?NT_UINT:NT_INT;
		yylval.n.i = ll;
	}
}

void yyerror(char *s, ...){
	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "%s:%d:Error:", filename, yylineno);
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}

void yywarn(char *s, ...){
	va_list ap;
	va_start(ap, s);

	fprintf(stderr, "%s:%d:Warning:", filename, yylineno);
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}
