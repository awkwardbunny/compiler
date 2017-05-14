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

void print_ast(struct ast_node *root, int level){
	/*
	for(struct ast_node *a = root; a; a = a->next){
		switch(root->type){
			for(int i = 0; i < level; i++) printf("\t");
			case NODE_IDENT:
				printf("IDENT %s\n", root->u.ident);
				break;
			default:
				printf("%s\n", "some kind of AST node");
		}
	}
	*/
}
