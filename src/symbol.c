#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <symbol.h>

#define DEBUG 0

void init_sym_table(){
	global = (struct scope *)malloc(sizeof(struct scope));
	global->parent = NULL;
	global->child = NULL;
	current = global;
}

void new_sym(char *n, int ns, int val){
#if DEBUG
	printf("Adding symbol: %d %s\t%d\n", ns, n, val);
#endif
	struct sym **spp;
	for(spp = &(current->table); *spp; spp = &((*spp)->next)){
		if(!strcmp((*spp)->name, n)){
			yyerror("Identifier already declared! Ignoring.");
			return;
		}
	}
	*spp = (struct sym *)malloc(sizeof(struct sym));
	//spp = &((*spp)->next);

	int nl = strlen(n);
	(*spp)->name = (char *)malloc(nl);
	(*spp)->ns = ns;
	strncpy((*spp)->name, n, nl);
	(*spp)->value = val;
	(*spp)->next = NULL;
}

/* TODO Not tested */
int *get_sym(char *n, int ns){
	for(struct scope *csc = current; csc; csc = csc->parent){
		//printf(":%x\n", csc);
		for(struct sym *cs = csc->table; cs; cs = cs->next){
			//printf("::%s\n", csym->name);
			if(ns == cs->ns && !strcmp(n, cs->name))
				return &(cs->value);
		}
	}
	yyerror("Identifier %s has not been declared.", n);
	return NULL;
}

void new_scope(){
#if DEBUG
	printf("New scope\n");
#endif
	current->child = (struct scope *)malloc(sizeof(struct scope));
	current->child->parent = current;
	current = current->child;

	current->child = NULL;
	current->table = NULL;
}

void exit_scope(){
#if DEBUG
	printf("Exiting scope\n");
#endif
	struct sym *cs = current->table;
	while(cs){
		struct sym *ns = cs->next;
		free(cs);
		cs = ns;
	}

	if(current==global){
		yyerror("Currently in global scope. Cannot exit. Ignoring.");
		return;
	}

	current = current->parent;
	free(current->child);
	current->child = NULL;
}

void print_table(){
	struct scope *csc;
	struct sym *cs;
	int i = 0;
	for(csc = global; csc; csc = csc->child){
		printf("%d: ", i++);
		for(cs = csc->table; cs; cs = cs->next){
			printf("(%s:%d:%s) ", cs->name, cs->value, (cs->ns ==NS_NAME)?"NAME":(cs->ns==NS_TAGS)?"TAG":(cs->ns==NS_MEMB)?"MEMB":"LABEL");
		}
		printf("\n");
	}
}
