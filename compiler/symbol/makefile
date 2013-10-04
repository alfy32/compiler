all: symbolTable.o  symbol.o  const.o var.o func.o type.o  string.o char.o int.o  simple.o record.o array.o


symbolTable.o: SymbolTable.hpp SymbolTable.cpp
	g++ -std=c++11 -c SymbolTable.cpp -o SymbolTable.o


symbol.o: Symbol.hpp Symbol.cpp
	g++ -std=c++11 -c Symbol.cpp -o Symbol.o


const.o: Const.hpp Const.cpp
	g++ -std=c++11 -c Const.cpp -o Const.o

var.o: Var.hpp Var.cpp
	g++ -std=c++11 -c Var.cpp -o Var.o

func.o: Func.hpp Func.cpp
	g++ -std=c++11 -c Func.cpp -o Func.o

type.o: Type.hpp Type.cpp
	g++ -std=c++11 -c Type.cpp -o Type.o



string.o: String.hpp String.cpp
	g++ -std=c++11 -c String.cpp -o String.o

char.o: Char.hpp Char.cpp
	g++ -std=c++11 -c Char.cpp -o Char.o

int.o: Int.hpp Int.cpp
	g++ -std=c++11 -c Int.cpp -o Int.o



simple.o: Simple.hpp Simple.cpp
	g++ -std=c++11 -c Simple.cpp -o Simple.o

record.o: Record.hpp Record.cpp
	g++ -std=c++11 -c Record.cpp -o Record.o

array.o: Array.hpp Array.cpp
	g++ -std=c++11 -c Array.cpp -o Array.o



class: class.cpp
	g++ -std=c++11 class.cpp -o class.o

clean: 
	rm -f *.o