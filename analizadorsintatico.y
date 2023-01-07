%{

    #include "myglobals.h"
	#include "arvoresintatica.h"

	int yylex();
	int yyerror(char* msg);
	extern char* yytext;
	static treeNode *newTree;

%}

%code requires{
    #include "myglobals.h"
	#include "arvoresintatica.h"
}

%code provides{
	treeNode* parse();
}

%locations

%token <num> NUM 
%token <str> ID
%token IF ELSE INT RETURN VOID WHILE
%token <op> MAIORIGUAL MENORIGUAL IGUALIGUAL DIFERENTE MENOR MAIOR
%token <op> MULT DIV ADI SUB IGUAL
%token PARNTS_OP PARNTS_CL COLX_OP COLX_CL CHAVES_OP CHAVES_CL
%token PNTVIRG VIRG  

%start programa

%union{
    int num;
    char* str;
	char ch;
	opType op;
	treeNode* Node;
}

%type <Node> programa declaracaolista declaracao vardeclaracao fundeclaracao tipoespecificador params
%type <Node> paramlista VOID INT compostodecl param localdeclaracoes statementlista statement
%type <Node> expressaodecl selecaodecl iteracaodecl retornodecl expressao var simplesexpressao
%type <Node> somaexpressao termo fator ativacao args arglista
%type <op> relacional soma multi

%%

programa : declaracaolista {newTree = $1;};

declaracaolista : declaracaolista declaracao {treeNode* t = $1;
												while(t->irmao != NULL){
													t = t-> irmao;
												}t->irmao = $2;}
				| declaracao {$$ = $1;};

declaracao : vardeclaracao {$$ = $1;}
		   | fundeclaracao {$$ = $1;};

vardeclaracao : tipoespecificador ID PNTVIRG {$$ = criaDecl(vari, @2.first_line);
												$$->key.nome = strdup($2);
												free($2);
			  									$$->filho[0] = $1;
												$$->especial = 0;}
			 | tipoespecificador ID COLX_OP NUM COLX_CL PNTVIRG {$$ = criaDecl(vari, @2.first_line);
                                                                	$$->filho[0] = $1;
																	$$->filho[1] = criaEnd(cons, @4.first_line);
																	$$->filho[1]->key.val = $4;
																	$$->key.nome = strdup($2);
																	free($2);
																	$$->especial = 1;};

tipoespecificador : INT {$$ = criaEnd(int_t, @1.first_line);}
				  | VOID {$$ = criaEnd(void_t, @1.first_line);};

fundeclaracao : tipoespecificador ID PARNTS_OP params PARNTS_CL compostodecl{$$ = criaDecl(func, @2.first_line);
			  																	$$->key.nome = strdup($2);
																				free($2);
			  																	$$->filho[0] = $1;
																				$$->filho[1] = $4;
																				$$->filho[2] = $6;};

params : paramlista {$$ = $1;}
	   | VOID {$$ = criaEnd(void_t, @1.first_line);};

paramlista : paramlista VIRG param {treeNode* t = $1;
		   							while(t->irmao != NULL){
										t = t->irmao;
									} t->irmao = $3;}
		   | param {$$ = $1;};

param : tipoespecificador ID {$$ = criaDecl(vari, @2.first_line);
	  							$$->key.nome = strdup($2);
								free($2);
								$$->filho[0] = $1;
								$$->especial = 0;}
	  | tipoespecificador ID COLX_OP COLX_CL {$$ = criaDecl(vari, @2.first_line);
	  											$$->filho[0] = $1;
												$$->key.nome = strdup($2);
												free($2);
												$$->especial = 1;};

compostodecl : CHAVES_OP localdeclaracoes statementlista CHAVES_CL {treeNode *t = $2;
			 														if(t != NULL){
																		while(t->irmao != NULL)
																			t= t->irmao;
																		t->irmao = $3;
																		$$ = $2;
																	}else $$ = $3;};

localdeclaracoes : localdeclaracoes vardeclaracao {treeNode *t = $1;
				 									if(t != NULL){
														while(t->irmao != NULL){
															t = t-> irmao;
														} t-> irmao = $2;
													}else $$ = $2;}
				 | %empty {$$ = NULL;};

