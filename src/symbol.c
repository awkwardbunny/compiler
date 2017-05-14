#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <symbol.h>

#define DEBUG 0

struct scope *new_sym_table(){
#if DEBUG
	printf("Creating new symbol table\n");
#endif
	struct scope *table = (struct scope *)malloc(sizeof(struct scope));
	table->parent = NULL;
	table->child = NULL;
	table->symbols = NULL;
	return table;
}

void del_sym_table(struct scope *st){
#if DEBUG
	printf("Deleting a symbol table\n");
#endif
	struct scope *csc = st;
	while(csc){
		struct sym *cs = csc->symbols;
		while(cs){
			struct sym *ns = cs->next;
			free(cs->name);
			free(cs->fname);
			free(cs);
			cs = ns;
		}
		struct scope *nsc = csc->child;
		free(csc);
		csc = nsc;
	}
}

struct sym *get_sym(struct scope *table, char *name, enum namespace_type ns){
	for(struct scope *csc = table; csc; csc = csc->parent){
		for(struct sym *cs = csc->symbols; cs; cs = cs->next){
			if(ns == cs->ns && !strcmp(name, cs->name))
				return cs;
		}
	}
	//yyerror("Undefined variable %s", name);
	return NULL;
}

int set_sym(struct scope *table, char *name, enum namespace_type ns, struct sym *sp, int replace){
	// Attempt a search
	for(struct sym *cs = table->symbols; cs; cs = cs->next){
		//if found
		if(ns == cs->ns && !strcmp(name, cs->name)){
			// and if replace is set
			if(replace){
				memcpy(cs, sp, sizeof(struct sym));
				free(sp->name);
				free(sp->fname);
				free(sp);
				return 0;
			}
			//yyerror("Identifier already declared! Ignoring.");
			return -1;
		}
	}

	// If not found, add
#if DEBUG
	printf("Adding symbol: %d %s\n", ns, name);
#endif
	sp->next = table->symbols;
	table->symbols = sp;
	return 0;
}

struct sym *new_sym(char *id, enum namespace_type ns, enum ident_type it, char *fn, int ln){
	struct sym *sp;
	sp = (struct sym *)malloc(sizeof(struct sym));

	sp->name = strdup(id);
	sp->ns = ns;
	sp->it = it;
	sp->an = NULL;
	sp->next = NULL;

	sp->fname = strdup(fn);
	sp->lineno = ln;

	return sp;
}

void new_scope(enum scope_type st){
#if DEBUG
	printf("New scope %d\n", st);
#endif
	current->child = (struct scope *)malloc(sizeof(struct scope));
	current->child->parent = current;
	current = current->child;

	current->type = st;
	current->child = NULL;
	current->symbols = NULL;
}

void exit_scope(){
#if DEBUG
	printf("Exiting scope %d\n", current->type);
#endif
	struct sym *cs = current->symbols;
	while(cs){
		struct sym *ns = cs->next;
		free(cs);
		cs = ns;
	}

	if(!current->parent){
		yyerror("Currently in global scope. Cannot exit. Ignoring.");
		return;
	}

	current = current->parent;
	free(current->child);
	current->child = NULL;
}

void print_table(struct scope *table){
	struct scope *csc;
	struct sym *cs;
	int i = 0;
	for(csc = table; csc; csc = csc->child){
		printf("%d (%d): ", i++, csc->type);
		for(cs = csc->symbols; cs; cs = cs->next){
			printf("(%s:%s) ", cs->name, (cs->ns ==NS_NAME)?"NAME":(cs->ns==NS_TAGS)?"TAG":(cs->ns==NS_MEMB)?"MEMB":"LABEL");
		}
		printf("\n");
	}
}
