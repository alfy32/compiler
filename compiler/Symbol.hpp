#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include "cpsl.h"

class Symbol {
public:
	std::string name;

	Symbol() {
		name = "NO_NAME";
	}

	Symbol(std::string identifier) {
		name = identifier;
	}

	virtual void print() {
		std::cout << "Symbol: " << name << std::endl;
	}
};

enum ConstType { CONST_INT, CONST_STR, CONST_CHAR, UNKNOWN_TYPE }; 

class Const : public Symbol {
public:
	ConstType constType;

	Const() {
		name = "NO_NAME";
		constType = UNKNOWN_TYPE;
	}

	Const(Const* left, std::string op, Const* right) {
		if(left->constType == right->constType) {
			name = left->name;
			constType = left->constType;
		} else {
			std::cout << "We failed here. These are different const types. I can't do the operation " << op << ".";
			exit(1);
		}
		name = "Operation";
	}

	Const(std::string op, Const* right) {
		if(right->constType == CONST_INT) {
			name = right->name;
			constType = right->constType;
		} else {
			name = "Operation";
			constType = UNKNOWN_TYPE;
		}
	}

	Const(std::string identifier) {
		name = identifier;
		constType = UNKNOWN_TYPE;
	}

	virtual void print() {
		std::cout << "Const: " << name << " Type: " << constType << std::endl;
	}
};

class String : public Const {
public:
	std::string val;

	String(char* value) {
		std::string makeValue(value);
		val = makeValue;

		name = "NO_NAME";
		constType = CONST_STR;
	}

	void print() {
		std::cout << "String Const: " << name << " Value: " << val << std::endl;
	}
};

class Char : public Const {
public:
	std::string val;

	Char(char* value) {
		std::string makeValue(value);
		val = makeValue;

		name = "NO_NAME";
		constType = CONST_CHAR;
	}

	void print() {
		std::cout << "Char Const: " << name << " Value: " << val << std::endl;
	}
};

class Int : public Const {
public:
	int val;

	Int(int value) {
		val = value;

		name = "NO_NAME";
		constType = CONST_INT;
	}

	void print() {
		std::cout << "Int Const: " << name << " Value: " << val << std::endl;
	}
};

class Type : public Symbol {
public:
	int size;

	Type() {
		name = "NO_NAME";
		size = 4;
	}

	Type(std::string identifier) {
		name = identifier;
		size = 4;
	}

	virtual void print() {
		std::cout << "Type: " << name << " Size: " << size << std::endl;
	}
};

class SimpleType : public Type {
public:

	SimpleType() {
		name = "NO_NAME";
		size = 4;
	}

	SimpleType(std::string identifier) {
		name = identifier;
		size = 4;
	}

	virtual void print() {
		std::cout << "Simple Type: " << name << " Size: " << size << std::endl;
	}
};

class Record : public Type {
public:
	std::map<std::string, std::pair<Type*, int> > recordMap;

	Record() {
		name = "NO_NAME";
		size = 0;
	}

	Record(std::vector<Type*>* types) {
		name = "NO_NAME";
		
		int offset = 0;

		for(int i = 0; i < types->size(); i++) {
			Type* theType = (*types)[i];

			theType->print();

			std::pair<Type*, int> thePair = std::make_pair(theType, offset);

			offset += theType->size;

			recordMap[theType->name] = thePair;
		}

		size = offset;
	}

	virtual void print() {
		std::cout << "Record Type: " << std::endl;

		// for(auto iter = recordMap.begin(); iter != recordMap.end(); iter++) {
		// 	// auto thePair = iter->second;

		// 	// std::cout << "  Offset: " << thePair.second; 
		// 	// thePair.first->print();
		// }
	}
};

class Array : public Type {
public:
	Type* type;
	int low;
	int upper;

	Array() {
		name = "NO_NAME";
		size = 0;

		low = 0;
		upper = 0;

		type = new Type();
	}

	Array(Const* lower, Const* upper, Type* type) {
		name = "NO_NAME";
		size = 0;

		low = 0;
		upper = 0;

		type = new Type();

		// this->type = type;
		// this->low = dynamic_cast<Int*>(lower)->val;
		// this->upper = dynamic_cast<Int*>(upper)->val;
	}
};

class Table {
public: 
	std::map<std::string, Symbol*> tableMap;


