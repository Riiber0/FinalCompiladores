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

typedef struct lin_t{
	unsigned long key;

	struct lin_t* prox;
}lin_t;

typedef struct tab_cols{
	char* nome;
	char* escopo;
	id_type tipo_id;
	data_type tipo_dado;

	lin_t* linhas;
	lin_t* last_line; 

	struct tab_cols* prox; //caso especial
}tab_cols;

typedef struct tab_lines{
	unsigned char num_entradas;
	tab_cols* linhas[TAB_SIZE];
}tab_lines;

extern tab_lines* symtab;
extern unsigned char atribuicao;

tab_lines* create_tab(treeNode* node);
void printa_tab(tab_lines* tab);
void destroy_tab(tab_lines* tab);

#endif
