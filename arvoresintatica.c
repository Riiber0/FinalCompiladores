#include "arvoresintatica.h"
#include "myglobals.h"

static treeNode *alocador(){
	treeNode *ret = (treeNode*)malloc(sizeof(treeNode));

	for(int i = 0; i < MAXFILHO; i++) ret->filho[i] = NULL;
	ret->irmao = NULL;

	return ret;

}

treeNode* criaExp(tipoExp subt){
	treeNode *ret = alocador();

	ret->linha = line_cont;
	ret->tipo = expk;
	ret->subTipo.exp = subt;

	return ret;
}

treeNode* criaDecl(tipoDecl subt){
	treeNode *ret = alocador();

	ret->linha = line_cont;
	ret->tipo = declk;
	ret->subTipo.decl = subt;

	return ret;
}

treeNode* criaEnd(tipoEnd subt){
	treeNode *ret = alocador();

	ret->linha = line_cont;
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

int ident = 0;
#define IDENTAR ident++
#define DESIDENTAR ident--


void prettyprint(treeNode* node){
	for(int i = 0; i < ident; i++)
		printf("  ");
	printf("%d- ",node->linha);
}

void printaArv(treeNode *node){
	if(node == NULL) return;
	prettyprint(node);

	if(node->tipo == expk){
		switch(node->subTipo.exp){
			case comp:
				printf("%s\n", node->key.op);
			break;
			
			case atrb:
				printf("%s\n", node->key.op);
			break;
			
			case mat:
				printf("%s\n", node->key.op);
			break;
			
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
	IDENTAR;
	for(int i = 0; i < MAXFILHO; i++) printaArv(node->filho[i]);
	DESIDENTAR;
	printaArv(node->irmao);

}
