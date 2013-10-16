#include "Symbol.hpp"

//// Symbol ////

Symbol::Symbol() {
	name = "NO_NAME";
}

Symbol::Symbol(std::string identifier) {
	name = identifier;
}

void Symbol::print() {
	std::cout << "Symbol: " << name << std::endl;
}

//// End Symbol ////


//// Constant ////

Constant::Constant() {
	name = "NO_NAME";
	constType = UNKNOWN_TYPE;
}

Constant::Constant(Constant* left, std::string op, Constant* right) {
	
	if(left->constType == right->constType) {
		name = left->name;
		constType = left->constType;
	} else {
		std::cout << "We failed here. These are different const types. I can't do the operation " << op << ".";
		exit(1);
	}
	name = "Operation";
}

Constant::Constant(std::string op, Constant* right) {
	if(right->constType == CONST_INT) {
		name = right->name;
		constType = right->constType;
	} else {
		name = "Operation";
		constType = UNKNOWN_TYPE;
	}
}

Constant::Constant(std::string identifier) {
	name = identifier;
	constType = UNKNOWN_TYPE;
}

void Constant::print() {
	std::cout << "Constant: " << name << " Type: " << constType << std::endl;
}

//// End Constant ////


//// StringConstant ////

StringConstant::StringConstant(char* value) {
	std::string makeValue(value);
	val = makeValue;

	name = "NO_NAME";
	constType = CONST_STR;
}

void StringConstant::print() {
	std::cout << "\tString Constant: " << std::endl
			  << "\tName: " << name << std::endl
			  << "\tValue: " << val << std::endl;
}

//// End StringConstant ////

//// CharacterConstant ////

CharacterConstant::CharacterConstant(char* value) {
	std::string makeValue(value);
	val = makeValue;

	name = "NO_NAME";
	constType = CONST_CHAR;
}

CharacterConstant::CharacterConstant(std::string value) {
	val = value;

	name = "NO_NAME";
	constType = CONST_CHAR;
}

void CharacterConstant::print() {
	std::cout << "\tCharacter Constant: " << std::endl
			  << "\tName: " << name << std::endl
			  << "\tValue: " << val << std::endl;
}

//// End CharacterConstant ////


//// IntegerConstant ////

IntegerConstant::IntegerConstant(int value) {
	val = value;

	name = "NO_NAME";
	constType = CONST_INT;
}

void IntegerConstant::print() {
	std::cout << "\tInteger Constant: " << std::endl
		  	  << "\tName: " << name << std::endl
		  	  << "\tValue: " << val << std::endl;
}

//// End IntegerConstant ////

//// Boolean ////

Boolean::Boolean(bool value) {
	this->val = value;
}

void Boolean::print() {
	std::cout << "\tBoolean Constant: " << std::endl
	  	      << "\tValue: " << val << std::endl;	
}

//// End Boolean ////

//// Type ////

Type::Type() {
	name = "NO_NAME";
	size = 4;
}

Type::Type(std::string identifier) {
	name = identifier;
	size = 4;
}

void Type::print() {
	std::cout << "Type: " << name << " Size: " << size << std::endl;
}

//// End Type ////

//// SimpleType ////

SimpleType::SimpleType() {
	name = "NO_NAME";
	size = 4;
}

SimpleType::SimpleType(std::string identifier) {
	name = identifier;
	size = 4;
}

void SimpleType::print() {
	std::cout << "\tSimpleType:" << std::endl 
			  << "\tName: " << name << std::endl
			  << "\tSize: " << size << std::endl;
}

//// End SimpleType ////

//// Variable ////

Variable::Variable(std::string name, Type* type, int location) {
	this->name = name;
	this->type = type;
	this->location = location;
}

void Variable::print() {
	std::cout << "\tVariable:" << std::endl 
			  << "\tName: " << name << std::endl
			  << "\tLocation: " << this->location << std::endl 
			  << "\tSize: " << type->size << std::endl;
}

//// End Variable ////

//// Record ////

Record::Record() {
	name = "NO_NAME";
	size = 0;
}

