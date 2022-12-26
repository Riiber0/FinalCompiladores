%{
    #include "myglobals.h"

    extern int line_cont;
    FILE* source;

%}

%token NUM ID
%token IF ELSE INT RETURN VOID WHILE
%token MAIORIGUAL MENORIGUAL IGUALIGUAL DIFERENTE MENOR MAIOR
%token MULT DIV ADI SUB
%token PARNTS_OP PARNTS_CL COLX_OP COLX_CL CHAVES_OP CHAVES_CL
%token IGUAL PNTVIRG VIRG  

%union{
    int num;
    char* str;
    char ch;
}

%%

programa : declaracaolista;

declaracaolista : declaracaolista declaracao
				| declaracao;

declaracao : vardeclaracao
		   | fundeclaracao;

vardeclaracao : tipoespecificador ID PNTVIRG
			 | tipoespecificador ID COLX_OP NUM COLX_CL PNTVIRG;

tipoespecificador : INT
				  | VOID

fundeclaracao : tipoespecificador ID PARNTS_OP params PARNTS_CL compostodecl;

params : paramlista
	   | VOID

paramlista : paramlista VIRG param
		   | param;

param : tipoespecificador ID
	  | tipoespecificador ID COLX_OP COLX_CL;

compostodecl : CHAVES_OP localdeclaracoes statementlista CHAVES_CL;

localdeclaracoes : localdeclaracoes vardeclaracao
				 | %empty;

statementlista : statementlista statement
			   | %empty;

statement : expressaodecl 
		  | compostodecl
		  | selecaodecl
		  | iteracaodecl
		  | retornodecl;

expressaodecl : expressao PNTVIRG
			  | PNTVIRG;
			  
selecaodecl : IF PARNTS_OP expressao PARNTS_CL statement
			| IF PARNTS_OP expressao PARNTS_CL statement ELSE statement;

iteracaodecl : WHILE PARNTS_OP expressao PARNTS_CL statement;

retornodecl : RETURN PNTVIRG
			| RETURN expressao PNTVIRG;

expressao : var IGUAL expressao 
		  | simplesexpressao;

var : ID 
	| ID COLX_OP expressao COLX_CL;

simplesexpressao : somaexpressao relacional somaexpressao
				 | somaexpressao

relacional : MENORIGUAL
		   | MENOR
		   | MAIORIGUAL
		   | MAIOR
		   | IGUALIGUAL
		   | DIFERENTE;

somaexpressao : somaexpressao soma termo
			  | termo;

soma : ADI
	 | SUB;

termo : termo multi fator
	  | fator;

multi : MULT
	 | DIV;

fator : PARNTS_OP expressao PARNTS_CL
	  | var
	  | ativacao
	  | NUM;

ativacao : ID PARNTS_OP args PARNTS_CL

args : arglista
	 | %empty;

arglista : arglista VIRG expressao
		 | expressao;

%%

int yyerror(char* msg){
    printf("error: %c, linha: %d\n",yylval.ch, line_cont);
    return 0;
}
/*
static int yylex(void){ 
    return getToken(); 
}*/

void parse(){
    yyparse();

}

int main(int arc, char** argv){
    source = fopen(argv[1], "r");
    extern FILE* yyin;
    yyin = source;

    parse();
}
