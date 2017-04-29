#ifndef SYMBOL_H
#define SYMBOL_H
#include <ast.h>

struct scope {
	enum scope_type type;
	struct scope *parent;
	struct scope *child;
	struct sym *symbols;
};

struct sym {
	enum namespace_type ns;
	char *name;
	long long val;
	struct ast_node *an;
	struct sym *next;
};

struct scope *global, *current;

struct scope *new_sym_table(void);
void del_sym_table(struct scope *);

long long get_sym(struct scope *table, char *name, enum namespace_type);
void set_sym(struct scope *table, char *name, enum namespace_type, long long val, int replace);
struct ast_node *get_node(struct scope *table, char *name, enum namespace_type);
void set_node(struct scope *table, char *name, enum namespace_type, struct ast_node *val);

void new_scope(enum scope_type);
void exit_scope();
void print_table(struct scope *);

void yyerror(char *, ...);
void yywarn(char *, ...);
#endif

