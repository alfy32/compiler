#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>

int yyerror(std::string error) {
	std::cout << "Symbol Error: " << error << std::endl;
}

enum SymbolType {FUNC_PROC, CONST, VAR, TYPE};

struct Symbols {
	std::string name;
	SymbolType type;
	
	/*** function/procedure ***/
	std::vector<std::pair<std::string, std::string> > sig;

	/*** Const ***/
	std::string sVal;
	int iVal;
	char cVal;
	bool bVal;


	int location;

	/*** Var ***/

	/*** Type ***/
	int size;
	std::vector<std::pair<std::string, int> > layout;

};

class SymbolTab {
	static std::vector<std::map<std::string, Symbols> > tables;

	static std::map<std::string, Symbols>& getCurrentTable();

	static void printTable(const std::map<std::string, Symbols>&);

	static std::std::vector<std::pair<std::string, std::string> > typeList;

public:
	static void pop();
	static void push();

	// void addFuncProc(name, sig)
	static void addFuncProc(std::string, std::vector<std::pair<std::string, std::string> >);
	// void addConst(name, value)
	static void addConst(std::string, std::string);
	static void addConst(std::string, int);
	static void addConst(std::string, bool);
	// void addVar(name, location)
	static void addVar(std::string, int);
	// void addType(name, location, layout)
	static void addType(std::string, int, std::vector<std::pair<std::string, int> >);

	// formalParams makeFormalParams(identList, type);
	// formalParams makeFormalParams(identList, type, formalParams);

	// type makeRecoredType(recordItem)
	// recordItem makeRecordItem(identList, type, recordItem)

	// type makeArrayType(constExpr, constExpr, type)

};

#endif