#ifndef SYMBOL_H
#define SYMBOL_H

enum ident_type {
	ID_VAR = 0,
	ID_FUNC,
	ID_TDEF,
	ID_ECONST,
	ID_STAG,
	ID_ETAG,
	ID_UTAG,
	ID_LABL,
	ID_SMEM,
	ID_UMEM
};

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

struct scope {
	enum scope_type type;
	struct scope *parent;
	struct scope *child;
	struct sym *symbols;
};

enum stg_class{
	stg_auto,
	stg_extern,
	stg_register,
	stg_static
};

struct attr_var{
	struct ast_node *type;
	enum stg_class stg;
	int stack_offset;
};

struct attr_func{
	struct ast_node *ret_type;
	struct ast_node *arg_type;
	enum stg_class stg;
	int is_inline;
	int is_defined;
};

struct attr_tag{
	struct scope *table;
	int is_def;
};

struct attr_econst{
	int val;
};

struct attr_lbl{
	// ??
};

struct attr_tdef{
	struct ast_node *eqv;
};

struct attr_memb{
	struct ast_node *type;
	int offset;
	int bit_width;
	int bit_offset;
};

struct sym {
	char *name;
	struct ast_node *an;
	struct sym *next;
	enum namespace_type ns;

	char *fname;
	int lineno;

	enum ident_type it;
	union attrs{
		struct attr_var v;
		struct attr_func f;
		struct attr_tag t;
		struct attr_econst ec;
		struct attr_lbl l;
		struct attr_tdef td;
		struct attr_memb m;
	} u;
};

struct scope *global, *current;

struct scope *new_sym_table(void);
void del_sym_table(struct scope *);

struct sym *get_sym(struct scope *table, char *name, enum namespace_type);
int set_sym(struct scope *table, char *name, enum namespace_type, struct sym *sp, int replace);

void new_scope(enum scope_type);
void exit_scope();
void print_table(struct scope *);

void yyerror(char *, ...);
void yywarn(char *, ...);
#endif

