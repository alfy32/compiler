#ifndef FUNC_HPP
#define FUNC_HPP

#include "Symbol.hpp"

class Func : public Symbol {
	//signature
	std::vector<std::pair<std::string, std::string> > signature;
	// location: label
	std::string location;

public: 
	void print();
};

#endif
