#include "arvoresintatica.h"
#include "myglobals.h"
#include "analizadorsintatico.tab.h"
#include <stdio.h>

FILE *source;

treeNode* preDef(treeNode* comeco){

	treeNode *raiz, *irmao;

	raiz = criaDecl(func, 0);
	raiz->key.nome = "input";

	irmao = criaDecl(func, 0);
	irmao->key.nome = "output";

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
	raiz = preDef(tree);

	printaArv(raiz);
	fflush(stdout);
	desaloca(raiz);

	return 0;
}
