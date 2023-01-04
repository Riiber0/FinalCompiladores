#include "symtab.h"
#include "arvoresintatica.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

unsigned char need_int = 0;
char* local_scope = "global";

static tab_lines* init_tab(){
	tab_lines* ret;

	ret = (tab_lines*)malloc(sizeof(tab_lines));

	ret->num_entradas = 0;
	for(int i = 0; i < TAB_SIZE; i++){
		ret->linhas[i] = NULL;
	}
	
	return ret;
}

static lin_t* aloca_lin(int linha){
	lin_t* ret = (lin_t*)malloc(sizeof(lin_t));

	ret->key = linha;
	ret->prox = NULL;

	return ret;
}

static tab_cols* new_func_tab(treeNode* node){
	tab_cols* ret;
	ret = (tab_cols*)malloc(sizeof(tab_cols));

	ret->nome = node->key.nome;
	ret->escopo = "global";
	ret->tipo_id = func_tab;
	ret->prox = NULL;
	
	if(node->filho[0]->subTipo.decl == int_t)
		ret->tipo_dado = int_d;
	else ret->tipo_dado = void_d;

	ret->linhas = ret->last_line = aloca_lin(node->linha);

	return ret;
}

static tab_cols* new_var_tab(treeNode* node){
	tab_cols* ret;
	ret = (tab_cols*)malloc(sizeof(tab_cols));

		ret->nome = node->key.nome;
	ret->escopo = local_scope;
	ret->tipo_id = var_tab;
	ret->prox = NULL;
	ret->tipo_dado = int_d;

	ret->linhas = ret->last_line = aloca_lin(node->linha);

	return ret;
}

static int hash(char *scope, char* key){
	char* fullName = strcat(scope, key);

	register int ret = 0;

	for(int i = 0; fullName[i] != '\0'; i++)
		ret = ((ret << SHIFT) + fullName[i]) % TAB_SIZE;

	return ret;
}

void insert_tab(tab_lines* tab, treeNode* node){
	int location;

	if(node->subTipo.decl == func_tab){
		location = hash("", node->key.nome);
		tab->linhas[location] = new_func_tab(node);
	} else {
		location = hash(local_scope, node->key.nome);
		tab->linhas[location] = new_var_tab(node);
	}

}
