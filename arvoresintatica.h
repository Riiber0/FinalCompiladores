#ifndef ARVORESINTATICA_H
#define ARVORESINTATICA_H

#include "myglobals.h"
#define MAXFILHO 3
typedef enum {expk, declk, endk} tipoNo;
//o tipo endk foi um tipo feita para identificar tokens que nao tem filhos

typedef enum { selec, iterac, reto, func, vari} tipoDecl;
//os subtipos de declaracao sao, selec para if, iterac para while, 
//reto para return, func para declaracao de funcao e vari para declaracao de variavel
typedef enum {oper, atv, uso} tipoExp;
//os sub tipos de expressao sao, oper para operadores matematicos, atv para ativacao de funcao
//e uso para uso de variaveis
typedef enum {int_t, void_t, cons} tipoEnd;
//os sub tipos de end sao, int_t para inteiro, void_t para void e cons para constantes

typedef struct treeNode{
	struct treeNode *irmao;
	struct treeNode *filho[MAXFILHO];

	int linha;//linha em q o token foi encontrado
	tipoNo tipo; 

	union {tipoDecl decl; tipoExp exp; tipoEnd end;} subTipo;
	union { opType op;
			int val;
			char *nome;} key;// constantes quardam val, ids node e operadores op

	unsigned char especial;//usado para identifiacr casos especiais como reuturn nao vazio, else e arryas

}treeNode;
//cado tipo de no tem sua funcao de craicao, subtipo e passado como argumento
treeNode* criaExp(tipoExp subt, int line);
treeNode* criaDecl(tipoDecl subt, int line);
treeNode* criaEnd(tipoEnd subt, int line);
void printa_noArv(treeNode* node);
void printaArv(treeNode* node);
void desaloca(treeNode* node);

#endif
