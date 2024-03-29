all: clean cpsl

cpsl: symbol.o symbolTable.o bison.o lex.o main.o
	g++ -g -std=c++11 symbol.o symbolTable.o bison.o lex.o main.o -o cpsl

lex.o: lex.yy.c
	g++ -g -std=c++11 -c lex.yy.c -o lex.o

lex.yy.c: cpsl.l
	flex cpsl.l

bison.o: bison.c
	g++ -g -std=c++11 -c cpsl.tab.c -o bison.o

bison.c: cpsl.y
	bison -d -v cpsl.y

main.o: main.cpp
	g++ -g -std=c++11 -c main.cpp -o main.o

symbol.o: Symbol.hpp Symbol.cpp
	g++ -g -std=c++11 -c Symbol.cpp -o symbol.o

symbolTable.o: SymbolTable.hpp SymbolTable.cpp
	g++ -g -std=c++11 -c SymbolTable.cpp -o symbolTable.o


clean:
	rm -f *.o

	rm -f lex.yy.c
	rm -f lex

	rm -f cpsl.tab*
	rm -f cpsl.output
	rm -f cpsl_verbose.output

	rm -f cpsl

	rm -f *.tar

	rm -f *.asm

mars: cpsl.asm
	java -jar mars.jar cpsl.asm

tar: clean
	tar -cvzf alanCompiler.tar ../compiler/*

test: all
	./cpsl test/assign
	java -jar mars.jar cpsl.asm
	./cpsl test/bigTest
	java -jar mars.jar cpsl.asm
	./cpsl test/for_statements
	java -jar mars.jar cpsl.asm
	./cpsl test/function_procedure
	java -jar mars.jar cpsl.asm
	./cpsl test/if_statements
	java -jar mars.jar cpsl.asm
	./cpsl test/repeat_statements
	java -jar mars.jar cpsl.asm
	./cpsl test/while_statements
	java -jar mars.jar cpsl.asm
	./cpsl test/test_expressions
	java -jar mars.jar cpsl.asm
	./cpsl test/test_factorial
	java -jar mars.jar cpsl.asm
	./cpsl test/test_personnel
	java -jar mars.jar

array: all
	./cpsl test/array
	java -jar mars.jar cpsl.asm

record: all
	./cpsl test/record
	java -jar mars.jar cpsl.asm

bigTest: all
	./cpsl test/bigTest
	java -jar mars.jar cpsl.asm

functions: all
	./cpsl test/function_procedure
	java -jar mars.jar cpsl.asm

expressions: all
	./cpsl test/test_expressions
	java -jar mars.jar cpsl.asm

factorial: all
	./cpsl test/test_factorial
	java -jar mars.jar cpsl.asm

personnel: all
	./cpsl test/test_personnel
	java -jar mars.jar

if: test/if_statements
	./cpsl test/if_statements
	java -jar mars.jar cpsl.asm

for: all
	./cpsl test/for_statements
	java -jar mars.jar cpsl.asm

while: all
	./cpsl test/while_statements
	java -jar mars.jar cpsl.asm

repeat: all
	./cpsl test/repeat_statements
	java -jar mars.jar cpsl.asm

lex: cpsl.l
	flex -f cpsl.l
	g++ -std=c++11 lex.yy.c -o lex
	./lex test_lex

bison: cpsl.y 
	bison -d -v cpsl.y 
