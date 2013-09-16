all: lex

build: lex

lex: m2.l
	flex -f m2.l
	g++ lex.yy.c -o lex

run: 
	./lex test

clean:
	rm -f lex.yy.c
	rm -f *.o
	rm -f lex
	