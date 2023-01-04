all: sintatico lexico final clean

sintatico: $(wildcard *.y)
	bison $< -d

lexico: $(wildcard *.l)
	flex $<

*.o: $(wildcard *.c) 
	gcc $< -c $@

final: *.c
	gcc $^ -lfl -o teste

clean: 
	rm *.o
	rm *.tab.* *.yy.*
