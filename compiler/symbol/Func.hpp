#ifndef FUNC_HPP
#define FUNC_HPP

#include <vector>

#include "Symbol.hpp"

class Func : public Symbol {
	//signature
	std::vector<std::pair<std::string, std::string> > signature;
	// location: label
	std::string location;

	bool isForward;

public: 
	// (name, FormalParameters, isForward)
	Func(std::string, std::vector<Type>, bool) {
				
	}
	void print();
};

#endif
