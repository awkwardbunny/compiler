#ifndef SYMBOL_H
#define SYMBOL_H

#define NS_NAME 1
#define NS_TAGS 2
#define NS_MEMB 3
#define NS_LABL 4

struct scope {
	struct scope *parent;
	struct scope *child;
	struct sym *table;
};

struct sym {
	int ns;
	char *name;
	int value;
	struct sym *next;
};

struct scope *global, *current;

void init_sym_table();
void new_sym(char *, int, int);
int get_sym(char *, int);
int *where_sym(char *, int);
void new_scope();
void exit_scope();
void print_table();

void yyerror(char *, ...);
void yywarn(char *, ...);
#endif

