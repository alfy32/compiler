all: cpsl.y cpsl.l
	bison -d cpsl.y
	flex -f cpsl.l

build: 

lex: cpsl.l
	flex -f cpsl.l
	g++ lex.yy.c -o lex

bison: cpsl.y 
	bison -d -v cpsl.y 

run: 
	./lex test_lex

clean:
	rm -f *.o

	rm -f lex.yy.c
	rm -f lex

	rm -f cpsl.tab*
	rm -f cpsl.output
	