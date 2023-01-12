#ifndef MYGLOBALS_H
#define MYGLOBALS_H

//algumas variaveis e definicoes usadas em todo programa

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *source;
extern FILE *saida;
extern unsigned char error;

typedef enum{maig, meig, igig, dife, me, ma, mult, divs, adi, sub, ig} opType;

void flex_end();

#endif

