#include "arvoresintatica.h"
#include "myglobals.h"
#include <stdio.h>

static treeNode *alocador(){// alocador de memoria para um no
	treeNode *ret = (treeNode*)malloc(sizeof(treeNode));

	for(int i = 0; i < MAXFILHO; i++) ret->filho[i] = NULL;
	ret->irmao = NULL;

	return ret;

}

treeNode* criaExp(tipoExp subt, int line){// cria no do tipo expressao
	treeNode *ret = alocador();

	ret->linha = line;
	ret->tipo = expk;
	ret->subTipo.exp = subt;

	return ret;
}

treeNode* criaDecl(tipoDecl subt, int line){// cria no do tipo declaracao
	treeNode *ret = alocador();

	ret->linha = line;
	ret->tipo = declk;
	ret->subTipo.decl = subt;

	return ret;
}

treeNode* criaEnd(tipoEnd subt, int line){// cria no do tipo end
	treeNode *ret = alocador();

	ret->linha = line;
	ret->tipo = endk;
	ret->subTipo.end = subt;

	return ret;
}

void desaloca(treeNode *node){// desaloca a arvore recursivamente
	if(node == NULL) return;

	for(int i = 0; i < MAXFILHO; i++) desaloca(node->filho[i]);
	desaloca(node->irmao);

	// caso o no quarde nome de um id, e precisa desalocar a string 
	if(node->tipo == declk && (node->subTipo.decl == func || node->subTipo.decl == vari))
		free(node->key.nome);
	else if(node->tipo == expk && node->subTipo.exp != oper)
		free(node->key.nome);

	free(node);
}

int ident = 0;
#define IDENTAR ident++
#define DESIDENTAR ident--


void prettyprint(treeNode* node){// identa a saida da arvore printando um quantidade de espacos 
								 // de acordo com a variavel ident
	for(int i = 0; i < ident; i++)
		fprintf(saida, "  ");
	fprintf(saida, "%d- ",node->linha);
}

void printa_noArv(treeNode* node){// identifica o tipo de no o printa de acordo
	if(node->tipo == expk && node->subTipo.exp != oper){
		switch(node->subTipo.exp){
			case atv:
				fprintf(saida, "%s\n", node->key.nome);
			break;

			case uso:
				fprintf(saida, "%s\n", node->key.nome);
			break;

			default:
				fprintf(saida, "subtipo nao reconhecido: %d", node->subTipo.exp);
			break;
		}
	} else if(node->tipo == expk && node->subTipo.exp == oper){
		switch(node->key.op){
			case igig:
				fprintf(saida, "==\n");
			break;

			case maig:
				fprintf(saida, ">=\n");
			break;

			case meig:
				fprintf(saida, "<=\n");
			break;

			case dife:
				fprintf(saida, "!=\n");
			break;

			case ma:
				fprintf(saida, ">\n");
			break;

			case me:
				fprintf(saida, "<\n");
			break;

			case ig:
				fprintf(saida, "=\n");
			break;

			case adi:
				fprintf(saida, "+\n");
			break;

			case sub:
				fprintf(saida, "-\n");
			break;

			case mult:
				fprintf(saida, "*\n");
			break;

			case divs:
				fprintf(saida, "/\n");
			break;

			default:
				fprintf(saida, "operador nao reconhecido");
			break;
		}
	} else if(node->tipo == declk){
		switch(node->subTipo.decl){

			case selec:
				fprintf(saida, "if\n");
			break;

			case iterac:
				fprintf(saida, "while\n");
			break;

			case reto:
				fprintf(saida, "return\n");
			break;

			case func:
				fprintf(saida, "%s\n", node->key.nome);
			break;

			case vari:
				fprintf(saida, "%s\n", node->key.nome);
			break;

			default:
				fprintf(saida, "subtipo nao reconhecido: %d", node->subTipo.decl);
			break;
		}
	} else if(node->tipo == endk){
		switch(node->subTipo.end){
			case int_t:
				fprintf(saida, "int\n");
			break;

			case void_t:
				fprintf(saida, "void\n");
			break;

			case cons:
				fprintf(saida, "%d\n", node->key.val);
			break;

			default:
				fprintf(saida, "subtipo nao reconhecido: %d", node->subTipo.end);
			break;
		}
	} else{
		fprintf(saida, "tipo não reconhecido: %d\n", node->tipo);
	}

	fflush(stdout);
}

void printaArv(treeNode *node){// percorre a arvore recursivamente e printa cada no
	if(node == NULL) return;
	prettyprint(node);
		
	printa_noArv(node);

	IDENTAR;
	for(int i = 0; i < MAXFILHO; i++) printaArv(node->filho[i]);
	DESIDENTAR;
	printaArv(node->irmao);

}
