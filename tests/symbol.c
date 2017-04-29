#include <stdio.h>
#include <stdarg.h>
#include <symbol.h>
#include <ast.h>

int main(){
	global = current = new_sym_table();
	new_scope(SCOPE_FUNC);
	set_sym(global, "i", NS_NAME, 3, 1);
	set_sym(global, "i", NS_NAME, 21, 0);
	print_table(global);

	struct scope *struct_table = new_sym_table();
	set_sym(struct_table, "member1", NS_MEMB, 3, 0);
	set_sym(struct_table, "member2", NS_MEMB, 3, 0);
	print_table(struct_table);

	exit_scope();
	set_sym(global, "var1", NS_NAME, 3, 0);
	set_sym(global, "var1", NS_NAME, 9, 1);
	int a = get_sym(global, "var1", NS_NAME);
	printf("--%d--\n", a);
	print_table(global);

	struct ast_node *b = new_node(NODE_VAR);
	ast = add_node(ast, b);
	b->u.var.name = "var1";
	b->u.var.filename = "file1.c";
	b->u.var.lineno = 4;
	set_node(global, "var1", NS_NAME, b);
	struct ast_node *c = get_node(global, "var1", NS_NAME);
	print_ast(ast, 0);
}

void yyerror(char *s, ...){
	va_list ap;
	va_start(ap, s);
	fprintf(stderr, "Error: ");
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}

