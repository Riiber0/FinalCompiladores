#ifndef ARVORESINTATICA_H
#define ARVORESINTATICA_H

#include "myglobals.h"
#define MAXFILHO 3
typedef enum {expk, declk, endk} tipoNo;

typedef enum { selec, iterac, reto, func, vari} tipoDecl;
typedef enum {oper, atv, uso} tipoExp;
typedef enum {int_t, void_t, cons} tipoEnd;

typedef struct treeNode{
	struct treeNode *irmao;
	struct treeNode *filho[MAXFILHO];

	int linha;
	tipoNo tipo;

	union {tipoDecl decl; tipoExp exp; tipoEnd end;} subTipo;
	union { opType op;
			int val;
			char *nome;} key;

	unsigned char especial;

}treeNode;

treeNode* criaExp(tipoExp subt, int line);
treeNode* criaDecl(tipoDecl subt, int line);
treeNode* criaEnd(tipoEnd subt, int line);
void printa_noArv(treeNode* node);
void printaArv(treeNode* node);
void desaloca(treeNode* node);

#endif
