#include <stdio.h>
#include <ctype.h>
#include <lexer.h>

void printchar(unsigned char cin){
	char c;
	switch(cin){
		case '\0': c = '0'; goto p;
		case '\a': c = 'a'; goto p;
		case '\b': c = 'b'; goto p;
		case '\f': c = 'f'; goto p;
		case '\n': c = 'n'; goto p;
		case '\r': c = 'r'; goto p;
		case '\t': c = 't'; goto p;
		case '\v': c = 'v'; goto p;
		case '\'': c = '\''; goto p;
		case '"': c = '"'; goto p;
		case '\\': c = '\\';
		p: printf("\\%c", c); break;
		default: 
			if(isprint(cin)){
				printf("%c", cin);
			}else{
				printf("\\%03o", cin);
			}
	}

}

int main(){
	int t;
	while(t = yylex()){
		printf("%s\t%d\t", filename, yylineno);
		switch(t){
			case IDENT: printf("IDENT\t%s\n", yylval.id.name); break;
			case CHARLIT:
				printf("CHARLIT\t");
				printchar(yylval.charlit);
				printf("\n");
				break;
			case STRING :
				printf("STRING\t");
				for(int i = 0; i < yylval.s.length; i++){
					printchar(yylval.s.characters[i]);
				}
				printf("\n");
				break;
			case NUMBER :
				printf("NUMBER\t");
				switch(yylval.n.type){
					case NT_INT:
						printf("INTEGER\t%d\tINT\n", yylval.n.i); break;
					case NT_UINT:
						printf("INTEGER\t%u\tUNSIGNED,INT\n", yylval.n.i); break;
					case NT_L:
						printf("INTEGER\t%ld\tLONG\n", yylval.n.l); break;
					case NT_UL:
						printf("INTEGER\t%lu\tUNSIGNED,LONG\n", yylval.n.l); break;
					case NT_LL:
						printf("INTEGER\t%lld\tLONGLONG\n", yylval.n.ll); break;
					case NT_ULL:
						printf("INTEGER\t%llu\tUNSIGNED,LONGLONG\n", yylval.n.ll); break;
					case NT_FP:
						printf("REAL\t%g\tFLOAT\n", yylval.n.f); break;
					case NT_DBL:
						printf("REAL\t%g\tDOUBLE\n", yylval.n.d); break;
					case NT_LDBL:
						printf("REAL\t%Lg\tLONGDOUBLE\n", yylval.n.ld);
				}
				break;
			// Tabs needed to match ltests/ltests.output with diff
			case INDSEL :  printf("INDSEL\t\n"); break;
			case PLUSPLUS:  printf("PLUSPLUS\t\n"); break;
			case MINUSMINUS:  printf("MINUSMINUS\t\n"); break;
			case SHL :  printf("SHL\t\n"); break;
			case SHR :  printf("SHR\t\n"); break;
			case LTEQ :  printf("LTEQ\t\n"); break;
			case GTEQ :  printf("GTEQ\t\n"); break;
			case EQEQ :  printf("EQEQ\t\n"); break;
			case NOTEQ :  printf("NOTEQ\t\n"); break;
			case LOGAND :  printf("LOGAND\t\n"); break;
			case LOGOR :  printf("LOGOR\t\n"); break;
			case ELLIPSIS :  printf("ELLIPSIS\t\n"); break;
			case TIMESEQ :  printf("TIMESEQ\t\n"); break;
			case DIVEQ :  printf("DIVEQ\t\n"); break;
			case MODEQ :  printf("MODEQ\t\n"); break;
			case PLUSEQ :  printf("PLUSEQ\t\n"); break;
			case MINUSEQ :  printf("MINUSEQ\t\n"); break;
			case SHLEQ :  printf("SHLEQ\t\n"); break;
			case SHREQ :  printf("SHREQ\t\n"); break;
			case ANDEQ :  printf("ANDEQ\t\n"); break;
			case OREQ :  printf("OREQ\t\n"); break;
			case XOREQ :  printf("XOREQ\t\n"); break;

			case AUTO : printf("AUTO\t\n"); break;
			case BREAK : printf("BREAK\t\n"); break;
			case CASE : printf("CASE\t\n"); break;
			case CHAR : printf("CHAR\t\n"); break;
			case CONST : printf("CONST\t\n"); break;
			case CONTINUE : printf("CONTINUE\t\n"); break;
			case DEFAULT : printf("DEFAULT\t\n"); break;
			case DO : printf("DO\t\n"); break;
			case DOUBLE : printf("DOUBLE\t\n"); break;
			case ELSE : printf("ELSE\t\n"); break;
			case ENUM : printf("ENUM\t\n"); break;
			case EXTERN : printf("EXTERN\t\n"); break;
			case FLOAT : printf("FLOAT\t\n"); break;
			case FOR : printf("FOR\t\n"); break;
			case GOTO : printf("GOTO\t\n"); break;
			case IF : printf("IF\t\n"); break;
			case INLINE : printf("INLINE\t\n"); break;
			case INT : printf("INT\t\n"); break;
			case LONG : printf("LONG\t\n"); break;
			case REGISTER : printf("REGISTER\t\n"); break;
			case RESTRICT : printf("RESTRICT\t\n"); break;
			case RETURN : printf("RETURN\t\n"); break;
			case SHORT : printf("SHORT\t\n"); break;
			case SIGNED : printf("SIGNED\t\n"); break;
			case SIZEOF : printf("SIZEOF\t\n"); break;
			case STATIC : printf("STATIC\t\n"); break;
			case STRUCT : printf("STRUCT\t\n"); break;
			case SWITCH : printf("SWITCH\t\n"); break;
			case TYPEDEF : printf("TYPEDEF\t\n"); break;
			case UNION : printf("UNION\t\n"); break;
			case UNSIGNED : printf("UNSIGNED\t\n"); break;
			case VOID : printf("VOID\t\n"); break;
			case VOLATILE : printf("VOLATILE\t\n"); break;
			case WHILE : printf("WHILE\t\n"); break;
			case _BOOL : printf("_BOOL\t\n"); break;
			case _COMPLEX : printf("_COMPLEX\t\n"); break;
			case _IMAGINARY : printf("_IMAGINARY\t\n"); break;
			default: printf("%c\t\n", t);
		}
	}
}
