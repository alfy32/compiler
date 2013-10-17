#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <iostream>
#include <fstream>
#include <deque>
#include <map>
#include <string>
#include <memory>

class Symbol {
public:
	std::string name;

	Symbol();
	Symbol(std::string identifier);
	virtual void print(std::ostream&);
};

enum ConstType { CONST_INT, CONST_STR, CONST_CHAR, UNKNOWN_TYPE }; 

class Constant : public Symbol {
public:
	ConstType constType;

	Constant();
	Constant(Constant* left, std::string op, Constant* right);
	Constant(std::string op, Constant* right);
	Constant(std::string identifier);
	virtual void print(std::ostream&);
};

class StringConstant : public Constant {
public:
	std::string val;

	StringConstant(char* value);
	virtual void print(std::ostream&);
};

class CharacterConstant : public Constant {
public:
	std::string val;

	CharacterConstant(char* value);
	CharacterConstant(std::string value);
	virtual void print(std::ostream&);
};

class IntegerConstant : public Constant {
public:
	int val;

	IntegerConstant(int value);
	virtual void print(std::ostream&);
};

class Boolean : public Constant {
public:
	bool val;

	Boolean(bool value);
	virtual void print(std::ostream&);
};

class Type : public Symbol {
public:
	int size;

	Type();
	Type(std::string identifier);
	virtual void print(std::ostream&);
};

class SimpleType : public Type {
public:

	SimpleType();
	SimpleType(std::string identifier);
	virtual void print(std::ostream&);
};

class Variable : public Symbol {
public:
	Type* type;
	int location;

	Variable(std::string name, Type* type, int location);
	virtual void print(std::ostream&);
};

class Record : public Type {
public:
	std::map<std::string, std::pair<Type*, int> > recordMap;

	Record();

	Record(std::deque<std::pair<std::deque<std::string>, Type*> >* recordItems);
	virtual void print(std::ostream&);
};

class Array : public Type {
public:
	Type* type;
	int low;
	int upper;

	Array();
	Array(Constant* lower, Constant* up, Type* type);
	virtual void print(std::ostream&);
};

class Func : public Symbol {
public:
	std::deque<std::pair<std::string, Type*> > signature;
	Type* returnType;

	bool isForward;

	Func(std::string identifier, std::deque<std::pair<std::deque<std::string>, Type*> >* formalParams, Type* returnType);
	virtual void print(std::ostream&);
};

class Proc : public Symbol {
public:
	std::deque<std::pair<std::string, Type*> > signature;

	bool isForward;

	Proc(std::string identifier, std::deque<std::pair<std::deque<std::string>, Type*> >* formalParams);
	virtual void print(std::ostream&);
};


#endif