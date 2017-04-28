#include <stdio.h>
#include <stdarg.h>
#include <symbol.h>

int main(){
	init_sym_table();
	new_sym("i", NS_NAME, 3);
	new_sym("k", NS_TAGS, 43);
	new_sym("j", NS_MEMB, -21);
	new_sym("j", NS_MEMB, 21);
	new_scope("f");
	print_table();
	new_sym("a", NS_LABL, 103);
	new_scope("g");
	new_sym("b", NS_NAME, -2);
	new_scope("");
	new_sym("variable", NS_NAME, 0);
	print_table();
	exit_scope();
	exit_scope();
	exit_scope();
	exit_scope();
	exit_scope();
	exit_scope();
	print_table();
}

void yyerror(char *s, ...){
	va_list ap;
	va_start(ap, s);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, s, ap);
}

