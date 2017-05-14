#ifndef AST_H
#define AST_H
#include <symbol.h>

enum node_type {
	AST_IDENT=1,
	AST_SCALAR,
	AST_NUM,
	AST_PTR,
	AST_FUNC,
	AST_ARRAY,
	AST_VAR,
	AST_MEMB,
	AST_STRUCT,
	AST_UNION,
	AST_ENUM,
	AST_BINOP,
	AST_UNOP,
	AST_IF,
	AST_IFELSE,
	AST_BLOCK
};

struct node_ident {
	char *name;
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

struct node_scalar {
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

struct node_ptr {
	struct ast_node *ptr;
};

struct node_func {
	struct ast_node *func;
	struct ast_node *args;
	enum stg_class stg;
	unsigned int is_inline : 1;
	unsigned int is_defined : 1;
};

struct node_array {
	struct ast_node *ptr;
	int size;
};

struct node_var {
	struct ast_node *ptr;
	char *name;
	enum stg_class stg;
};

struct node_memb {
	struct ast_node *ptr;
	char *name;
};

struct node_struct {
	struct scope *table;
	char *name;
};

struct node_union {
	struct ast_node *table;
	char *name;
};

struct node_enum {
	char *name;
};

enum binop_type {
	BINOP_PLUS,
	BINOP_MINUS,
	BINOP_TIMES,
	BINOP_DIV,
	BINOP_SHL,
	BINOP_SHR,
	BINOP_MOD,
	BINOP_LT,
	BINOP_GT,
	BINOP_LTE,
	BINOP_GTE,
	BINOP_EQEQ,
	BINOP_NOTEQ,
	BINOP_AND,
	BINOP_OR,
	BINOP_XOR,
	BINOP_LOR,
	BINOP_LAND,
	BINOP_DOT
};

struct node_binop {
	enum binop_type op;
	struct ast_node *left, *right;
};

enum unop_type {
	UNOP_LNOT,
	UNOP_BNEG,
	UNOP_DEREF,
	UNOP_ADDROF,
	UNOP_SIZEOF
};

struct node_unop {
	enum unop_type op;
	struct ast_node *ptr;
};

struct node_if {
	struct ast_node *cond, *t;
};

struct node_ifelse {
	struct ast_node *cond, *t, *f;
};

struct node_block {
	struct ast_node *stmts;
};

struct ast_node {
	enum node_type type;
	struct ast_node *next;
	union {
		struct node_ident ident;
		struct node_scalar scalar;
		struct node_num num;
		struct node_ptr ptr;
		struct node_func func;
		struct node_array array;
		struct node_var var;
		struct node_memb memb;
		struct node_union uni;
		struct node_enum enu;
		struct node_binop binop;
		struct node_unop unop;
		struct node_if nif;
		struct node_ifelse nifelse;
		struct node_block block;
	} u;
};

struct ast_node *ast;

struct ast_node *new_node(enum node_type);
void print_ast(struct ast_node *, int);
#endif
