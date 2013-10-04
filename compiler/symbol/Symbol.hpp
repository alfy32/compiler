#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>

class Symbol {
private:
	std::string name;

public:
	std::string getName() 					{ return name; }
	std::string setName(std::string name) 	{ this->name = name; }
};

#endif
