#ifndef AST_H
#define AST_H

enum namespace_type {
	NS_NAME=0,
	NS_TAGS,
	NS_MEMB,
	NS_LABL
};

enum scope_type{
	SCOPE_GLBL=0,
	SCOPE_FUNC,
	SCOPE_BLOC,
	SCOPE_PROT
};

enum node_type {
	NODE_IDENT=1
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
