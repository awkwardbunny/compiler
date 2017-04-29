#include <stdio.h>
#include <stdarg.h>
#include <symbol.h>
#include <ast.h>

int main(){
	global = current = new_sym_table();
	new_scope(FUNC);
	set_sym(global, "i", NAME, 3, 1);
	set_sym(global, "i", NAME, 21, 0);
	print_table(global);

	struct scope *struct_table = new_sym_table();
	set_sym(struct_table, "member1", MEMB, 3, 0);
	set_sym(struct_table, "member2", MEMB, 3, 0);
	print_table(struct_table);

	exit_scope();
	set_sym(global, "var1", NAME, 3, 0);
	set_sym(global, "var1", NAME, 9, 1);
	int a = get_sym(global, "var1", NAME);
	printf("--%d--\n", a);
	print_table(global);

	struct ast_node *b = new_node(IDENT);
	set_node(global, "var2", NAME, b);
	struct ast_node *c = get_node(global, "var1", NAME);
}

void yyerror(char *s, ...){
	va_list ap;
	va_start(ap, s);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}

