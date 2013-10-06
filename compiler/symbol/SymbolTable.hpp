#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <map>
#include <memory>

#include "Symbol.hpp"

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

private:	
	std::map<std::string, std::shared_ptr<Symbol> > table;

public:
	std::shared_ptr<Symbol> lookup(std::string) {
		
	}
	void add(std::shared_ptr<Symbol>);
};

#endif