statementlista : statementlista statement {treeNode* t = $1;
			   								if(t != NULL){
												while(t->irmao != NULL){
													t = t-> irmao;
												} t-> irmao = $2;
											}else $$ = $2;}
			   | %empty {$$ = NULL;};

statement : expressaodecl {$$ = $1;}
		  | compostodecl {$$ = $1;}
		  | selecaodecl {$$ = $1;}
		  | iteracaodecl {$$ = $1;}
		  | retornodecl {$$ = $1;};

expressaodecl : expressao PNTVIRG {$$ = $1;}
			  | PNTVIRG {$$ = NULL;};
			  
selecaodecl : IF PARNTS_OP expressao PARNTS_CL statement {$$ = criaDecl(selec, @1.first_line);
															$$->filho[0] = $3;
															$$->filho[1] = $5;}
			| IF PARNTS_OP expressao PARNTS_CL statement ELSE statement{$$ = criaDecl(selec, @1.first_line);
																		$$->filho[0] = $3;
																		$$->filho[1] = $5;
																		$$->filho[2] = $7;
																		$$->especial = 1;};

iteracaodecl : WHILE PARNTS_OP expressao PARNTS_CL statement{$$ = criaDecl(iterac, @1.first_line);
			 													$$->filho[0] = $3;
																$$->filho[1] = $5;};

retornodecl : RETURN PNTVIRG {$$ = criaDecl(reto, @1.first_line);}
			| RETURN expressao PNTVIRG {$$ = criaDecl(reto, @1.first_line);
										$$->filho[0] = $2;
										$$->especial = 1;};

expressao : var IGUAL expressao {$$ = criaExp(oper, @2.first_line);
		  							$$->key.op = $2;
									$$->filho[0] = $1;
									$$->filho[1] = $3;}
		  | simplesexpressao ;

var : ID {$$ = criaExp(uso, @1.first_line);
			$$->key.nome = strdup($1);
			free($1);}
	| ID COLX_OP expressao COLX_CL {$$ = criaExp(uso, @1.first_line);
									$$->key.nome = strdup($1);
									free($1);
									$$->especial = 1;
									$$->filho[0] = $3;};

simplesexpressao : somaexpressao relacional somaexpressao {$$ = criaExp(oper, @2.first_line);
				 											$$->filho[0] = $1;
															$$->filho[1] = $3;
															$$->key.op = $2;}
				 | somaexpressao {$$ = $1;};

relacional : MENORIGUAL {$$ = $1;}
		   | MENOR {$$ = $1;}
		   | MAIORIGUAL {$$ = $1;}
		   | MAIOR {$$ = $1;}
		   | IGUALIGUAL {$$ = $1;}
		   | DIFERENTE {$$ = $1;};

somaexpressao : somaexpressao soma termo {$$ = criaExp(oper, @2.first_line);
			  								$$->filho[0] = $1;
											$$->filho[2] = $3;
											$$->key.op = $2;}
			  | termo {$$ = $1;};

soma : ADI {$$ = $1;}
	 | SUB {$$ = $1;};

termo : termo multi fator {$$ = criaExp(oper, @2.first_line);
	                       $$->filho[0] = $1;
						   $$->filho[1] = $3;
						   $$->key.op = $2;}
	  | fator {$$ = $1;};

multi : MULT {$$ = $1;}
	 | DIV {$$ = $1;};

fator : PARNTS_OP expressao PARNTS_CL {$$ = $2;}
	  | var {$$ = $1;}
	  | ativacao {$$ = $1;}
	  | NUM {$$ = criaEnd(cons, @1.first_line);
	  			$$->key.val = $1;};

ativacao : ID PARNTS_OP args PARNTS_CL {$$ = criaExp(atv, @1.first_line);
		 								$$->filho[0] = $3;
										$$->key.nome = strdup($1);
										free($1);};

args : arglista {$$ = $1;}
	 | %empty {$$ = NULL;};

arglista : arglista VIRG expressao {$1->irmao = $3;}
		 | expressao {$$ = $1;};

%%

int yyerror(char* msg){
    fprintf(saida, "Erro sintatico: %s, linha: %d\n", yytext, yylloc.first_line);
	error = 1;
    return 1;
}

treeNode *parse(){
    yyparse();
	
	return newTree;
}

