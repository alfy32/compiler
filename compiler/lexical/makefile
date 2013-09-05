all: lex

lex: m2.l
	flex m2.l
	g++ lex.yy.c -o lex

run: lex
	./lex test

clean:
	rm -f lex.yy.c
	rm -f *.o
	