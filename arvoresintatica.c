#include "arvoresintatica.h"
#include <stdlib.h>
#include <stdio.h>

static treeNode *alocador(){
	treeNode *ret = (treeNode*)malloc(sizeof(treeNode));

	for(int i = 0; i < MAXFILHO; i++) ret->filho[i] = NULL;
	ret->irmao = NULL;

	return ret;

}

treeNode* criaExp(int clin, tipoExp subt){
	treeNode *ret = alocador();

	ret->linha = clin;
	ret->tipo = expk;
	ret->subTipo.exp = subt;

	return ret;
}

treeNode* criaDecl(int clin, tipoDecl subt){
	treeNode *ret = alocador();

	ret->linha = clin;
	ret->tipo = declk;
	ret->subTipo.decl = subt;

	return ret;
}

treeNode* criaEnd(int clin, tipoEnd subt){
	treeNode *ret = alocador();

	ret->linha = clin;
	ret->tipo = endk;
	ret->subTipo.end = subt;

	return ret;
}

void desaloca(treeNode *node){
	if(!node) return;

	for(int i = 0; i < MAXFILHO; i++) desaloca(node->filho[i]);
	desaloca(node->irmao);

	free(node);
}

void printaArv(treeNode *node){
	if(!node) return;

	if(node->tipo == expk){
		switch(node->subTipo.exp){
			case comp:
				printf("%c\n", node->key.op);
			break;
			
			case atrb:
				printf("%c\n", node->key.op);
			break;
			
			case mat:
				printf("%c\n", node->key.op);
			break;
			
			case atv:
				printf("%s\n", node->key.nome);
			break;

			default:
				printf("subtipo nao reconhecido: %d", node->subTipo.exp);
			break;
		}
	} else if(node->tipo == declk){
		switch(node->subTipo.decl){

			case selec:
				printf("if\n");
			break;

			case iterac:
				printf("while\n");
			break;

			case reto:
				printf("return\n");
			break;

			case func:
				printf("%s\n", node->key.nome);
			break;

			case vari:
				printf("%s\n", node->key.nome);
			break;

			default:
				printf("subtipo nao reconhecido: %d", node->subTipo.decl);
			break;
		}
	} else if(node->tipo == endk){
		switch(node->subTipo.end){
			case int_t:
				printf("int\n");
			break;

			case void_t:
				printf("void\n");
			break;

			case cons:
				printf("%d\n", node->key.val);
			break;

			default:
				printf("subtipo nao reconhecido: %d", node->subTipo.end);
			break;
		}
	} else{
		printf("tipo não reconhecido: %d\n", node->tipo);
	}

	for(int i = 0; i < MAXFILHO; i++) printaArv(node->filho[i]);
	printaArv(node->irmao);

}
