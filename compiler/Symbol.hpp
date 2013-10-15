#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include "cpsl.h"

class Symbol {
public:
	std::string name;

	Symbol() {
		name = "NO_NAME";
	}

	Symbol(std::string identifier) {
		name = identifier;
	}

	virtual void print() {
		std::cout << "Symbol: " << name << std::endl;
	}
};

enum ConstType { CONST_INT, CONST_STR, CONST_CHAR, UNKNOWN_TYPE }; 

class Constant : public Symbol {
public:
	ConstType constType;

	Constant() {
		name = "NO_NAME";
		constType = UNKNOWN_TYPE;
	}

	Constant(Constant* left, std::string op, Constant* right) {
		
		if(left->constType == right->constType) {
			name = left->name;
			constType = left->constType;
		} else {
			std::cout << "We failed here. These are different const types. I can't do the operation " << op << ".";
			exit(1);
		}
		name = "Operation";
	}

	Constant(std::string op, Constant* right) {
		if(right->constType == CONST_INT) {
			name = right->name;
			constType = right->constType;
		} else {
			name = "Operation";
			constType = UNKNOWN_TYPE;
		}
	}

	Constant(std::string identifier) {
		name = identifier;
		constType = UNKNOWN_TYPE;
	}

	virtual void print() {
		std::cout << "Constant: " << name << " Type: " << constType << std::endl;
	}
};

class StringConstant : public Constant {
public:
	std::string val;

	StringConstant(char* value) {
		std::string makeValue(value);
		val = makeValue;

		name = "NO_NAME";
		constType = CONST_STR;
	}

	void print() {
		std::cout << "\tString Constant: " << std::endl
				  << "\tName: " << name << std::endl
				  << "\tValue: " << val << std::endl;
	}
};

class CharacterConstant : public Constant {
public:
	std::string val;

	CharacterConstant(char* value) {
		std::string makeValue(value);
		val = makeValue;

		name = "NO_NAME";
		constType = CONST_CHAR;
	}

	CharacterConstant(std::string value) {
		val = value;

		name = "NO_NAME";
		constType = CONST_CHAR;
	}

	void print() {
		std::cout << "\tCharacter Constant: " << std::endl
				  << "\tName: " << name << std::endl
				  << "\tValue: " << val << std::endl;
	}
};

class IntegerConstant : public Constant {
public:
	int val;

	IntegerConstant(int value) {
		val = value;

		name = "NO_NAME";
		constType = CONST_INT;
	}

	void print() {
		std::cout << "\tInteger Constant: " << std::endl
			  	  << "\tName: " << name << std::endl
			  	  << "\tValue: " << val << std::endl;
	}
};

class Boolean : public Constant {
public:
	bool val;

	Boolean(bool value) {
		this->val = value;
	}

	void print() {
		std::cout << "\tBoolean Constant: " << std::endl
		  	      << "\tValue: " << val << std::endl;	
	}

};

class Type : public Symbol {
public:
	int size;

	Type() {
		name = "NO_NAME";
		size = 4;
	}

	Type(std::string identifier) {
		name = identifier;
		size = 4;
	}

	virtual void print() {
		std::cout << "Type: " << name << " Size: " << size << std::endl;
	}
};

class SimpleType : public Type {
public:

	SimpleType() {
		name = "NO_NAME";
		size = 4;
	}

	SimpleType(std::string identifier) {
		name = identifier;
		size = 4;
	}

	virtual void print() {
		std::cout << "\tSimpleType:" << std::endl 
				  << "\tName: " << name << std::endl
				  << "\tSize: " << size << std::endl;
	}
};

class Record : public Type {
public:
	std::map<std::string, std::pair<Type*, int> > recordMap;

	Record() {
		name = "NO_NAME";
		size = 0;
	}

