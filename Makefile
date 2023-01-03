CC = gcc

all: sintatico lexico objetos final clean

sintatico: $(wildcard *.y)
	bison $< -d

lexico: $(wildcard *.l)
	flex $<

objetos: $(wildcard *.c)
	gcc $^ -c

final: $(wildcard *.o)
	gcc $^ -lfl -o teste

clean: 
	rm *.o
	rm *.tab.* *.yy.*
