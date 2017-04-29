#include <stdio.h>
#include <stdarg.h>
#include <symbol.h>

int main(){
	init_sym_table();
	new_sym("i", NS_NAME, 3);
	int a = get_sym("i", NS_NAME);
	set_sym("i", NS_NAME, a+1);
	a = get_sym("i", NS_NAME);
	int *b = where_sym("i", NS_NAME);
	if(b){
		printf("blah%d\n", *b);
		*b += 20;
	}else{
		printf("bleh\n");
	}
	printf("%d\n", a);
	print_table();
}

void yyerror(char *s, ...){
	va_list ap;
	va_start(ap, s);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, s, ap);
}

