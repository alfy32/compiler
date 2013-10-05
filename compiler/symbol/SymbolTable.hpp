#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <map>
#include <memory>

#include "Symbol.hpp"

class SymbolTable {
	std::map<std::string, std::shared_ptr<Symbol> > table;

	std::shared_ptr<Symbol> lookup(std::string);
	void add(std::shared_ptr<Symbol>);
};

#endif
