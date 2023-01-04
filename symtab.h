#ifndef SYMTAB_H
#define SYMTAB_H

#include "arvoresintatica.h"

#define TAB_SIZE 255
#define SHIFT 4

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
int check_tab(tab_lines* tab, char* nome, id_type type);
void printa_tab(tab_lines* tab);
void destroy_tab(tab_lines* tab);

#endif
