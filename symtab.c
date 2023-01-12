#include "symtab.h"
#include "arvoresintatica.h"
#include "myglobals.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>

unsigned char need_int = 0;// indica que um funcao ativada em um expressao
						   // nao pode ser do tipo void
char* local_scope = "global0";// escopo inicial
//escopo global termina em 0 para nao ser confundido com nome de funcao

static tab_lines* init_tab(){// aloca a tabela e aponta suas entradas para null
	tab_lines* ret;

	ret = (tab_lines*)malloc(sizeof(tab_lines));

	ret->num_entradas = 0;
	for(int i = 0; i < TAB_SIZE; i++){
		ret->linhas[i] = NULL;
	}
	
	return ret;
}

static lin_t* aloca_lin(int linha){// aloca uma celula da lista de linhas
	lin_t* ret = (lin_t*)malloc(sizeof(lin_t));

	ret->key = linha;
	ret->prox = NULL;

	return ret;
}

static tab_cols* new_func_tab(treeNode* node){//cria nova entrada da tabela para funcao
	tab_cols* ret;
	ret = (tab_cols*)malloc(sizeof(tab_cols));
	if(ret == NULL){
		fprintf(saida, "sem memoria\n");
		error = 1;
		return NULL;
	}

	ret->nome = strdup(node->key.nome);
	ret->escopo = strdup("global0");//declaracao de funcao sempre goblal
	ret->tipo_id = func_tab;
	ret->prox = NULL;
	
	if(node->filho[0]->subTipo.decl == int_t)
		ret->tipo_dado = int_d;
	else ret->tipo_dado = void_d;

	ret->linhas = ret->last_line = aloca_lin(node->linha);

	return ret;
}

static tab_cols* new_var_tab(treeNode* node){// cria nova entrada da tabela para variaveis
	tab_cols* ret;
	ret = (tab_cols*)malloc(sizeof(tab_cols));
	if(ret == NULL){
		fprintf(saida, "sem memoria\n");
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

static int hash(char *scope, char* key){// funcao de hash
	//concatena o escopo com o nome para fazer o hash
	char* fullName = (char*)malloc(strlen(scope) + strlen(key) + 1);
	bzero(fullName, strlen(scope) + strlen(key) + 1);
	memcpy(fullName, scope, strlen(scope));
	strcat(fullName, key);

	int ret = 0;

	for(int i = 0; fullName[i] != '\0'; i++)
		ret = ((ret << SHIFT) + fullName[i]) % TAB_SIZE;

	free(fullName);// desaloca o buffer utilizado
	return ret;
}

static void erro_semantico(char* tk, char* msg, int lin){// prita o erro semantico
	fprintf(saida, "erro semantico: %s, token \'%s\', linha: %d\n", msg, tk, lin);
}

static void cuida_vari(tab_lines* tab, treeNode* node){//nova declaracao de variavel
	int h_local = HASH_LOCAL;
	int h_global = hash("", node->key.nome);

	if(node->filho[0]->subTipo.exp == void_t){// varivel nao pode ser void
		error = 1;
		erro_semantico(node->key.nome, "variavel void",node->linha);
		return;
	}
	//verifica se nome ja foi usado por um funcao ou se e apenas colizao de hash
	if(tab->linhas[h_global] != NULL){
		tab_cols* l = tab->linhas[h_global];
		
		while(l){
			if(!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, "global0") && l->tipo_id == func_tab){
				error = 1;
				erro_semantico(node->key.nome, "declaracao global anterior ja definida", node->linha);
				return;
			}
			l = l->prox;
		}
	}

	//verifica se a varivel ja foi declarada no escopo ou colizao de hash
	if(tab->linhas[h_local] != NULL){
		tab_cols* l = tab->linhas[h_local];

		if(!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, local_scope)){
			error = 1;
			erro_semantico(node->key.nome, "declaracao anterior ja definida", node->linha);
			return;
		}

		tab_cols* ant = l;
		l = l->prox;

		while(l){
			if(!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, local_scope)){
				error = 1;
				erro_semantico(node->key.nome, "variavel declarada anteriormente", node->linha);
				return;
			}
			ant = l;
			l = l->prox;
		}
		ant->prox = new_var_tab(node);
	}
	else tab->linhas[h_local] = new_var_tab(node);// caso base
}

void cuida_uso(tab_lines* tab, treeNode* node){//novo uso de varivel
	int h_local = HASH_LOCAL;
	int h_global = HASH_GLOBAL;

	// procura a declaracao da variavel e insire uma nova linha de aparicao
	// da preferencia para declaracao local
	// checao colizo de hassh
	if(tab->linhas[h_local] != NULL){
		tab_cols* l = tab->linhas[h_local];
		
		while(l){
			if(!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, local_scope)){
				l->last_line = l->last_line->prox = aloca_lin(node->linha);// encontrada no escopo local
				return;
			}
			l = l->prox;
		}
	}
	else if(tab->linhas[h_global] != NULL){
		tab_cols* l = tab->linhas[h_global];
		
		while(l){
			if(!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, "global0")){
				l->last_line = l->last_line->prox = aloca_lin(node->linha);//encontrada no escopo global
				return;
			}
			l = l->prox;
		}
	} 
	//declaracao previa nao encontrada
	error = 1;
	erro_semantico(node->key.nome, "variavel nao declarada",node->linha);
}

