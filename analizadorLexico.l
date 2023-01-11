%{
	#include "analizadorsintatico.tab.h"
	#include "myglobals.h"

	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

	void comment();
	
%}
%option yylineno

DIGIT [0-9]
NUM {DIGIT}{DIGIT}*
LETRA [a-zA-Z]
ID {LETRA}{LETRA}*
IF if
ELSE else
INT int
RETURN return
VOID void
WHILE while
MAIORIGUAL <=
MENORIGUAL >=
IGUALIGUAL ==
DIFERENTE !=
MENOR <
MAIOR >
MULT \*
DIV \/
ADI \+
SUB \-
PARNTS_OP \(
PARNTS_CL \)
COLX_OP \[
COLX_CL \]
CHAVES_OP \{
CHAVES_CL \}
IGUAL \=
PNTVIRG ;
VIRG ,
%%
{NUM} { fprintf(saida, "%d: NUM | %s\n", yylineno, yytext);
	yylval.num = atoi(yytext); return NUM;} 
{IF} { fprintf(saida, "%d: IF | %s\n", yylineno, yytext);
	 return IF;} 
{ELSE} { fprintf(saida, "%d: ELSE | %s\n", yylineno, yytext);
	return ELSE;} 
{INT} { fprintf(saida, "%d: INT | %s\n", yylineno, yytext);
	return INT;} 
{RETURN} { fprintf(saida, "%d: RETURN | %s\n", yylineno, yytext);
	return RETURN;} 
{VOID} { fprintf(saida, "%d: VOID | %s\n", yylineno, yytext);
	return VOID;} 
{WHILE} { fprintf(saida, "%d: WHILE | %s\n", yylineno, yytext);
	return WHILE;} 
{ID} { fprintf(saida, "%d: ID | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return ID;} 
{MAIORIGUAL} { fprintf(saida, "%d: MAIORIGUAL | %s\n", yylineno, yytext);
	yylval.op = maig; return MAIORIGUAL;} 
{MENORIGUAL} { fprintf(saida, "%d: MENORIGUAL | %s\n", yylineno, yytext);
	yylval.op = meig; return MENORIGUAL;} 
{IGUALIGUAL} { fprintf(saida, "%d: IGUALIGUAL | %s\n", yylineno, yytext);
	yylval.op = igig; return IGUALIGUAL;} 
{DIFERENTE} { fprintf(saida, "%d: DIFERENTE | %s\n", yylineno, yytext);
	yylval.op = dife; return DIFERENTE;} 
{MENOR} { fprintf(saida, "%d: MENOR | %s\n", yylineno, yytext);
	yylval.op = me; return MENOR;} 
{MAIOR} { fprintf(saida, "%d: MAIOR | %s\n", yylineno, yytext);
	yylval.op = ma; return MAIOR;} 
{IGUAL} { fprintf(saida, "%d: IGUAL | %s\n", yylineno, yytext);
	yylval.op = ig; return IGUAL;} 
{MULT} { fprintf(saida, "%d: MULT | %s\n", yylineno, yytext);
	yylval.op = mult; return MULT;} 
{DIV} { fprintf(saida, "%d: DIV | %s\n", yylineno, yytext);
	yylval.op = divs; return DIV;} 
{ADI} { fprintf(saida, "%d: ADI | %s\n", yylineno, yytext);
	yylval.op = adi; return ADI;} 
{SUB} { fprintf(saida, "%d: SUB | %s\n", yylineno, yytext);
	yylval.op = sub; return SUB;} 
{PARNTS_OP} { fprintf(saida, "%d: PARNTS_OP | %s\n", yylineno, yytext);
	return PARNTS_OP;} 
{PARNTS_CL} { fprintf(saida, "%d: PARNTS_CL | %s\n", yylineno, yytext);
	return PARNTS_CL;} 
{COLX_OP} { fprintf(saida, "%d: COLX_OP | %s\n", yylineno, yytext);
	return COLX_OP;} 
{COLX_CL} { fprintf(saida, "%d: COLX_CL | %s\n", yylineno, yytext);
	 return COLX_CL;} 
{CHAVES_OP} { fprintf(saida, "%d: CHAVES_OP | %s\n", yylineno, yytext);
	return CHAVES_OP;} 
{CHAVES_CL} { fprintf(saida, "%d: CHAVES_CL | %s\n", yylineno, yytext);
	return CHAVES_CL;} 
{PNTVIRG} { fprintf(saida, "%d: PNTVIRG | %s\n", yylineno, yytext);
	return PNTVIRG;} 
{VIRG} { fprintf(saida, "%d: VIRG | %s\n", yylineno, yytext);
	return VIRG;} 

\n	
[ \n\t]
\/\* comment();
		
<<EOF>> return 0;

. {fprintf(saida, "ERRO LEXICO: %s, linha: %d\n", yytext, yylineno); error = 1;}
%%

void comment(){

	char c;
	unsigned char fim = 0;
	while(fim != 2){

		c = input();
		if(c == EOF) break;
		else if(c == '/' && fim)
			fim++;
		else if(c == '*' && !fim) fim++;
		else {
			fim = 0;
		}

	}

}

void flex_end(){
	yy_delete_buffer(YY_CURRENT_BUFFER);
	yylex_destroy();
}

