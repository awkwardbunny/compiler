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
	NODE_IDENT=1,
	NODE_STRLIT,
	NODE_CHRLIT,
	NODE_NUM
};

struct node_strlit {
	char *bytes;
	int length;
};

enum num_type {
	NT_INT=1,
	NT_UINT,
	NT_L,
	NT_UL,
	NT_LL,
	NT_ULL,
	NT_FP,
	NT_DBL,
	NT_LDBL
};

struct node_num {
	enum num_type type;
	union {
		unsigned int i;
		unsigned long l;
		unsigned long long ll;
		float f;
		double d;
		long double ld;
	} val;
};

struct ast_node {
	enum node_type type;
	struct ast_node *next;
	union {
		char *ident;
		char chrlit;
		struct node_strlit strlit;
		struct node_num num;
	} u;
};

struct ast_node *ast;

struct ast_node *new_node(enum node_type);
struct ast_node *add_node(struct ast_node *, struct ast_node *);
void print_ast(struct ast_node *, int);
#endif
