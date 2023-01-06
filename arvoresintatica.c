#include "arvoresintatica.h"
#include "myglobals.h"
#include <stdio.h>

static treeNode *alocador(){
	treeNode *ret = (treeNode*)malloc(sizeof(treeNode));

	for(int i = 0; i < MAXFILHO; i++) ret->filho[i] = NULL;
	ret->irmao = NULL;

	return ret;

}

treeNode* criaExp(tipoExp subt, int line){
	treeNode *ret = alocador();

	ret->linha = line;
	ret->tipo = expk;
	ret->subTipo.exp = subt;

	return ret;
}

treeNode* criaDecl(tipoDecl subt, int line){
	treeNode *ret = alocador();

	ret->linha = line;
	ret->tipo = declk;
	ret->subTipo.decl = subt;

	return ret;
}

treeNode* criaEnd(tipoEnd subt, int line){
	treeNode *ret = alocador();

	ret->linha = line;
	ret->tipo = endk;
	ret->subTipo.end = subt;

	return ret;
}

void desaloca(treeNode *node){
	if(node == NULL) return;

	for(int i = 0; i < MAXFILHO; i++) desaloca(node->filho[i]);
	desaloca(node->irmao);

	if(node->tipo == declk && (node->subTipo.decl == func || node->subTipo.decl == vari))
		free(node->key.nome);
	else if(node->tipo == expk && (node->subTipo.decl == atv || node->subTipo.decl == uso))
		free(node->key.nome);

	free(node);
}

int ident = 0;
#define IDENTAR ident++
#define DESIDENTAR ident--


void prettyprint(treeNode* node){
	for(int i = 0; i < ident; i++)
		printf("  ");
	printf("%d- ",node->linha);
}

void printa_noArv(treeNode* node){
	if(node->tipo == expk && node->subTipo.exp != oper){
		switch(node->subTipo.exp){
			case atv:
				printf("%s\n", node->key.nome);
			break;

			case uso:
				printf("%s\n", node->key.nome);
			break;

			default:
				printf("subtipo nao reconhecido: %d", node->subTipo.exp);
			break;
		}
	} else if(node->tipo == expk && node->subTipo.exp == oper){
		switch(node->key.op){
			case igig:
				printf("==\n");
			break;

			case maig:
				printf(">=\n");
			break;

			case meig:
				printf("<=\n");
			break;

			case dife:
				printf("!=\n");
			break;

			case ma:
				printf(">\n");
			break;

			case me:
				printf("<\n");
			break;

			case ig:
				printf("=\n");
			break;

			case adi:
				printf("+\n");
			break;

			case sub:
				printf("-\n");
			break;

			case mult:
				printf("*\n");
			break;

			case divs:
				printf("/\n");
			break;

			default:
				printf("operador nao reconhecido");
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

	fflush(stdout);
}

void printaArv(treeNode *node){
	if(node == NULL) return;
	prettyprint(node);
		
	printa_noArv(node);

	IDENTAR;
	for(int i = 0; i < MAXFILHO; i++) printaArv(node->filho[i]);
	DESIDENTAR;
	printaArv(node->irmao);

}
