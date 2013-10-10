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

	Symbol() {}

	Symbol(std::string identifier) {
		name = identifier;
	}

	virtual void print() {
		std::cout << "Symbol: " << name << std::endl;
	}
};

enum ConstType { CONST_INT, CONST_STR, CONST_CHAR }; 

class Const : public Symbol {
public:
	ConstType constType;

	Const() {}

	Const(Const* left, std::string op, Const* right) {
		name = "Operation";
	}

	Const(std::string op, Const* right) {
		name = "Operation";
	}

	Const(std::string identifier) {
		name = identifier;
	}

	virtual void print() {
		std::cout << "Const: " << name << std::endl;
	}
};

class String : public Const {
public:
	std::string val;

	String(char* value) {
		val = value;
	}

	void print() {
		std::cout << "String Const: " << name << " Value: " << val << std::endl;
	}
};

class Char : public Const {
public:
	std::string val;

	Char(char* value) {
		val = value;
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
	}

	void print() {
		std::cout << "Int Const: " << name << " Value: " << val << std::endl;
	}
};

class Type : public Symbol {
public:
	int size;

	Type() {}

	Type(std::string identifier) {
		name = identifier;
	}

	virtual void print() {
		std::cout << "Type: " << name << " Size: " << size << std::endl;
	}
};

class SimpleType : public Type {
public:

	SimpleType() {
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

	Record(std::vector<Type*>* types) {
		for(int i = 0; i < types->size(); i++) {
			//recordMap[(*types)[i]->name] = std::make_pair<Type*, int>((*types)[i], types[i].size);
		}
	}

	virtual void print() {
		std::cout << "Record Type: " << std::endl;

		for(auto iter = recordMap.begin(); iter != recordMap.end(); iter++) {
			// auto thePair = iter->second;

			// std::cout << "  Offset: " << thePair.second; 
			// thePair.first->print();
		}
	}
};

class Array : public Type {
public:
	Type* type;
	int low;
	int upper;

	Array() {}

	Array(Const* lower, Const* upper, Type* type) {
		this->type = type;
		// this->low = dynamic_cast<Int*>(lower)->val;
		// this->upper = dynamic_cast<Int*>(upper)->val;
	}
};

class Table {
	std::map<std::string, Symbol*> tableMap;

public:
	void add(Symbol* symbol) {
		// std::cout << "Adding to table: ";
		// symbol->print();


		// if(tableMap.find(symbol->name) != tableMap.end()) {
		// 	std::cout << "We alread have that symbol in the table. I quit!!!!" << std::endl;
		// 	exit(1);
		// }

		// this->tableMap[name] = symbol;

		// std::cout << "Adding to table: ";
		// symbol->print();
	}

	Symbol* lookup(std::string name) {
		if(tableMap.find(name) != tableMap.end()) {
			return tableMap.find(name)->second;
		} else {
			std::cout << "That symbol doesn't exist. I'm going to die!" << std::endl;
		}
	}
};

class SymbolTable {
	/* Singleton Stuff */
private:
	static std::shared_ptr<SymbolTable> symbolTableInstance;

public:
	static std::shared_ptr<SymbolTable> getInstance() {
		if(!symbolTableInstance){
			symbolTableInstance = std::make_shared<SymbolTable>();
		}
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
		constExpression->print();

		auto tableInstance = getInstance();
		tableInstance->symbolTable[0].add(constExpression);
	}

	static void typeDecl(std::string identifier, Type* type) {
		std::cout << "Type Decl: " << identifier << " Type: ";
		type->print();
	}

	static Symbol* expression(Symbol* left, void*, Symbol* right){

		return left;
	}

	static Symbol* expression(void*, Symbol* right){

		return right;
	}

	static Symbol* expressionLvalue(int){

		return new Symbol();
	}

	static Symbol* function_call(std::string identifier) {

		return new Symbol(identifier);
	}

	static Symbol* function_call(std::string identifier, std::vector<Symbol*>*) {

		return NULL;
	}

	static Symbol* chr(Symbol* symbol) {

		return NULL;
	}

	static Symbol* ord(Symbol* symbol) {

		return NULL;
	}

	static Symbol* pred(Symbol* symbol) {

		return NULL;
	}

	static Symbol* succ(Symbol* symbol) {

		return NULL;
	}

	static std::vector<Symbol*> * makeSymbolVector(Symbol* symbol, std::vector<Symbol*>* symbols) {
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
	}
};

#endif