#include "Symbol.hpp"


std::map<std::string, Symbols>& SymbolTab::getCurrentTable(){
	if(tables.empty()){
		std::map<std::string, Symbols> table;

		tables.push_back(table);

		return tables.back();
	} else {
		return tables.back();
	}
}

void SymbolTab::printTable(const std::map<std::string, Symbols>&){

}

void SymbolTab::pop(){
	printTable(tables.back());
	tables.pop_back();
}

void SymbolTab::push(){
	std::map<std::string, Symbols> table;

	tables.push_back(table);
}

void SymbolTab::addFuncProc(std::string name, std::vector<std::pair<std::string, std::string> > sig){
	std::map<std::string, Symbols> table = getCurrentTable();

	if(table.find(name) == table.end()){
		yyerror("The symbol already exists.");
	} else {
		Symbols funcProc;
		funcProc.type = FUNC_PROC;
		funcProc.name = name;
		funcProc.sig = sig;

		table[name] = funcProc;
	}
}
void SymbolTab::addConst(std::string name, std::string sVal){
	std::map<std::string, Symbols> table = getCurrentTable();

	if(table.find(name) == table.end()){
		yyerror("The symbol already exists.");
	} else {
		Symbols cons;
		cons.type = CONST;
		cons.name = name;
		cons.sVal = sVal;

		table[name] = cons;
	}
}
void SymbolTab::addConst(std::string name, int iVal){
	std::map<std::string, Symbols> table = getCurrentTable();

	if(table.find(name) == table.end()){
		yyerror("The symbol already exists.");
	} else {
		Symbols cons;
		cons.type = CONST;
		cons.name = name;
		cons.iVal = iVal;

		table[name] = cons;
	}
}
void SymbolTab::addConst(std::string name, bool bVal){
	std::map<std::string, Symbols> table = getCurrentTable();

	if(table.find(name) == table.end()){
		yyerror("The symbol already exists.");
	} else {
		Symbols cons;
		cons.type = CONST;
		cons.name = name;
		cons.bVal = bVal;

		table[name] = cons;
	}
}
void SymbolTab::addVar(std::string name, int iVal){
	std::map<std::string, Symbols> table = getCurrentTable();

	if(table.find(name) == table.end()){
		yyerror("The symbol already exists.");
	} else {
		Symbols cons;
		cons.type = CONST;
		cons.name = name;
		cons.bVal = iVal;

		table[name] = cons;
	}
}
void SymbolTab::addType(std::string name, int size, std::vector<std::pair<std::string, int> > layout){
	std::map<std::string, Symbols> table = getCurrentTable();

	if(table.find(name) == table.end()){
		yyerror("The symbol already exists.");
	} else {
		Symbols type;
		type.type = TYPE;
		type.name = name;
		type.size = size;
		type.layout = layout;

		table[name] = type;
	}
}

void SymbolTab::addToTypeList(std::string name, std::string value){
	typeList.push_back(make_pair(name, value));
}