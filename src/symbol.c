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
			free(cs);
			cs = ns;
		}
		struct scope *nsc = csc->child;
		free(csc);
		csc = nsc;
	}
}

long long get_sym(struct scope *table, char *name, enum namespace_type ns){
	for(struct scope *csc = table; csc; csc = csc->parent){
		for(struct sym *cs = csc->symbols; cs; cs = cs->next){
			if(ns == cs->ns && !strcmp(name, cs->name))
				return cs->val;
		}
	}
	yyerror("Undefined variable %s", name);
	return 0;
}

void set_sym(struct scope *table, char *name, enum namespace_type ns, long long val, int replace){
	// Attempt a search
	for(struct scope *csc = table; csc; csc = csc->child){
		for(struct sym *cs = csc->symbols; cs; cs = cs->next){
			//if found
			if(ns == cs->ns && !strcmp(name, cs->name)){
				// and if replace is set
				if(replace)
					cs->val = val;
				else
					yyerror("Identifier already declared! Ignoring.");
				return;
			}
		}
	}

	// If not found, add
#if DEBUG
	printf("Adding symbol: %d %s\t%d\n", ns, name, val);
#endif
	struct scope *csc;
	struct sym **spp;
	for(csc = table; csc->child; csc=csc->child);
	for(spp = &(csc->symbols); *spp; spp = &((*spp)->next));
	*spp = (struct sym *)malloc(sizeof(struct sym));

	(*spp)->name = strdup(name);
	(*spp)->ns = ns;
	(*spp)->val = val;
	(*spp)->next = NULL;
}

struct ast_node *get_node(struct scope *table, char *name, enum namespace_type ns){
	for(struct scope *csc = table; csc; csc = csc->parent){
		for(struct sym *cs = csc->symbols; cs; cs = cs->next){
			if(ns == cs->ns && !strcmp(name, cs->name))
				return cs->an;
		}
	}
	yyerror("Undefined variable %s", name);
	return NULL;
}

void set_node(struct scope *table, char *name, enum namespace_type ns, struct ast_node *val){
	// Attempt a search
	for(struct scope *csc = table; csc; csc = csc->child){
		for(struct sym *cs = csc->symbols; cs; cs = cs->next){
			//if found
			if(ns == cs->ns && !strcmp(name, cs->name)){
				cs->an = val;
				return;
			}
		}
	}

	// If not found
	fprintf(stderr, "Identifier %s cannot be found\n", name);
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
			printf("(%s:%d:%s) ", cs->name, cs->val, (cs->ns ==NS_NAME)?"NAME":(cs->ns==NS_TAGS)?"TAG":(cs->ns==NS_MEMB)?"MEMB":"LABEL");
		}
		printf("\n");
	}
}
