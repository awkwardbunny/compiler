#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <symbol.h>
#include <ast.h>

struct ast_node *new_node(enum node_type nt){
	struct ast_node *pan = calloc(1, sizeof(struct ast_node));
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
	for(int i = 0; i < level; i++) printf("  ");
	if(root == NULL){
		printf("NULL\n");
		return;
	}
	for(struct ast_node *a = root; a; a = a->next){
		switch(a->type){
			case AST_NUM:
				printf("CONSTANT: (type=");
				switch(a->u.num.type){
					case NT_INT:
						printf("int)%d\n", a->u.num.val.i);
						break;
					default:
						printf("unknown)\n");
				}
				break;
			case AST_ASGN:
				printf("ASSIGNMENT\n");
				print_ast(a->u.asgn.lval, level+1);
				print_ast(a->u.asgn.rval, level+1);
				break;
			case AST_BINOP:
				printf("BINARY OP ");
				switch(a->u.binop.op){
					case BINOP_LT:
						printf("<\n");
						break;
					default:
						printf("(Unknown)\n");
				}
				print_ast(a->u.binop.left, level+1);
				print_ast(a->u.binop.right, level+1);
				break;
			case AST_FOR:
				printf("FOR\n");
				printf("INIT:\n");
				print_ast(a->u.nfor.init, level+1);
				printf("COND:\n");
				print_ast(a->u.nfor.cond, level+1);
				printf("BODY:\n");
				print_ast(a->u.nfor.body, level+1);
				printf("INCR:\n");
				print_ast(a->u.nfor.inc, level+1);
				break;
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
