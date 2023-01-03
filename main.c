#include "arvoresintatica.h"
#include "myglobals.h"
#include "analizadorsintatico.tab.h"
#include <stdio.h>

FILE *source;
unsigned char error = 0;

treeNode* preDef(treeNode* comeco){

	treeNode *filhoRaiz, *filhoIrmao;
	
	filhoRaiz = criaEnd(int_t, 0);
	filhoIrmao = criaEnd(void_t, 0);

	treeNode *raiz, *irmao;

	raiz = criaDecl(func, 0);
	raiz->key.nome = "input";
	raiz->filho[0] = filhoRaiz;

	irmao = criaDecl(func, 0);
	irmao->key.nome = "output";
	irmao->filho[0] = filhoIrmao;

	raiz->irmao = irmao;
	irmao->irmao = comeco;

	return raiz;
}

int main(int arc, char** argv){

    source = fopen(argv[1], "r");
    extern FILE* yyin;
    yyin = source;

	treeNode *raiz, *tree;

    tree = parse();
	if(!error){
		raiz = preDef(tree);

		printaArv(raiz);
		fflush(stdout);
	}
	desaloca(raiz);

	return 0;
}
