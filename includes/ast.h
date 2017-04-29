#ifndef AST_H
#define AST_H

enum namespace_type {
	NAME=0,
	TAGS,
	MEMB,
	LABL
};

enum scope_type{
	GLBL=0,
	FUNC,
	BLOC,
	PROT
};

enum node_type {
	IDENT=1
};

struct node_ident {
	enum namespace_type type;
	char *name;
	char *filename;
	int lineno;
};

struct ast_node {
	enum node_type type;
	struct ast_node *next;
	union {
		struct node_ident ident;
	} u;
};

struct ast_node *ast;

struct ast_node *new_node(enum node_type);
void add_node(struct ast_node *, struct ast_node *);
#endif
