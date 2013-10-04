all: cpsl

cpsl: bison.o lex.o main.o
	g++ -std=c++11 bison.o lex.o main.o -o cpsl

lex.o: lex.yy.c
	g++ -std=c++11 -c lex.yy.c -o lex.o

lex.yy.c: cpsl.l
	flex cpsl.l

bison.o: bison.c
	g++ -std=c++11 -c cpsl.tab.c -o bison.o

bison.c: cpsl.y
	bison -d -v cpsl.y

main.o: main.cpp
	g++ -std=c++11 -c main.cpp -o main.o


clean:
	rm -f *.o

	rm -f lex.yy.c
	rm -f lex

	rm -f cpsl.tab*
	rm -f cpsl.output

	rm -f cpsl

	rm -f *.tar

tar: clean
	tar -cvzf alanCompiler.tar ../compiler/*

test: test.cpp
	g++ -std=c++11 test.cpp -o test.o

lex: cpsl.l
	flex -f cpsl.l
	g++ lex.yy.c -o lex
	./lex test_lex

bison: cpsl.y 
	bison -d -v cpsl.y 
