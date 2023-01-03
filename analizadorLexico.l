%{
	#include "analizadorsintatico.tab.h"
	#include "myglobals.h"

	#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;

	FILE* line_r;
	size_t line_size;
	char* line_buf = NULL;

	void printa_linha(){
		getline(&line_buf, &line_size, line_r);
		printf("%d: %s", yylineno, line_buf);
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
{NUM} { printf("%d: NUM | %s\n", yylineno, yytext);
	yylval.num = atoi(yytext); return NUM;} 
{IF} { printf("%d: IF | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return IF;} 
{ELSE} { printf("%d: ELSE | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return ELSE;} 
{INT} { printf("%d: INT | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return INT;} 
{RETURN} { printf("%d: RETURN | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return RETURN;} 
{VOID} { printf("%d: VOID | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return VOID;} 
{WHILE} { printf("%d: WHILE | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return WHILE;} 
{ID} { printf("%d: ID | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return ID;} 
{MAIORIGUAL} { printf("%d: MAIORIGUAL | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return MAIORIGUAL;} 
{MENORIGUAL} { printf("%d: MENORIGUAL | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return MENORIGUAL;} 
{IGUALIGUAL} { printf("%d: IGUALIGUAL | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return IGUALIGUAL;} 
{DIFERENTE} { printf("%d: DIFERENTE | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return DIFERENTE;} 
{MENOR} { printf("%d: MENOR | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return MENOR;} 
{MAIOR} { printf("%d: MAIOR | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return MAIOR;} 
{MULT} { printf("%d: MULT | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return MULT;} 
{DIV} { printf("%d: DIV | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return DIV;} 
{ADI} { printf("%d: ADI | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return ADI;} 
{SUB} { printf("%d: SUB | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return SUB;} 
{PARNTS_OP} { printf("%d: PARNTS_OP | %s\n", yylineno, yytext);
	yylval.ch = yytext[0]; return PARNTS_OP;} 
{PARNTS_CL} { printf("%d: PARNTS_CL | %s\n", yylineno, yytext);
	yylval.ch = yytext[0]; return PARNTS_CL;} 
{COLX_OP} { printf("%d: COLX_OP | %s\n", yylineno, yytext);
	yylval.ch = yytext[0]; return COLX_OP;} 
{COLX_CL} { printf("%d: COLX_CL | %s\n", yylineno, yytext);
	yylval.ch = yytext[0]; return COLX_CL;} 
{CHAVES_OP} { printf("%d: CHAVES_OP | %s\n", yylineno, yytext);
	yylval.ch = yytext[0]; return CHAVES_OP;} 
{CHAVES_CL} { printf("%d: CHAVES_CL | %s\n", yylineno, yytext);
	yylval.ch = yytext[0]; return CHAVES_CL;} 
{IGUAL} { printf("%d: IGUAL | %s\n", yylineno, yytext);
	yylval.str = strdup(yytext); return IGUAL;} 
{PNTVIRG} { printf("%d: PNTVIRG | %s\n", yylineno, yytext);
	yylval.ch = yytext[0]; return PNTVIRG;} 
{VIRG} { printf("%d: VIRG | %s\n", yylineno, yytext);
	yylval.ch = yytext[0]; return VIRG;} 

\n	
[ \n\t]
\/\* comment();
		
<<EOF>> return 0;

. printf("ERRO LEXICO: %s, linha: %d\n", yytext, yylineno); error = 1;
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
			//line_cont++;
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
