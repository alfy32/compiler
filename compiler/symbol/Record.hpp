#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <memory>
#include <map>

#include "Type.hpp"

class Record : public Type {
	std::map<std::string, std::pair<std::shared_ptr<Type>, int> > types;

public:
	Record(std::map<std::string, std::pair<std::shared_ptr<Type> types) {
		this->types = types;
	}
};

#endif
