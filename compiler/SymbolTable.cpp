#include "SymbolTable.hpp"


SymbolTable* SymbolTable::symbolTableInstance = NULL;

int SymbolTable::currentRegister = 8;

int SymbolTable::currentOffset = 0;

//// Expression ////

Expression::Expression(int location) {
	this->location = location;
}

int Expression::getLocation() {
	return this->location;
}

//// End Expression ////

//// Table ////

Table::Table() {
	
}

void Table::add(std::string identifier, Symbol* symbol) {
	// std::cout << "Adding to table...\n ";
	// std::cout << "  Identifier: " << identifier << " Symbol: " << symbol << std::endl;


	if(tableMap.find(identifier) != tableMap.end()) {
		std::cout << "We alread have the symbol (" << identifier << ") in the table. I quit!!!!" << std::endl;
		exit(1);
	}

	this->tableMap[identifier] = symbol;
}

Symbol* Table::lookup(std::string name) {
	if(tableMap.find(name) != tableMap.end()) {
		return tableMap.find(name)->second;
	} else {
		return NULL;
	}
}

void Table::print() {
	std::cout << "Here is the table: \n";

	std::map<std::string, Symbol*>::iterator iter;

	for(iter = tableMap.begin(); iter != tableMap.end(); iter++) {
		std::cout << " Name: " << iter->first << " Symbol: " << iter->second << std::endl;
		iter->second->print();
	}
}

//// End Table ////

//// SymbolTable ////

SymbolTable* SymbolTable::getInstance() {
	if(symbolTableInstance == NULL) {
		symbolTableInstance = new SymbolTable;

		symbolTableInstance->symbolTable.push_back(initializedMainTable());

		symbolTableInstance->symbolTable.push_back(Table());

	}
	return symbolTableInstance;
}

void SymbolTable::add(std::string identifier, Symbol* symbol) {
	SymbolTable* tableInstance = getInstance();

	tableInstance->symbolTable.back().add(identifier, symbol);
}

