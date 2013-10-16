all: clean cpsl

cpsl: symbol.o symbolTable.o bison.o lex.o main.o
	g++ -std=c++11 symbol.o symbolTable.o bison.o lex.o main.o -o cpsl

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

symbol.o: Symbol.hpp Symbol.cpp
	g++ -std=c++11 -c Symbol.cpp -o symbol.o

symbolTable.o: SymbolTable.hpp SymbolTable.cpp
	g++ -std=c++11 -c SymbolTable.cpp -o symbolTable.o


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

test: all
	./cpsl test/test_expressions
	./cpsl test/test_personnel
	./cpsl test/test_factorial

lex: cpsl.l
	flex -f cpsl.l
	g++ -std=c++11 lex.yy.c -o lex
	./lex test_lex

bison: cpsl.y 
	bison -d -v cpsl.y 
