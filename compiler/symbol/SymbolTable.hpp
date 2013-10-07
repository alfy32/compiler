#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <map>
#include <memory>

#include "Symbol.hpp"
#include "cpsl.tab.h"

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
	std::map<std::string, std::shared_ptr<Symbol> > table;

public:
	std::shared_ptr<Symbol> lookup(std::string) {
		
	}
	void add(std::shared_ptr<Symbol>);


	static makeFormalParams(std::vector<std::string>, Type type) {

	}

	static makeFormalParams(std::vector<std::string>, Type type, ) {
		
	}

	static std::vector<std::string>* makeIdentList(std::string ident) {
		return new std::vector<std::string>(ident);
	}

	static std::vector<std::string>* makeIdentList(std::string ident, std::vector<std::string>* identList) {
		identList->push_back(ident);
		return identList;
	}	

	static void addType(std::string, Type*) {

	}

	static evaluateConstExpression(int, int, int);
	static evaluateConstExpression(std::string, int, std::string);
	static evaluateConstExpression(Const, int, Const);
};

#endif
