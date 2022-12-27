#ifndef ARVORESINTATICA_H
#define ARVORESINTATICA_H

#define MAXFILHO 3
typedef char opTipo;
typedef enum {expk, declk, endk} tipoNo;

typedef enum { selec, iterac, reto, func, vari} tipoDecl;
typedef enum {comp, atrb, mat, atv} tipoExp;
typedef enum {int_t, void_t, cons} tipoEnd;

typedef struct treeNode{
	struct treeNode *irmao;
	struct treeNode *filho[MAXFILHO];

	int linha;
	tipoNo tipo;

	union {tipoDecl decl; tipoExp exp; tipoEnd end;} subTipo;
	union { opTipo op;
			int val;
			char *nome;} key;

	unsigned char isList;

}treeNode;

treeNode* criaExp(int clin, tipoExp subt);
treeNode* criaDecl(int clin, tipoDecl subt);

#endif
