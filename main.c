#include "arvoresintatica.h"
#include "myglobals.h"
#include "analizadorsintatico.tab.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *source;
FILE* saida;
unsigned char error = 0;

treeNode* preDef(treeNode* comeco){

	treeNode *filhoRaiz, *filhoIrmao;
	
	filhoRaiz = criaEnd(int_t, 0);
	filhoIrmao = criaEnd(void_t, 0);

	treeNode *raiz, *irmao, *arg;

	arg = criaDecl(vari, 0);
	arg->filho[0] = criaEnd(int_t, 0);
	arg->key.nome = strdup("x");

	irmao = criaDecl(func, 0);
	irmao->key.nome = strdup("output");
	irmao->filho[0] = filhoIrmao;
	irmao->filho[1] = arg;

	raiz = criaDecl(func, 0);
	raiz->key.nome = strdup("input");
	raiz->filho[0] = filhoRaiz;
	raiz->irmao = irmao;
	irmao->irmao = comeco;

	return raiz;
}

void protocolo_saida(tab_lines* tab, treeNode* raiz, int status){

	if(raiz)desaloca(raiz);
	if(tab)destroy_tab(tab);
	fclose(source);

	flex_end();

	exit(status);
}

int main(int argc, char** argv){

	if(argc == 1){
		source = stdin;
		saida = stdout;
	}else if(argc == 2){
		source = fopen(argv[1], "r");
		saida = stdout;
		if(source == NULL){
			printf("Arquivo nao encontrado");
			return 1;
		}
	}else if(argc == 3){
		source = fopen(argv[1], "r");
		saida = fopen(argv[2], "w");
		if(source == NULL || saida == NULL){
			printf("Arquivo nao encontrado");
			return 1;
		}
	}else printf("formato errado");


    extern FILE* yyin;
    yyin = source;

	treeNode *raiz, *tree;
	tab_lines* tab;

    tree = parse();
	if(error) protocolo_saida(tab, tree, EXIT_FAILURE);

	raiz = preDef(tree);
	printaArv(raiz);

	tab = create_tab(raiz);
	if(error) protocolo_saida(tab, raiz, EXIT_FAILURE);

	printa_tab(tab);

	protocolo_saida(tab, raiz, EXIT_SUCCESS);
}
