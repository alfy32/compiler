#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <memory>

#include "Type.hpp"

class Array : public Type {
	std::shared_ptr<Type> type;
	int lowerBound;
	int upperBound; 

public:
	Array(int lowerBound, int upperBound, Type* type) {
		this->lowerBound = lowerBound;
		this->upperBound = upperBound;
		this->type = make_shared<Type>(*Type);
	}
};

#endif
