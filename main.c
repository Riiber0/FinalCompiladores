#include "arvoresintatica.h"
#include "myglobals.h"
#include "analizadorsintatico.tab.h"
#include "symtab.h"
#include <stdio.h>

FILE *source;
unsigned char error = 0;

treeNode* preDef(treeNode* comeco){

	treeNode *filhoRaiz, *filhoIrmao;
	
		filhoRaiz = criaEnd(int_t, 0);
	filhoIrmao = criaEnd(void_t, 0);

	treeNode *raiz, *irmao;

	raiz = criaDecl(func, 0);
	raiz->key.nome = strdup("input");
	raiz->filho[0] = filhoRaiz;

	irmao = criaDecl(func, 0);
	irmao->key.nome = strdup("output");
	irmao->filho[0] = filhoIrmao;

	raiz->irmao = irmao;
	irmao->irmao = comeco;

	return raiz;
}

int main(int argc, char** argv){

    //source = fopen(argv[1], "r");
	source = fopen("sample", "r");
    extern FILE* yyin;
    yyin = source;

	treeNode *raiz, *tree;

    tree = parse();
	if(error){
		desaloca(tree);
		fclose(source);
		return 1;
	}
	raiz = preDef(tree);
	printaArv(raiz);

	printf("tabela comeco\n"); fflush(stdout);
	tab_lines* tab = create_tab(raiz);
	printf("tabela feita\n"); fflush(stdout);
	if(error){
		desaloca(raiz);
		destroy_tab(tab);
		fclose(source);
		return 1;
	}
	if(tab == NULL){
		printf("erro");
		return 1;
	}

	printa_tab(tab);

	desaloca(raiz);
	destroy_tab(tab);
	fclose(source);

	return 0;
}
