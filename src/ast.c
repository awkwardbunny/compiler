#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <symbol.h>
#include <ast.h>

struct ast_node *new_node(enum node_type nt){
	struct ast_node *pan = malloc(sizeof(struct ast_node));
	if(!pan){
		yyerror("Cannot allocate memory for AST node");
		return NULL;
	}
	pan->type = nt;
	pan->next = NULL;
	return pan;
}

struct ast_node *dup_node(struct ast_node *n){
	struct ast_node *m = malloc(sizeof(struct ast_node));
	memcpy(m, n, sizeof(struct ast_node));
	return m;
}

void print_ast(struct ast_node *root, int level){
	for(struct ast_node *a = root; a; a = a->next){
		for(int i = 0; i < level; i++) printf("  ");
		switch(a->type){
			case AST_IDENT:
				printf("IDENT %s\n", a->u.ident.name);
				print_ast(a->u.var.ptr, level+1);
				break;
			case AST_SCALAR:
				printf("SCALAR %s\n", (a->u.scalar.type == SCLR_INT)?"INT":(a->u.scalar.type==SCLR_SHORT)?"SHORT":"CHAR");
				break;
			case AST_VAR:
				printf("VAR %s\n", a->u.var.name);
				print_ast(a->u.var.ptr, level+1);
				break;
			case AST_ARRAY:
				printf("ARRAY SIZE %d OF\n", a->u.array.size);
				print_ast(a->u.array.ptr, level+1);
				break;
			case AST_PTR:
				printf("POINTER TO\n");
				print_ast(a->u.ptr.ptr, level+1);
				break;
			default:
				printf("%s\n", "some kind of AST node");
		}
	}
}
