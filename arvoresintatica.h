#ifndef ARVORESINTATICA_H
#define ARVORESINTATICA_H

#define MAXFILHO 3
typedef enum {expk, declk, endk} tipoNo;

typedef enum { selec, iterac, reto, func, vari} tipoDecl;
typedef enum {comp, atrb, mat, atv, uso} tipoExp;
typedef enum {int_t, void_t, cons} tipoEnd;

typedef struct treeNode{
	struct treeNode *irmao;
	struct treeNode *filho[MAXFILHO];

	int linha;
	tipoNo tipo;

	union {tipoDecl decl; tipoExp exp; tipoEnd end;} subTipo;
	union { char* op;
			int val;
			char *nome;} key;

	unsigned char especial;

}treeNode;

treeNode* criaExp( tipoExp subt);
treeNode* criaDecl(tipoDecl subt);
treeNode* criaEnd(tipoEnd subt);
void printaArv(treeNode* node);
void desaloca(treeNode* node);

#endif
