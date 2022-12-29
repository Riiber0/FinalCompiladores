%{
	#include "analizadorsintatico.tab.h"
	#include "myglobals.h"

	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

	int line_cont = 1;
	FILE* line_r;
	size_t line_size;
	char* line_buf = NULL;

	void printa_linha(){
		getline(&line_buf, &line_size, line_r);
		printf("%d: %s", line_cont, line_buf);
	}

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
{NUM} { printf("%d: NUM | %s\n", line_cont, yytext);
	yylval.num = atoi(yytext); return NUM;} 
{IF} { printf("%d: IF | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return IF;} 
{ELSE} { printf("%d: ELSE | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return ELSE;} 
{INT} { printf("%d: INT | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return INT;} 
{RETURN} { printf("%d: RETURN | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return RETURN;} 
{VOID} { printf("%d: VOID | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return VOID;} 
{WHILE} { printf("%d: WHILE | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return WHILE;} 
{ID} { printf("%d: ID | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return ID;} 
{MAIORIGUAL} { printf("%d: MAIORIGUAL | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return MAIORIGUAL;} 
{MENORIGUAL} { printf("%d: MENORIGUAL | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return MENORIGUAL;} 
{IGUALIGUAL} { printf("%d: IGUALIGUAL | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return IGUALIGUAL;} 
{DIFERENTE} { printf("%d: DIFERENTE | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return DIFERENTE;} 
{MENOR} { printf("%d: MENOR | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return MENOR;} 
{MAIOR} { printf("%d: MAIOR | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return MAIOR;} 
{MULT} { printf("%d: MULT | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return MULT;} 
{DIV} { printf("%d: DIV | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return DIV;} 
{ADI} { printf("%d: ADI | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return ADI;} 
{SUB} { printf("%d: SUB | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return SUB;} 
{PARNTS_OP} { printf("%d: PARNTS_OP | %s\n", line_cont, yytext);
	yylval.ch = yytext[0]; return PARNTS_OP;} 
{PARNTS_CL} { printf("%d: PARNTS_CL | %s\n", line_cont, yytext);
	yylval.ch = yytext[0]; return PARNTS_CL;} 
{COLX_OP} { printf("%d: COLX_OP | %s\n", line_cont, yytext);
	yylval.ch = yytext[0]; return COLX_OP;} 
{COLX_CL} { printf("%d: COLX_CL | %s\n", line_cont, yytext);
	yylval.ch = yytext[0]; return COLX_CL;} 
{CHAVES_OP} { printf("%d: CHAVES_OP | %s\n", line_cont, yytext);
	yylval.ch = yytext[0]; return CHAVES_OP;} 
{CHAVES_CL} { printf("%d: CHAVES_CL | %s\n", line_cont, yytext);
	yylval.ch = yytext[0]; return CHAVES_CL;} 
{IGUAL} { printf("%d: IGUAL | %s\n", line_cont, yytext);
	yylval.str = strdup(yytext); return IGUAL;} 
{PNTVIRG} { printf("%d: PNTVIRG | %s\n", line_cont, yytext);
	yylval.ch = yytext[0]; return PNTVIRG;} 
{VIRG} { printf("%d: VIRG | %s\n", line_cont, yytext);
	yylval.ch = yytext[0]; return VIRG;} 

\n	line_cont++;
[ \n\t]
\/\* comment();
		
<<EOF>> return 0;

. printf("ERRO LEXICO: %s", yytext); exit(0);
%%

void comment(){

	char c;
	unsigned char fim = 0;
	unsigned char line = 0;
	while(fim != 2){

		c = input();
		if(c == EOF) break;
		else if(c == '/'){
			if(fim) fim++;
			else if(!line)line++;
		}
		else if(c == '*' && !fim) fim++;
		else if(c == 'n' && line){
			line_cont++;
			line = 0;
		}
		else {
			line = 0;
			fim = 0;
		}

	}

}

int getToken(void){
	yyin = source;
	return yylex();
}