Symbol* SymbolTable::lookup(std::string name) {
	
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

void SymbolTable::pop() {
	// std::cout << "Popping Table...\n";
	SymbolTable* tableInstance = getInstance();

	std::cout << "*********** This is level: " << tableInstance->symbolTable.size()-1 << " **********" << std::endl;

	tableInstance->symbolTable.back().print();

	std::cout << "*********** We are popping the above table off the stack. **********" << std::endl;
	tableInstance->symbolTable.pop_back();
}


Table SymbolTable::initializedMainTable() {
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

void SymbolTable::addVar(std::vector<std::string>* identList, Type* type) {
	for(std::string identifier : *identList) {
		Variable* var = new Variable(identifier, type, currentOffset);
		add(identifier, var);
		currentOffset += type->size;
	}
}

void SymbolTable::constDecl(std::string identifier, Constant* constExpression) {
	constExpression->name = identifier;
	add(identifier, constExpression);
}

void SymbolTable::typeDecl(std::string identifier, Type* type) {
	type->name = identifier;
	add(identifier, type);
}

void SymbolTable::funcDecl(std::string identifier, Func* func) {
	add(identifier, func);

	addNewScope(func);
}

void SymbolTable::addNewScope(Func* func) {
	SymbolTable* tableInstance = getInstance();

	Table table;
	tableInstance->symbolTable.push_back(table);

	currentOffset = 0;

	for(std::pair<std::string, Type*> pair : func->signature) {
		add(pair.first, pair.second);
	}
}

void SymbolTable::procDecl(std::string identifier, Proc* proc) {
	add(identifier, proc);

	addNewScope(proc);
}

void SymbolTable::addNewScope(Proc* proc) {
	SymbolTable* tableInstance = getInstance();

	Table table;		
	tableInstance->symbolTable.push_back(table);
	
	for(std::pair<std::string, Type*> pair : proc->signature) {
		add(pair.first, pair.second);
	}
}
//FOR_SYM IDENT_SYM ASSIGNMENT_SYM Expression TO_SYM Expression DO_SYM
void SymbolTable::forStatement(std::string identifier, Expression* initalValue, Expression* expr, std::string UpToOrDownTo) {
	// SymbolTable* tableInstance = getInstance();

	// Table table;
	// tableInstance->symbolTable.push_back(table);

	// std::cout << "Added new for scope...\n";

	// add(identifier, initalValue);
}

Constant* SymbolTable::evalConstant(Constant* left, std::string oper, Constant* right) {
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

Constant* SymbolTable::evalConstant(std::string oper, Constant* right) {
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

Constant* SymbolTable::evalIntConstant(Constant* left, std::string oper, Constant* right) {
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

Constant* SymbolTable::evalCharConstant(Constant* left, std::string oper, Constant* right) {
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

Constant* SymbolTable::evalStrConstant(Constant* left, std::string oper, Constant* right) {
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

Expression* SymbolTable::lValueToExpression(std::string identifier) {
	Variable* variable = dynamic_cast<Variable*>(lookup(identifier));

	Expression* expression = load(variable->location);
	expression->type = variable->type;
	return expression;
}

Expression* SymbolTable::integerConstToExpression(int value) {
	std::cout << "# loading a constant integer into a register.\n";
	return loadImmediateInt(value);
}

Constant* SymbolTable::lookupConstant(std::string identifier) {
	return dynamic_cast<Constant*>(lookup(identifier));
}

Expression* SymbolTable::expression(Expression* left, std::string op, Expression* right){
	if(left->type == right->type) {
		return eval(left, right, op);
	} else {
		std::cout << "We have a type problem. You must have matching types in expressions.\n";
		exit(1);
	}
}

Expression* SymbolTable::expression(std::string, Expression* right){
	// std::cout << "Expression: op, symbol" << std::endl;
	return right;
}

Expression* SymbolTable::expressionLvalue(Expression* symbol){
	// std::cout << "Expression: int" << std::endl;
	return symbol;
}

Expression* SymbolTable::function_call(std::string identifier) {
	// std::cout << "Function Call: no params." << std::endl;
	return new Expression(0);
}

Expression* SymbolTable::function_call(std::string identifier, std::vector<Expression*>*) {
	// std::cout << "Function Call: has params." << std::endl;
	return new Expression(0);
}

Expression* SymbolTable::chr(Expression* symbol) {
	// std::cout << "CHR: " << std::endl;
	return new Expression(0);
}

Expression* SymbolTable::ord(Expression* symbol) {
	// std::cout << "ORD: " << std::endl;
	return new Expression(0);
}

Expression* SymbolTable::pred(Expression* symbol) {
	// std::cout << "PRED: " << std::endl;
	return new Expression(0);
}

Expression* SymbolTable::succ(Expression* symbol) {
	// std::cout << "SUCC: " << std::endl;
	return new Expression(0);
}

std::vector<Expression*>* SymbolTable::makeExpressionList(Expression* expression, std::vector<Expression*>* expressions) {
	if(expressions == NULL) {
		expressions = new std::vector<Expression*>;
	}

	expressions->push_back(expression);

	return expressions;
}

std::vector<Symbol*>* SymbolTable::makeSymbolVector(Symbol* symbol, std::vector<Symbol*>* symbols) {
	if(symbols == NULL) {
		symbols = new std::vector<Symbol*>;
	}

	symbols->push_back(symbol);

	return symbols;
}

std::vector<Type*>* SymbolTable::makeTypeList(std::vector<Symbol*>* identifiers, Type* type, std::vector<Type*>* symbols) {
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

std::vector<std::string>* SymbolTable::makeIdentList(std::string identifier, std::vector<std::string>* identList) {
	if(identList == NULL) {
		identList = new std::vector<std::string>;
	}

	identList->push_back(identifier);

	return identList;
}

std::vector<std::pair<std::vector<std::string>, Type*> >* SymbolTable::makeRecordItem(std::vector<std::string>* identList, Type* type, std::vector<std::pair<std::vector<std::string>, Type*> >* recordItem) {
	if(recordItem == NULL) {
		recordItem = new std::vector<std::pair<std::vector<std::string>, Type*> >;
	}

	std::pair<std::vector<std::string>, Type*> thePair = std::make_pair(*identList, type);

	recordItem->push_back(thePair);

	return recordItem;
}


///////////////////////// Initialize Assembly //////////////////////////////

void SymbolTable::initAssembly() {
	/*
		.text
		.globl main
	main: la $gp, GA
		b _begin


	somewhere is the main
	_begin: 


	string area
	.asciiz " "
	*/
	std::cout << "main: "; //TODO: write main stuff here.
}

////////////////////////////////////////////////////////////////////////////

//////////////////////////// Read and Write ////////////////////////////////

void SymbolTable::write(std::vector<Expression*>* expressionList) {
	for(Expression* expression: *expressionList) {
		if(expression->type == dynamic_cast<Type*>(lookup("integer"))) {
			int location = lookup(expression);
			writeInteger(location);
		} 
		else if(expression->type == dynamic_cast<Type*>(lookup("string"))) {
			std::string label = "Find the label for this string.";
			writeString(label);
		}
		else if(expression->type == dynamic_cast<Type*>(lookup("char"))) {
			int location = lookup(expression);
			writeCharacter(location);
		}
		else {
			std::cout << "Trying to write non integer.\n";
		}
	}
}

void SymbolTable::writeInteger(int location) {
	std::cout << std::endl
			  << "# Writing an integer to the console.\n."
			  << "li	$v0, 1" << std::endl
			  << "mv	$a0, $" << location << std::endl
			  << "syscall" << std::endl
			  << std::endl;
}

void SymbolTable::writeCharacter(int location) {
	std::cout << std::endl
			  << "# Writing a character to the console.\n."
			  << "li	$v0, #figure out what this system call is" << std::endl
			  << "mv	$a0, $" << location << std::endl
			  << "syscall" << std::endl
			  << std::endl;
}

void SymbolTable::writeString(std::string label) {
	std::cout << std::endl
			  << "Writing a string to the console.\n"
			  << "li	$v0, 4" << std::endl
			  << "mv	$a0, " << label << std::endl
			  << "syscall" << std::endl
			  << std::endl;
}

Expression* SymbolTable::read(Expression* expression) {
	int location = expression->getLocation();
	return readInteger(location);
}

Expression* SymbolTable::readInteger(int location) {
	std::cout << "li	$v0, 5" << std::endl
			  << "syscall" << std::endl
			  << "sw	$v0, $" << location << std::endl;
}

////////////////////////////////////////////////////////////////////////////

///////////////////////////// Expressoins //////////////////////////////////


int SymbolTable::getRegister() {
	return currentRegister++;
}

int SymbolTable::lookup(Expression* expression) {
	return expression->location;
}

std::pair<int,int> SymbolTable::lookupExpression(std::string name) {
	int location, offset;
	//TODO: find the location given a string.
	return std::make_pair(location, offset);
}

Expression* SymbolTable::eval(Expression* left, Expression* right, std::string operation) {
	int leftLocation = lookup(left);
	int rightLocation = lookup(right);
	int resultLocation = getRegister();

	std::cout << operation << " $" << resultLocation
		  	  << ", $" << leftLocation 
		  	  << ", $" << rightLocation
		  	  << std::endl;

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::load(int location) {
	int resultLocation = getRegister();
	std::cout << "lw $" << resultLocation
			  << ", " << location << "($sp)"
			  << std::endl;

	return new Expression(resultLocation);
}

Expression* SymbolTable::load(std::string name) {
	std::pair<int, int> offset = lookupExpression(name);
	int resultLocation = getRegister();
	std::cout << "lw $" << resultLocation
			  << ", $" << offset.second
			  << "(" << offset.first << ")"
			  << std::endl
			  << std::endl;

	return new Expression(resultLocation); 
}

Expression* SymbolTable::loadImmediateInt(int value) {
	int location = getRegister();
	std::cout << "li	$" << location << ", " << value << std::endl << std::endl;
	
	Expression* expression = new Expression(location);
	expression->type = dynamic_cast<Type*>(lookup("integer"));
	return expression;
}

Expression* SymbolTable::loadImmediateChar(std::string value) {
	int location = getRegister();
	std::cout << "li	$" << location << ", " << value << std::endl << std::endl;
	return new Expression(location);
}

////////////////////////////////////////////////////////////////////////////

/*	strings

	.data 

	string1: asciz "STRING"

	GA: 
	.space
*/

//// End SymbolTable ////