all: sintatico lexico final

sintatico: $(wildcard *.y)
	bison $< -d

lexico: $(wildcard *.l)
	flex $<

final: $(wildcard *.c)
	gcc $^ -lfl -o teste

gdb: $(wildcard *.c)
	gcc $^ -g -lfl -o teste

clean: 
	rm *.tab.* *.yy.*
