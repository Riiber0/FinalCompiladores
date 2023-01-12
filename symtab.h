#ifndef SYMTAB_H
#define SYMTAB_H

#include "arvoresintatica.h"

#define TAB_SIZE 255
#define SHIFT 4
#define HASH_LOCAL hash(local_scope, node->key.nome)
#define HASH_GLOBAL hash("", node->key.nome)

extern int yylineno;

typedef enum{var_tab, func_tab} id_type;
typedef enum{void_d, int_d} data_type;

typedef struct lin_t{// lista ligada de linhas em que um id aparece
	unsigned long key;// numero da linha

	struct lin_t* prox;// proxima linha
}lin_t;

typedef struct tab_cols{//colunas da tabelad e simbolos
	char* nome;//nome do id
	char* escopo;//escopo do id
	id_type tipo_id;// funcao ou variavel
	data_type tipo_dado;//void ou int

	lin_t* linhas;//lista de linhas em um id aparece
	lin_t* last_line;// ponteiro pro final da lista para facilitar incercao

	struct tab_cols* prox; //trata colisao na tabela hash
}tab_cols;

typedef struct tab_lines{// tabela hash
	unsigned char num_entradas;//numeor de entradas nao nulas 
	tab_cols* linhas[TAB_SIZE];//linhas da tabela
}tab_lines;

extern tab_lines* symtab;

tab_lines* create_tab(treeNode* node);
void printa_tab(tab_lines* tab);
void destroy_tab(tab_lines* tab);

#endif
