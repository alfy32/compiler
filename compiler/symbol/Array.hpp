#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <memory>

#include "Type.hpp"

class Array : public Type {
	std::shared_ptr<Type> type;
	int lowerBound;
	int upperBound; 
};

#endif
