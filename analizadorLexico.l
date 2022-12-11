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
LETRA [a-zA-Z]
KEYWORDS else|if|int|return|void|while
COMPARADOR2 <=|>=|==|!=
COMPARADOR \<|\>
MULT \*|\/|
SOMA \+|\-
\(|\)
\[|\]|
|\{|\}
\=|;|,
%%
{KEYWORDS} printf("%d: KEY : %d\n", line_cont, yytext); fflush(stdout);
{DIGIT}{DIGIT}* printf("%d: NUM : %d\n", line_cont, yytext); fflush(stdout);
{LETRA}{LETRA}* printf("%d: ID : %d\n", line_cont, yytext); fflush(stdout);
{COMPARADOR2} printf("%d: COMPARADOR : %d\n", line_cont, yytext); fflush(stdout);
{COMPARADOR} printf("%d: COMPARADOR : %d\n", line_cont, yytext); fflush(stdout);
{SOMA} printf("%d: SOMA : %d\n", line_cont, yytext); fflush(stdout);
{MULT} printf("%d: MULT : %d\n", line_cont, yytext); fflush(stdout);
\n	line_cont++;
\s+|\t+
\/\* comment();
		
<<EOF>> return 0;
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
