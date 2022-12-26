%{
	int line_cont = 1;
	FILE* line_r;
	size_t line_size;
	char* line_buf = NULL;

	void printa_linha(){
		getline(&line_buf, &line_size, line_r);
		printf("%d: %s", line_cont, line_buf);
	}

	enum tipos {KEY};

	void comment();
	
%}
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
SOMA \+
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
{NUM} { printf("%d: NUM | %s\n", line_cont, yytext);} 
{IF} { printf("%d: IF | %s\n", line_cont, yytext);} 
{ELSE} { printf("%d: ELSE | %s\n", line_cont, yytext);} 
{INT} { printf("%d: INT | %s\n", line_cont, yytext);} 
{RETURN} { printf("%d: RETURN | %s\n", line_cont, yytext);} 
{VOID} { printf("%d: VOID | %s\n", line_cont, yytext);} 
{WHILE} { printf("%d: WHILE | %s\n", line_cont, yytext);} 
{ID} { printf("%d: ID | %s\n", line_cont, yytext);} 
{MAIORIGUAL} { printf("%d: MAIORIGUAL | %s\n", line_cont, yytext);} 
{MENORIGUAL} { printf("%d: MENORIGUAL | %s\n", line_cont, yytext);} 
{IGUALIGUAL} { printf("%d: IGUALIGUAL | %s\n", line_cont, yytext);} 
{DIFERENTE} { printf("%d: DIFERENTE | %s\n", line_cont, yytext);} 
{MENOR} { printf("%d: MENOR | %s\n", line_cont, yytext);} 
{MAIOR} { printf("%d: MAIOR | %s\n", line_cont, yytext);} 
{MULT} { printf("%d: MULT | %s\n", line_cont, yytext);} 
{DIV} { printf("%d: DIV | %s\n", line_cont, yytext);} 
{SOMA} { printf("%d: SOMA | %s\n", line_cont, yytext);} 
{SUB} { printf("%d: SUB | %s\n", line_cont, yytext);} 
{PARNTS_OP} { printf("%d: PARNTS_OP | %s\n", line_cont, yytext);} 
{PARNTS_CL} { printf("%d: PARNTS_CL | %s\n", line_cont, yytext);} 
{COLX_OP} { printf("%d: COLX_OP | %s\n", line_cont, yytext);} 
{COLX_CL} { printf("%d: COLX_CL | %s\n", line_cont, yytext);} 
{CHAVES_OP} { printf("%d: CHAVES_OP | %s\n", line_cont, yytext);} 
{CHAVES_CL} { printf("%d: CHAVES_CL | %s\n", line_cont, yytext);} 
{IGUAL} { printf("%d: IGUAL | %s\n", line_cont, yytext);} 
{PNTVIRG} { printf("%d: PNTVIRG | %s\n", line_cont, yytext);} 
{VIRG} { printf("%d: VIRG | %s\n", line_cont, yytext);} 

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
int main(int argc, char** argv){
	FILE *f_in; 
	if(!(line_r = fopen(argv[1], "r"))) return 0;

	if (argc == 2) {
		if(f_in = fopen(argv[1],"r")) yyin = f_in;
		else perror(argv[0]);
	}
	else yyin = stdin;

	yylex();
	return 0;
}