Record::Record(std::vector<std::pair<std::vector<std::string>, Type*> >* recordItems) {
	name = "Record";

	int offset = 0;

	for(int i = 0; i < recordItems->size(); i++) {
		std::pair<std::vector<std::string>, Type*> recordItem = (*recordItems)[i];

		for(int j = 0; j < recordItem.first.size(); j++) {
			std::pair<Type*, int> recordMapItem = std::make_pair(recordItem.second, offset);

			offset += recordItem.second->size;
			
			recordMap[recordItem.first[j]] = recordMapItem;
		}
	}

	size = offset;
}

void Record::print() {
	std::cout << "\tRecord Type: " << std::endl
			  << "\tName: " << this->name << std::endl
			  << "\tSize: " << this->size << std::endl
			  << "\tElements: " << std::endl;

	for(std::pair<std::string, std::pair<Type*, int> > pair : recordMap) {
		std::cout << "\t\tName: " << pair.first << std::endl
				  << "\t\tOffset: " << pair.second.second << std::endl
				  << "\t\tStart Element Type Info *******" << std::endl;
				  pair.second.first->print();
		std::cout << "\t\tEnd Element Type Info *******" << std::endl;
	}
}

//// End Record ////

//// Array ////

Array::Array() {
	name = "NO_NAME";
	size = 0;

	low = 0;
	upper = 0;

	type = new Type();
}

Array::Array(Constant* lower, Constant* up, Type* type) {
	this->name = "NO_NAME";

	this->low = dynamic_cast<IntegerConstant*>(lower)->val;
	this->upper = dynamic_cast<IntegerConstant*>(up)->val;

	this->size = type->size*(upper-low+1);

	this->type = type;
}

void Array::print() {
	std::cout << "\tArray Type:" << std::endl
			  << "\tSize: " << this->size << std::endl
			  << "\tLower Bound: " << this->low << " Upper Bound: " << this->upper << std::endl
			  << "\tArray Address: " << this <<std::endl
			  << "\tType: " << this->type << std::endl;
}

//// End Array ////

//// Func ////

Func::Func(std::string identifier, std::vector<std::pair<std::vector<std::string>, Type*> >* formalParams, Type* returnType) {
	std::cout << "Function... Name: " << identifier << " ReturnType: " << returnType << " Params: ";

	this->name = identifier;
	this->returnType = returnType;

	if(formalParams) {

		for(int i = 0; i < formalParams->size(); i++) {
			std::pair<std::vector<std::string>, Type*> formalParam = (*formalParams)[i];
			Type* type = formalParam.second;
			std::vector<std::string> identifiers = formalParam.first;

			for(int j = 0; j < identifiers.size(); j++) {
				std::cout << " Ident: " << identifiers[j] << " Type: " << type;
				std::pair<std::string, Type*> thePair = std::make_pair(identifiers[j], type);

				signature.push_back(thePair);
			}
		}
	}
	std::cout << std::endl;
}

void Func::print() {
	std::cout << "\tFunction: " << std::endl
			  << "\tLabel/Location: " << name << std::endl
			  << "\tParameters start*********:" << std::endl;

	int paramCount = 1;

	for(std::pair<std::string, Type*> pair : signature) {
		std::cout << "\tParam " << paramCount++ << std::endl;
		pair.second->print();
	}

	std::cout << "\tParameters End*********:" << std::endl;
}	

//// End Func ////

//// Proc ////

Proc::Proc(std::string identifier, std::vector<std::pair<std::vector<std::string>, Type*> >* formalParams) {
	std::cout << "Procedure... Name: " << identifier << " Params: ";

	this->name = identifier;

	if(formalParams) {

		for(int i = 0; i < formalParams->size(); i++) {
			std::pair<std::vector<std::string>, Type*> formalParam = (*formalParams)[i];
			Type* type = formalParam.second;
			std::vector<std::string> identifiers = formalParam.first;

			for(int j = 0; j < identifiers.size(); j++) {
				std::cout << " Ident: " << identifiers[j] << " Type: " << type;
				std::pair<std::string, Type*> thePair = std::make_pair(identifiers[j], type);

				signature.push_back(thePair);
			}
		}
	}
	std::cout << std::endl;
}	

void Proc::print() {
	std::cout << "\tProcedure: " << std::endl
			  << "\tName/Label/Location: " << name << std::endl; 
}	

//// End Proc ////