	Record(std::vector<std::pair<std::vector<std::string>, Type*> >* recordItems) {
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

	virtual void print() {
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
};

class Array : public Type {
public:
	Type* type;
	int low;
	int upper;

	Array() {
		name = "NO_NAME";
		size = 0;

		low = 0;
		upper = 0;

		type = new Type();
	}

	Array(Constant* lower, Constant* up, Type* type) {
		this->name = "NO_NAME";

		this->low = dynamic_cast<IntegerConstant*>(lower)->val;
		this->upper = dynamic_cast<IntegerConstant*>(up)->val;

		//std::cout << "Array upper: " << this->upper << " Lower: " << this->low << std::endl;

		this->size = type->size*(upper-low+1);

		this->type = type;
	}

	virtual void print() {
		std::cout << "\tArray Type:" << std::endl
				  << "\tSize: " << this->size << std::endl
				  << "\tLower Bound: " << this->low << " Upper Bound: " << this->upper << std::endl
				  << "\tArray Address: " << this <<std::endl
				  << "\tType: " << this->type << std::endl;
	}
};

class Func : public Symbol {
public:
	std::vector<std::pair<std::string, Type*> > signature;
	Type* returnType;

	bool isForward;

	Func(std::string identifier, std::vector<std::pair<std::vector<std::string>, Type*> >* formalParams, Type* returnType) {
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

	virtual void print() {
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
};

class Proc : public Symbol {
public:
	std::vector<std::pair<std::string, Type*> > signature;

	bool isForward;

	Proc(std::string identifier, std::vector<std::pair<std::vector<std::string>, Type*> >* formalParams) {
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

	virtual void print() {
		std::cout << "\tProcedure: " << std::endl
				  << "\tName/Label/Location: " << name << std::endl; 
	}	
};

class Expression {
public:
	Type* type;
	int location;

	Expression(int location) {
		this->location = location;
	}

	int getLocation() {
		return this->location;
	}
};

class Table {
public: 
	std::map<std::string, Symbol*> tableMap;


	Table() {
		
	}

	void add(std::string identifier, Symbol* symbol) {
		// std::cout << "Adding to table...\n ";
		// std::cout << "  Identifier: " << identifier << " Symbol: " << symbol << std::endl;


		if(tableMap.find(identifier) != tableMap.end()) {
			std::cout << "We alread have the symbol (" << identifier << ") in the table. I quit!!!!" << std::endl;
			exit(1);
		}

		this->tableMap[identifier] = symbol;
	}

	Symbol* lookup(std::string name) {
		if(tableMap.find(name) != tableMap.end()) {
			return tableMap.find(name)->second;
		} else {
			return NULL;
		}
	}

	void print() {
		std::cout << "Here is the table: \n";

		std::map<std::string, Symbol*>::iterator iter;

		for(iter = tableMap.begin(); iter != tableMap.end(); iter++) {
			std::cout << " Name: " << iter->first << " Symbol: " << iter->second << std::endl;
			iter->second->print();
		}
	}
};

class SymbolTable {
	/* Singleton Stuff */
private:
	static SymbolTable* symbolTableInstance;

public:
	static SymbolTable* getInstance() {
		if(symbolTableInstance == NULL) {
			symbolTableInstance = new SymbolTable;

			symbolTableInstance->symbolTable.push_back(initializedMainTable());

			symbolTableInstance->symbolTable.push_back(Table());

		}
		return symbolTableInstance;
	}
/* End Singleton Stuff */

public:
	std::vector<Table> symbolTable; 

	static void add(std::string identifier, Symbol* symbol) {
		SymbolTable* tableInstance = getInstance();

		tableInstance->symbolTable.back().add(identifier, symbol);
	}

	static Symbol* lookup(std::string name) {
		
		SymbolTable* tableInstance = getInstance();

		for(int i = tableInstance->symbolTable.size()-1; i >= 0; i--) {
			Symbol* symbol = tableInstance->symbolTable[i].lookup(name);
			// std::cout << "Finding Symbol: level " << i << " Name: " << name << " Symbol: " << symbol << std::endl;
			if(symbol) {
				return symbol;
			}
		}
		
		std::cout << "The symbol(" << name << ") is not in the table so I will die." << std::endl;
		exit(1);
		
		return NULL;
	}

	static void pop() {
		// std::cout << "Popping Table...\n";
		SymbolTable* tableInstance = getInstance();

		std::cout << "*********** This is level: " << tableInstance->symbolTable.size()-1 << " **********" << std::endl;

		tableInstance->symbolTable.back().print();

		std::cout << "*********** We are popping the above table off the stack. **********" << std::endl;
		tableInstance->symbolTable.pop_back();
	}


	static Table initializedMainTable() {
		Table table;

		SimpleType* integer = new SimpleType("integer");
		SimpleType* character = new SimpleType("char");
		SimpleType* str = new SimpleType("string");
		SimpleType* boolean = new SimpleType("boolean");
		SimpleType* boolTrue = new SimpleType("true");
		SimpleType* boolFalse = new SimpleType("false");
		
		table.add("integer", integer);
		table.add("INTEGER", integer);
		table.add("char", character);
		table.add("CHAR", character);
		table.add("string", str);
		table.add("STRING", str);
		table.add("boolean", boolean);
		table.add("BOOLEAN", boolean);
		table.add("true", boolTrue);
		table.add("TRUE", boolTrue);
		table.add("false", boolFalse);
		table.add("FALSE", boolFalse);

		return table;
	}

	static void addVar(std::vector<std::string>* identList, Type* type) {
		for(int i = 0; i < identList->size(); i++) {
			add((*identList)[i], type);
		}
	}

	static void constDecl(std::string identifier, Constant* constExpression) {
		constExpression->name = identifier;
		add(identifier, constExpression);
	}

	static void typeDecl(std::string identifier, Type* type) {
		type->name = identifier;
		add(identifier, type);
	}

	static void funcDecl(std::string identifier, Func* func) {
		// std::cout << "Adding Function..." << std::endl;
		add(identifier, func);

		addNewScope(func);
	}
//std::vector<std::pair<std::string, Type*> > signature;
	static void addNewScope(Func* func) {
		SymbolTable* tableInstance = getInstance();

		Table table;
		tableInstance->symbolTable.push_back(table);

		for(std::pair<std::string, Type*> pair : func->signature) {
			add(pair.first, pair.second);
		}
	}

	static void procDecl(std::string identifier, Proc* proc) {
		add(identifier, proc);

		addNewScope(proc);
	}

	static void addNewScope(Proc* proc) {
		SymbolTable* tableInstance = getInstance();

		Table table;		
		tableInstance->symbolTable.push_back(table);
		
		for(std::pair<std::string, Type*> pair : proc->signature) {
			add(pair.first, pair.second);
		}
	}
//FOR_SYM IDENT_SYM ASSIGNMENT_SYM Expression TO_SYM Expression DO_SYM
	static void forStatement(std::string identifier, Symbol* initalValue, Symbol* expr, std::string UpToOrDownTo) {
		// SymbolTable* tableInstance = getInstance();

		// Table table;
		// tableInstance->symbolTable.push_back(table);

		// std::cout << "Added new for scope...\n";

		// add(identifier, initalValue);
	}

	static Constant* evalConstant(Constant* left, std::string oper, Constant* right) {
		if(left->constType == right->constType) {
			switch(left->constType)
			{
			case CONST_INT:
				evalIntConstant(left, oper, right);
				break;
			case CONST_CHAR:
				evalCharConstant(left, oper, right);
				break;
			case CONST_STR:
				evalStrConstant(left, oper, right);
				break;
			default:
				std::cout << "This is and unknown constant type. I quit.\n";
				exit(1);
			}
		} else {
			std::cout << "We quit. You are trying to add constants of different types. \n";
			exit(1);
		}
	}

	static Constant* evalConstant(std::string oper, Constant* right) {
		if(right->constType == CONST_INT) {
			IntegerConstant* value = dynamic_cast<IntegerConstant*>(right);

			if(oper == "~") {
				return new Boolean(!value->val);
			} else if(oper == "neg") {
				return new IntegerConstant(-value->val);
			}
		}

		std::cout << "I don't know how to evaluate this constant.\n";
		exit(1);
	}

	static Constant* evalIntConstant(Constant* left, std::string oper, Constant* right) {
		int leftValue = dynamic_cast<IntegerConstant*>(left)->val;
		int rightValue = dynamic_cast<IntegerConstant*>(right)->val;

		if(oper == "|") {
			return new Boolean(leftValue || rightValue);
		} else if(oper == "&") {
			return new Boolean(leftValue && rightValue);
		}  else if(oper == "=") {
			return new Boolean(leftValue == rightValue);
		} else if(oper == "<>") {
			return new Boolean(leftValue != rightValue);
		} else if(oper == "<=") {
			return new Boolean(leftValue <= rightValue);
		} else if(oper == ">=") {
			return new Boolean(leftValue >= rightValue);
		} else if(oper == "<") {
			return new Boolean(leftValue < rightValue);
		} else if(oper == ">") {
			return new Boolean(leftValue > rightValue);
		} else if(oper == "+") {
			return new IntegerConstant(leftValue + rightValue);
		} else if(oper == "-") {
			return new IntegerConstant(leftValue - rightValue);
		} else if(oper == "*") {
			return new IntegerConstant(leftValue * rightValue);
		} else if(oper == "/") {
			return new IntegerConstant(leftValue / rightValue);
		} else if(oper == "%") {
			return new IntegerConstant(leftValue % rightValue);
		} 

		std::cout << "That was an invalid operator for integers. I quit.\n";
		exit(1); 

		return NULL;
	}

	static Constant* evalCharConstant(Constant* left, std::string oper, Constant* right) {
		std::string leftValue = dynamic_cast<CharacterConstant*>(left)->val;
		std::string rightValue = dynamic_cast<CharacterConstant*>(right)->val;

		if(oper == "=") {
			return new Boolean(leftValue == rightValue);
		} else if(oper == "<>") {
			return new Boolean(leftValue != rightValue);
		} else if(oper == "<=") {
			return new Boolean(leftValue <= rightValue);
		} else if(oper == ">=") {
			return new Boolean(leftValue >= rightValue);
		} else if(oper == "<") {
			return new Boolean(leftValue < rightValue);
		} else if(oper == ">") {
			return new Boolean(leftValue > rightValue);
		} else if(oper == "+") {
			return new CharacterConstant(leftValue + rightValue);
		} 

		std::cout << "That was an invalid operator for characters. I quit.\n";
		exit(1); 

		return NULL;
	}

	static Constant* evalStrConstant(Constant* left, std::string oper, Constant* right) {
		std::string leftValue = dynamic_cast<StringConstant*>(left)->val;
		std::string rightValue = dynamic_cast<StringConstant*>(right)->val;

		if(oper == "=") {
			return new Boolean(leftValue == rightValue);
		} else if(oper == "<>") {
			return new Boolean(leftValue != rightValue);
		} else if(oper == "<=") {
			return new Boolean(leftValue <= rightValue);
		} else if(oper == ">=") {
			return new Boolean(leftValue >= rightValue);
		} else if(oper == "<") {
			return new Boolean(leftValue < rightValue);
		} else if(oper == ">") {
			return new Boolean(leftValue > rightValue);
		} else if(oper == "+") {
			return new CharacterConstant(leftValue + rightValue);
		} 

		std::cout << "That was an invalid operator for strings. I quit.\n";
		exit(1); 

		return NULL;
	}

	static Constant* lookupConstant(std::string identifier) {
		return dynamic_cast<Constant*>(lookup(identifier));
	}

	static Symbol* expression(Symbol* left, void*, Symbol* right){
		// std::cout << "Expression: symbol, op, symbol" << std::endl;
		return new Symbol("NO_NAME From expresssion");
	}

	static Symbol* expression(void*, Symbol* right){
		// std::cout << "Expression: op, symbol" << std::endl;
		return new Symbol("NO_NAME From expresssion");
	}

	static Symbol* expressionLvalue(Symbol* symbol){
		// std::cout << "Expression: int" << std::endl;
		return symbol;
	}

	static Symbol* function_call(std::string identifier) {
		// std::cout << "Function Call: no params." << std::endl;
		return new Symbol(identifier);
	}

	static Symbol* function_call(std::string identifier, std::vector<Expression*>*) {
		// std::cout << "Function Call: has params." << std::endl;
		return new Symbol(identifier);
	}

	static Symbol* chr(Symbol* symbol) {
		// std::cout << "CHR: " << std::endl;
		return new Symbol("CHR thingy");
	}

	static Symbol* ord(Symbol* symbol) {
		// std::cout << "ORD: " << std::endl;
		return new Symbol("ORD thingy");
	}

	static Symbol* pred(Symbol* symbol) {
		// std::cout << "PRED: " << std::endl;
		return new Symbol("PRED thingy");
	}

	static Symbol* succ(Symbol* symbol) {
		// std::cout << "SUCC: " << std::endl;
		return new Symbol("SUCC thingy");
	}

	static std::vector<Expression*>* makeExpressionList(Symbol* symbol, std::vector<Expression*>* expressions) {
		Expression* expression = dynamic_cast<Expression*>(symbol);

		if(expressions == NULL) {
			expressions = new std::vector<Expression*>;
		}

		expressions->push_back(expression);

		return expressions;
	}

	static std::vector<Symbol*>* makeSymbolVector(Symbol* symbol, std::vector<Symbol*>* symbols) {
		if(symbols == NULL) {
			symbols = new std::vector<Symbol*>;
		}

		symbols->push_back(symbol);

		return symbols;
	}

	static std::vector<Type*>* makeTypeList(std::vector<Symbol*>* identifiers, Type* type, std::vector<Type*>* symbols) {
		if(symbols == NULL) {
			symbols = new std::vector<Type*>;
		}

		for(int i = 0; i < (int)identifiers->size(); i++) {
			Type* newType = new Type(*type);
			newType->name = (*identifiers)[i]->name;
			symbols->push_back(newType);
		}

		return symbols;
	}

	static std::vector<std::string>* makeIdentList(std::string identifier, std::vector<std::string>* identList) {
		if(identList == NULL) {
			identList = new std::vector<std::string>;
		}

		identList->push_back(identifier);

		return identList;
	}

	static std::vector<std::pair<std::vector<std::string>, Type*> >* makeRecordItem(std::vector<std::string>* identList, Type* type, std::vector<std::pair<std::vector<std::string>, Type*> >* recordItem) {
		if(recordItem == NULL) {
			recordItem = new std::vector<std::pair<std::vector<std::string>, Type*> >;
		}

		std::pair<std::vector<std::string>, Type*> thePair = std::make_pair(*identList, type);

		recordItem->push_back(thePair);

		return recordItem;
	}


	///////////////////////// Initialize Assembly //////////////////////////////

	static void initAssembly() {
		std::cout << "main: "; //TODO: write main stuff here.
	}

	////////////////////////////////////////////////////////////////////////////

	//////////////////////////// Read and Write ////////////////////////////////

	static void write(std::vector<Expression*>* expressionList) {
		for(Expression* expression: *expressionList) {
			int location = lookup(expression);

			writeInteger(location);
		}
	}

	static void writeInteger(int location) {
		std::cout << "li	$v0, 1" << std::endl
				  << "mv	$a0, $" << location << std::endl
				  << "syscall" << std::endl;
	}

	static void writeString(std::string label) {
		std::cout << "li	$v0, 4" << std::endl
				  << "mv	$a0, " << label << std::endl
				  << "syscall" << std::endl;
	}

	static Expression* read(Expression* expression) {
		int location = expression->getLocation();
		return readInteger(location);
	}

	static Expression* readInteger(int location) {
		std::cout << "li	$v0, 5" << std::endl
				  << "syscall" << std::endl
				  << "sw	$v0, $" << location << std::endl;
	}

	////////////////////////////////////////////////////////////////////////////

	///////////////////////////// Expressoins //////////////////////////////////

	static int currentRegister;

	static int getRegister() {
		return currentRegister++;
	}

	static int lookup(Expression* expression) {
		//TODO: find the location of the expression.
		return 0;
	}

	static std::pair<int,int> lookupExpression(std::string name) {
		int location, offset;
		//TODO: find the location given a string.
		return std::make_pair(location, offset);
	}

	static Expression* eval(Expression* left, Expression* right, std::string operation) {
		int leftLocation = lookup(left);
		int rightLocation = lookup(right);
		int resultLocation = getRegister();

		std::cout << operation << " $" << resultLocation
			  	  << ", $" << leftLocation 
			  	  << ", $" << rightLocation
			  	  << std::endl;

		return new Expression(resultLocation);
	}

	static Expression* load(std::string name) {
		std::pair<int, int> offset = lookupExpression(name);
		int resultLocation = getRegister();
		std::cout << "lw $" << resultLocation
				  << ", $" << offset.second
				  << "(" << offset.first << ")"
				  << std::endl;

		return new Expression(resultLocation); 
	}

	////////////////////////////////////////////////////////////////////////////
};

#endif