void cuida_atv(tab_lines* tab, treeNode* node){//noca ativacao de variavel
	int h_global = HASH_GLOBAL;

	//verifica se funcao foi declarada
	//verifica se e colizao de hash
	if(tab->linhas[h_global] != NULL){
		tab_cols* l = tab->linhas[h_global];
		
		while(l){
			if((!strcmp(l->nome, node->key.nome) && !strcmp(l->escopo, "global0"))
				&& !(l->tipo_dado == void_d && need_int)){
				l->last_line = l->last_line->prox = aloca_lin(node->linha);//declaracao encontrada insere nova linha
				return;
			}
			l = l->prox;
		}
	} 
	//declaracao nao encontrada
	error = 1;
	erro_semantico(node->key.nome, "funcao nao declarada",node->linha);
}

static void percorre_subarv_tab(tab_lines* tab, treeNode* node){// percorre subarvore recursivamente
	if(node == NULL) return;
	unsigned char isAssing = 0;

	//empre q identifica um id chama a funcao apropriada para verificar erros e inserrir na tabela
	if(node->tipo == declk && node->subTipo.decl == vari){//declaracao de varivel encontrada
		tab->num_entradas++;
		cuida_vari(tab, node);
	}else if(node->tipo == expk && node->subTipo.exp == uso){//uso de varivel encontrado
		cuida_uso(tab, node);
	}else if(node->tipo == expk && node->subTipo.exp == atv) {//ativacao de funcao encontrada
		cuida_atv(tab, node);
	}else if(node->tipo == expk && node->subTipo.exp == oper && node->key.op == ig && !need_int){
		//uma atribuicao foi encontrada, filho nao pode ser funcao void
		isAssing = 1;
		need_int = 1;
	}

	//recursao so ocorre caso nenhum erro for identificado
	for(int i = 0; i < MAXFILHO && !error; i++)
		percorre_subarv_tab(tab, node->filho[i]);
	if(isAssing) need_int = 0;//saida da atribuicao funcoes ativadas podem ser void
	if(!error)percorre_subarv_tab(tab, node->irmao);

}

tab_lines* create_tab(treeNode* n){// cria a tabela e faz analiza semantica
	tab_lines* ret = (tab_lines*)malloc(sizeof(tab_lines));
	int h_global;
	char have_main = 0;//identifica se funcao main ja foideclarada
	tab_cols* temp;
	treeNode* node = n;

	while(node && !have_main && !error){
	//para quando funcao main for encontrada ou quando erro for encontrado ou fim da arvore		

		ret->num_entradas++;
		h_global = hash("", node->key.nome);
		temp = ret->linhas[h_global];

		if(temp != NULL){// trata colizao de hash e declaracao repetida no escopo global
			if(!strcmp(node->key.nome, temp->nome) && !strcmp(temp->escopo, "global0")){
				erro_semantico(node->key.nome, "declaracao global anterior ja definida", node->linha);
				error = 1;
				return ret;
			}

			while(temp->prox != NULL){
				if(!strcmp(node->key.nome, temp->nome) && !strcmp(temp->escopo, "global0")){
					erro_semantico(node->key.nome, "variavel declarada globalmente anteriormente", node->linha);
					error = 1;
					return ret;
				}
				temp = temp->prox;
			}

			if(node->tipo == declk && node->subTipo.decl == func)
				temp->prox = new_func_tab(node);
			else temp->prox = new_var_tab(node);

		} else {
			if(node->tipo == declk && node->subTipo.decl == func)
				ret->linhas[h_global] = new_func_tab(node);
			else ret->linhas[h_global] = new_var_tab(node);

			temp = ret->linhas[h_global];
		}

		if(temp->tipo_id == func_tab){// caso de funcao muda escopo e percorre sub arvore
			local_scope = temp->nome;
			for(int i = 0; i < MAXFILHO; i++)
				percorre_subarv_tab(ret, node->filho[i]);

			if(!strcmp(temp->nome, "main")) have_main = 1;
		}

		local_scope = "global0";
		node = node->irmao;

	}

	//erros envolvendo funcao main
	if(node != NULL && !error){
		error = 1;
		erro_semantico(node->key.nome, "declaracao pos main", node->linha);
	} else if(!have_main && !error){
		error = 1;
		erro_semantico("main", "funcao main nao declarada", yylineno);
	}


	return ret;
}

static void desaloca_lin(lin_t* l){//desaloca lisra ligada de linhas
	if(l == NULL) return;

	desaloca_lin(l->prox);
	free(l);
}

static void desaloca_tab(tab_cols* l){//desaloca linha da tabela
	if(l == NULL) return;

	desaloca_tab(l->prox);
	desaloca_lin(l->linhas);
	free(l->nome);
	free(l->escopo);
	free(l);

}

void destroy_tab(tab_lines* tab){//percore tabela a desalocalndo

		for(int i = 0, j = 0; i < TAB_SIZE && j != tab->num_entradas; i++){
		if(tab->linhas[i] != NULL){
			j++;
			desaloca_tab(tab->linhas[i]);
		}
	}
	free(tab);

}

void printa_tab(tab_lines* tab){//percorre a tabela a printando
	fprintf(saida, "tamanho efetivo da tabela: %d\n", tab->num_entradas);

	for(int i = 0, j = 0; i < TAB_SIZE && j != tab->num_entradas; i++){
		tab_cols* entrada = tab->linhas[i];

		while(entrada){
			j++;
			fprintf(saida, "%d| %s - %s - ", i, entrada->nome, entrada->escopo);

			if(entrada->tipo_id == func_tab) fprintf(saida, "func - ");
			else fprintf(saida, "var - ");

			if(entrada->tipo_id == int_d) fprintf(saida, "int - ");
			else fprintf(saida, "void : ");

			lin_t* l = entrada->linhas;

			while(l){
				fprintf(saida, "%lu,",l->key);
				l = l->prox;
			}
			fprintf(saida, "\n");
			entrada = entrada->prox;
		}

	}

}