	Table() {
		tableMap["integer"] = new SimpleType("integer");
		tableMap["char"] = new SimpleType("char");
		tableMap["string"] = new SimpleType("string");
	}

	void add(Symbol* symbol) {
		std::cout << "Adding to table: ";
		symbol->print();


		if(tableMap.find(symbol->name) != tableMap.end()) {
			std::cout << "We alread have the symbol (" << symbol->name << ") in the table. I quit!!!!" << std::endl;
			exit(1);
		}

		this->tableMap[symbol->name] = symbol;
	}

	Symbol* lookup(std::string name) {
		if(tableMap.find(name) != tableMap.end()) {
			return tableMap.find(name)->second;
		} else {
			return NULL;
		}
	}
};

class SymbolTable {
	/* Singleton Stuff */
private:
	static SymbolTable* symbolTableInstance;

public:
	static SymbolTable* getInstance() {
		if(symbolTableInstance == NULL) {
			symbolTableInstance = new SymbolTable;
		}
		return symbolTableInstance;
	}
/* End Singleton Stuff */

public:
	std::vector<Table> symbolTable; 

	SymbolTable() {
		Table newTable;

		symbolTable.push_back(newTable);
	}


	static void constDecl(std::string identifier, Const* constExpression) {
		constExpression->name = identifier;

		SymbolTable* tableInstance = getInstance();
		tableInstance->symbolTable.back().add(constExpression);
	}

	static void typeDecl(std::string identifier, Type* type) {
		std::cout << "Type Decl: " << identifier << " Type: ";
		type->print();
	}

	static Symbol* expression(Symbol* left, void*, Symbol* right){
		std::cout << "Expression: symbol, op, symbol" << std::endl;
		return new Symbol("NO_NAME From expresssion");
	}

	static Symbol* expression(void*, Symbol* right){
		std::cout << "Expression: op, symbol" << std::endl;
		return new Symbol("NO_NAME From expresssion");
	}

	static Symbol* expressionLvalue(int){
		std::cout << "Expression: int" << std::endl;
		return new Symbol("NO_NAME From expresssion");
	}

	static Symbol* function_call(std::string identifier) {
		std::cout << "Function Call: no params." << std::endl;
		return new Symbol(identifier);
	}

	static Symbol* function_call(std::string identifier, std::vector<Symbol*>*) {
		std::cout << "Function Call: has params." << std::endl;
		return new Symbol(identifier);
	}

	static Symbol* chr(Symbol* symbol) {
		std::cout << "CHR: " << std::endl;
		return new Symbol("CHR thingy");
	}

	static Symbol* ord(Symbol* symbol) {
		std::cout << "ORD: " << std::endl;
		return new Symbol("ORD thingy");
	}

	static Symbol* pred(Symbol* symbol) {
		std::cout << "PRED: " << std::endl;
		return new Symbol("PRED thingy");
	}

	static Symbol* succ(Symbol* symbol) {
		std::cout << "SUCC: " << std::endl;
		return new Symbol("SUCC thingy");
	}

	static std::vector<Symbol*>* makeSymbolVector(Symbol* symbol, std::vector<Symbol*>* symbols) {
		if(symbols == NULL) {
			symbols = new std::vector<Symbol*>;
		}

		symbols->push_back(symbol);

		return symbols;
	}

	static std::vector<Type*>* makeTypeList(std::vector<Symbol*>* identifiers, Type* type, std::vector<Type*>* symbols) {
		if(symbols == NULL) {
			symbols = new std::vector<Type*>;
		}

		for(int i = 0; i < (int)identifiers->size(); i++) {
			Type* newType = new Type(*type);
			newType->name = (*identifiers)[i]->name;
			symbols->push_back(newType);
		}

		return symbols;
	}

	static std::vector<std::string>* makeIdentList(std::string identifier, std::vector<std::string>* identList) {
		if(identList == NULL) {
			identList = new std::vector<std::string>;
		}

		identList->push_back(identifier);

		return identList;
	}

	static std::vector<Type*>* makeRecordItem(std::vector<std::string>* identList, Type* type, std::vector<Type*>* recordItem) {
		if(recordItem == NULL) {
			recordItem = new std::vector<Type*>;
		}

		for(int i = 0; i < identList->size(); i++) {
			Type* newType = new Type(*type);

			newType->name = (*identList)[i];
			

			recordItem->push_back(newType);
		}

		return recordItem;
	}
};

#endif