#include <stdio.h>
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

void add_node(struct ast_node *a, struct ast_node *b){
	while(a->next) a = a->next;
	a->next = b;
}
