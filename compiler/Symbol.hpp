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

	Record(std::vector<std::pair<std::vector<std::string>, Type*> >* recordItems) {
		name = "Record";

		int offset = 0;

		for(int i = 0; i < recordItems->size(); i++) {
			std::pair<std::vector<std::string>, Type*> recordItem = (*recordItems)[i];

			for(int j = 0; j < recordItem.first.size(); j++) {
				std::pair<Type*, int> recordMapItem = std::make_pair(recordItem.second, offset);

				offset += recordItem.second->size;
				
				recordMap[recordItem.first[j]] = recordMapItem;
			}
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

	Array(Const* lower, Const* up, Type* type) {
		this->name = "NO_NAME";

		this->low = dynamic_cast<Int*>(lower)->val;
		this->upper = dynamic_cast<Int*>(up)->val;

		this->size = type->size*(upper-low+1);

		this->type = type;
	}
};

class Func : public Symbol {
public:
	std::vector<std::pair<std::string, Type*> > signature;
	Type* returnType;

	bool isForward;

	Func(std::string identifier, std::vector<std::pair<std::vector<std::string>, Type*> >* formalParams, Type* returnType) {
		std::cout << "Function... Name: " << identifier << " ReturnType: " << returnType << " Params: ";

		this->name = identifier;
		this->returnType = returnType;

		if(formalParams) {

			for(int i = 0; i < formalParams->size(); i++) {
				std::pair<std::vector<std::string>, Type*> formalParam = (*formalParams)[i];
				Type* type = formalParam.second;
				std::vector<std::string> identifiers = formalParam.first;

				for(int j = 0; j < identifiers.size(); j++) {
					std::cout << " Ident: " << identifiers[j] << " Type: " << type;
					std::pair<std::string, Type*> thePair = std::make_pair(identifiers[j], type);

					signature.push_back(thePair);
				}
			}
		}
		std::cout << std::endl;
	}	
};

class Proc : public Symbol {
public:
	std::vector<std::pair<std::string, Type*> > signature;

	bool isForward;

	Proc(std::string identifier, std::vector<std::pair<std::vector<std::string>, Type*> >* formalParams) {
		std::cout << "Procedure... Name: " << identifier << " Params: ";

		this->name = identifier;

		if(formalParams) {

			for(int i = 0; i < formalParams->size(); i++) {
				std::pair<std::vector<std::string>, Type*> formalParam = (*formalParams)[i];
				Type* type = formalParam.second;
				std::vector<std::string> identifiers = formalParam.first;

				for(int j = 0; j < identifiers.size(); j++) {
					std::cout << " Ident: " << identifiers[j] << " Type: " << type;
					std::pair<std::string, Type*> thePair = std::make_pair(identifiers[j], type);

					signature.push_back(thePair);
				}
			}
		}
		std::cout << std::endl;
	}	
};

class Table {
public: 
	std::map<std::string, Symbol*> tableMap;


	Table() {
		
	}

	void add(std::string identifier, Symbol* symbol) {
		std::cout << "Adding to table...\n ";
		std::cout << "  Identifier: " << identifier << " Symbol: " << symbol << std::endl;


		if(tableMap.find(identifier) != tableMap.end()) {
			std::cout << "We alread have the symbol (" << identifier << ") in the table. I quit!!!!" << std::endl;
			exit(1);
		}

		this->tableMap[identifier] = symbol;
	}

	Symbol* lookup(std::string name) {
		if(tableMap.find(name) != tableMap.end()) {
			return tableMap.find(name)->second;
		} else {
			return NULL;
		}
	}

	void print() {
		std::cout << "Here is the table:\n";

		std::map<std::string, Symbol*>::iterator iter;

		for(iter = tableMap.begin(); iter != tableMap.end(); iter++) {
			std::cout << " Name: " << iter->first << " Symbol: " << iter->second << std::endl;
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

			symbolTableInstance->symbolTable.push_back(initializedMainTable());
			std::cout << "Making inital Table...\n";
			symbolTableInstance->symbolTable.back().print();

		}
		return symbolTableInstance;
	}
/* End Singleton Stuff */

public:
	std::vector<Table> symbolTable; 

	static void add(std::string identifier, Symbol* symbol) {
		SymbolTable* tableInstance = getInstance();

		tableInstance->symbolTable.back().add(identifier, symbol);
	}

	static Symbol* lookup(std::string name) {
		std::cout << "Looking up\n";
		SymbolTable* tableInstance = getInstance();

		for(int i = tableInstance->symbolTable.size()-1; i >= 0; i--) {
			Symbol* symbol = tableInstance->symbolTable[i].lookup(name);
			std::cout << "Finding Symbol: " << i << " Name: " << name << " Symbol: " << symbol << std::endl;
			if(symbol) {
				return symbol;
			}
		}
		
		std::cout << "The symbol(" << name << ") is not in the table so I will die." << std::endl;
		exit(1);
		
		return NULL;
	}


	static Table initializedMainTable() {
		Table table;

		SimpleType* integer = new SimpleType("integer");
		SimpleType* character = new SimpleType("char");
		SimpleType* str = new SimpleType("string");
		SimpleType* boolean = new SimpleType("boolean");

		table.add("integer", integer);
		table.add("INTEGER", integer);
		table.add("char", character);
		table.add("CHAR", character);
		table.add("string", str);
		table.add("STRING", str);
		table.add("boolean", boolean);
		table.add("BOOLEAN", boolean);

		return table;
	}

	static void addVar(std::vector<std::string>* identList, Type* type) {
		for(int i = 0; i < identList->size(); i++) {
			add((*identList)[i], type);
		}
	}

	static void constDecl(std::string identifier, Const* constExpression) {
		add(identifier, constExpression);
	}

	static void typeDecl(std::string identifier, Type* type) {
		add(identifier, type);
	}

	static void funcDecl(std::string identifier, Func* func) {
		std::cout << "Adding Function..." << std::endl;
		add(identifier, func);

		addNewScope(func);
	}

	static void addNewScope(Func* func) {
		SymbolTable* tableInstance = getInstance();

		Table table;
		std::cout << "Making new Table..." << std::endl;
		tableInstance->symbolTable.push_back(table);
		std::cout << "table made\n";
	}

	static void procDecl(std::string identifier, Proc* proc) {
		add(identifier, proc);
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

	static std::vector<std::pair<std::vector<std::string>, Type*> >* makeRecordItem(std::vector<std::string>* identList, Type* type, std::vector<std::pair<std::vector<std::string>, Type*> >* recordItem) {
		if(recordItem == NULL) {
			recordItem = new std::vector<std::pair<std::vector<std::string>, Type*> >;
		}

		std::pair<std::vector<std::string>, Type*> thePair = std::make_pair(*identList, type);

		recordItem->push_back(thePair);

		return recordItem;
	}
};

#endif