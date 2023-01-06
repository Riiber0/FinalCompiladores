#include "symtab.h"
#include "arvoresintatica.h"
#include "myglobals.h"
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
	if(ret == NULL){
		printf("sem memoria\n");
		error = 1;
		return NULL;
	}

	ret->nome = strdup(node->key.nome);
	ret->escopo = strdup("global0");
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
	if(ret == NULL){
		printf("sem memoria\n");
		error = 1;
		return NULL;
	}

	ret->nome = strdup(node->key.nome);
	ret->escopo = strdup(local_scope);
	ret->tipo_id = var_tab;
	ret->prox = NULL;
	ret->tipo_dado = int_d;

	ret->linhas = ret->last_line = aloca_lin(node->linha);

	return ret;
}

static int hash(char *scope, char* key){
	char* fullName = (char*)malloc(sizeof(strlen(scope) + strlen(key) + 1));
	memcpy(fullName, scope, strlen(scope));
	strcat(fullName, key);

	int ret = 0;

	for(int i = 0; fullName[i] != '\0'; i++)
		ret = ((ret << SHIFT) + fullName[i]) % TAB_SIZE;

	free(fullName);
	return ret;
}

void erro_semantico(char* tk, int lin){
	printf("erro semantico: %s, linha: %d\n",tk, lin);
}

static void cuida_vari(tab_lines* tab, treeNode* node){
	int h_local = HASH_LOCAL;
	int h_global = HASH_GLOBAL;

	if(node->filho[0]->subTipo.exp == void_t){
		error = 1;
		printf("variavel void - ");
		erro_semantico(node->key.nome, node->linha);
		return;
	}

	if(tab->linhas[h_global] != NULL){
		tab_cols* l = tab->linhas[h_global];
		
		while(l){
			if(!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, "global0")){
				error = 1;
				printf("variavel declarada globalmente anteriormente - ");
				erro_semantico(node->key.nome, node->linha);
				return;
			}
			l = l->prox;
		}
	}

	else if(tab->linhas[h_local] != NULL){
		tab_cols* l = tab->linhas[h_local];

		if(!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, local_scope)){
			error = 1;
			printf("variavel declarada anteriormente - ");
			erro_semantico(node->key.nome, node->linha);
			return;
		}

		tab_cols* ant = l;
		l = l->prox;

		while(l){
			if(!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, local_scope)){
				error = 1;
				printf("variavel declarada anteriormente - ");
				erro_semantico(node->key.nome, node->linha);
				return;
			}
			ant = l;
			l = l->prox;
		}
		ant->prox = new_var_tab(node);
	}
	else if(tab->linhas[h_global] == NULL && tab->linhas[h_local] == NULL) tab->linhas[h_local] = new_var_tab(node);
}

void cuida_uso(tab_lines* tab, treeNode* node){
	int h_local = HASH_LOCAL;
	int h_global = HASH_GLOBAL;

	if(tab->linhas[h_global] != NULL){
		tab_cols* l = tab->linhas[h_global];
		
		while(l){
			if(!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, "global0")){
				l->last_line = l->last_line->prox = aloca_lin(node->linha);
				return;
			}
			l = l->prox;
		}
	} 
	if(tab->linhas[h_local] != NULL){
		tab_cols* l = tab->linhas[h_local];
		
		while(l){
			if(!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, local_scope)){
				l->last_line = l->last_line->prox = aloca_lin(node->linha);
				return;
			}
			l = l->prox;
		}
	}
	error = 1;
	printf("variavel nao declarada - ");
	erro_semantico(node->key.nome, node->linha);
}

void cuida_atv(tab_lines* tab, treeNode* node){
	int h_global = HASH_GLOBAL;

	if(tab->linhas[h_global] != NULL){
		tab_cols* l = tab->linhas[h_global];
		
		while(l){
			if((!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, "global0"))
				&& !(l->tipo_dado == void_d && need_int)){
				l->last_line = l->last_line->prox = aloca_lin(node->linha);
				return;
			}
			l = l->prox;
		}
	} 
	error = 1;
	erro_semantico(node->key.nome, node->linha);
}

static void percorre_subarv_tab(tab_lines* tab, treeNode* node){
	if(node == NULL) return;
	unsigned char isAssing = 0;
	printa_noArv(node);

	if(node->tipo == declk && node->subTipo.decl == vari){
		tab->num_entradas++;
		cuida_vari(tab, node);
	}else if(node->tipo == expk && node->subTipo.exp == uso){
		cuida_uso(tab, node);
	}else if(node->tipo == expk && node->subTipo.exp == atv) {
		cuida_atv(tab, node);
	}else if(node->tipo == expk && node->subTipo.exp == oper && node->key.op == ig && !need_int){
		isAssing = 1;
		need_int = 1;
	}

	for(int i = 0; i < MAXFILHO && !error; i++)
		percorre_subarv_tab(tab, node->filho[i]);
	if(isAssing) need_int = 0;
	if(!error)percorre_subarv_tab(tab, node->irmao);

}

tab_lines* create_tab(treeNode* n){
	tab_lines* ret = (tab_lines*)malloc(sizeof(tab_lines));
	int h_global;
	treeNode* node = n;

	while(node){
		ret->num_entradas++;
		h_global = HASH_GLOBAL;

		if(node->tipo == declk && node->subTipo.decl == func){
			printf("%s\n", node->key.nome); fflush(stdout);
			ret->linhas[h_global] = new_func_tab(node);

			local_scope = node->key.nome;
			for(int i = 0; i < MAXFILHO && !error; i++)
				percorre_subarv_tab(ret, node->filho[i]);


		} else if(node->tipo == declk && node->subTipo.decl == vari) {
			printf("%s\n", node->key.nome); fflush(stdout);
			ret->linhas[h_global] = new_var_tab(node);
		}

		node = node->irmao;

	}

	return ret;
}

static void desaloca_lin(lin_t* l){
	if(l == NULL) return;

	desaloca_lin(l->prox);
	free(l);
}

void destroy_tab(tab_lines* tab){

	for(int i = 0; i < TAB_SIZE; i++){
		if(tab->linhas[i] != NULL){
			desaloca_lin(tab->linhas[i]->linhas);
			free(tab->linhas[i]->nome);
			free(tab->linhas[i]->escopo);
			free(tab->linhas[i]);
		}
	}
	free(tab);

}

void printa_tab(tab_lines* tab){
	printf("tamanho efetivo da tabela: %d\n", tab->num_entradas);

	for(int i = 0; i < TAB_SIZE; i++){
		tab_cols* entrada = tab->linhas[i];

		while(entrada){
			printf("%d| %s - %s - ", i, entrada->nome, entrada->escopo);

			if(entrada->tipo_id == func_tab) printf("func - ");
			else printf("var - ");

			if(entrada->tipo_id == int_d) printf("int - ");
			else printf("void : ");

			lin_t* l = entrada->linhas;

			while(l){
				printf("%lu,",l->key);
				l = l->prox;
			}
			printf("\n");
			entrada = entrada->prox;
			fflush(stdout);
		}

	}

}