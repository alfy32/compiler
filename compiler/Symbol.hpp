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
	bool isConstant;

	Symbol();
	Symbol(std::string identifier);
	virtual void print(std::ostream&);
};

enum ConstType {CONST_CHAR, CONST_STRING, CONST_INT, CONST_BOOL, CONST_UNKNOWN};

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
	StringConstant(std::string value);
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
	bool isArray;
	bool isRecord;

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
private:
	bool isLocal;
public:
	Type* type;
	int location;

	Variable(std::string name, Type* type, int location, bool isLocal);
	std::string getLocation();
	int getOffset();
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

class FunctionProcedure : public Symbol {
public:
	std::deque<std::pair<std::string, Type*> > signature;
	Type* returnType;

	bool isForward;

	FunctionProcedure(std::string identifier, std::deque<std::pair<std::deque<std::string>, Type*> >* formalParams, Type* returnType);
	virtual void print(std::ostream&);
};

#endif