#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <string>
#include <iostream>

class Symbol {
private:
	std::string name;

public:
	std::string getName() 					{ return name; }
	std::string setName(std::string name) 	{ this->name = name; }

	virtual void print() = 0;
};

#endif
