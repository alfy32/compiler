#include "SymbolTable.hpp"

extern bool verbose;


SymbolTable* SymbolTable::symbolTableInstance = NULL;

int SymbolTable::currentRegister = 8;

int SymbolTable::currentOffset = 0;

int SymbolTable::currentConstString = 1;

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
	if(tableMap.find(identifier) != tableMap.end()) {
		SymbolTable::getInstance()->getErrorStream() << "We alread have the symbol (" << identifier << ") in the table. I quit!!!!" << std::endl;
		yyerror("Symbol already defined.");
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
	SymbolTable::getInstance()->getVerboseStream() << "Here is the table: \n";

	std::map<std::string, Symbol*>::iterator iter;

	for(iter = tableMap.begin(); iter != tableMap.end(); iter++) {
		std::ostream& out = SymbolTable::getInstance()->getVerboseStream();
		out << " Name: " << iter->first << " Symbol: " << iter->second << std::endl;
		iter->second->print(out);
	}
}

//// End Table ////

//// SymbolTable ////

SymbolTable::SymbolTable() {
	//open the mars file
	openFile();

	//add initial tables
	symbolTable.push_back(initializedMainTable());
	symbolTable.push_back(Table());
}

SymbolTable* SymbolTable::getInstance() {
	if(symbolTableInstance == NULL) {
		symbolTableInstance = new SymbolTable;
	}
	return symbolTableInstance;
}

void SymbolTable::openFile() {
	outputFile.open("cpsl.asm");
	if(!verbose)
		verboseFile.open("cpsl_verbose.output");
}

std::ofstream& SymbolTable::getFileStream() {
	return outputFile;
}

std::ostream& SymbolTable::getErrorStream() {
	return std::cout;
}

std::ostream& SymbolTable::getVerboseStream() {
	if(verbose) {
		return std::cout;
	} else {
		return verboseFile;
	}
}

void SymbolTable::add(std::string identifier, Symbol* symbol) {
	SymbolTable* tableInstance = getInstance();

	tableInstance->symbolTable.back().add(identifier, symbol);
}

Symbol* SymbolTable::lookup(std::string name) {
 	SymbolTable* tableInstance = getInstance();

	for(int i = tableInstance->symbolTable.size()-1; i >= 0; i--) {
	
		Symbol* symbol = tableInstance->symbolTable[i].lookup(name);
		
		if(symbol) {
			return symbol;
		}
	}
	
	getInstance()->getErrorStream() << "The symbol(" << name << ") is not in the table so I will die." << std::endl;
	yyerror("Symbol not found in symbol table.");
	
	return NULL;
}

void SymbolTable::pop() {
	SymbolTable* tableInstance = getInstance();

	getInstance()->getVerboseStream() << "*********** This is level: " << tableInstance->symbolTable.size()-1 << " **********" << std::endl;

	tableInstance->symbolTable.back().print();

	getInstance()->getVerboseStream() << "*********** We are popping the above table off the stack. **********" << std::endl;
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

void SymbolTable::addVar(std::deque<std::string>* identList, Type* type) {
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
			getInstance()->getErrorStream() << "This is and unknown constant type. I quit.\n";
			yyerror("Unknown Constant type.");
		}
	} else {
		yyerror("We quit. You are trying to add constants of different types.");
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

	yyerror("I don't know how to evaluate this constant.");
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

	yyerror("That was an invalid operator for integers. I quit.\n");
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

	yyerror("That was an invalid operator for characters. I quit.");

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

	yyerror("That was an invalid operator for strings. I quit.");

	return NULL;
}

Variable* SymbolTable::makeLvalue(std::string identifier) {
	Symbol* symbol;

	Variable* variable;
	try {
		variable = dynamic_cast<Variable*>(lookup(identifier));
	}
	catch (std::exception& e) {
		std::cout << "Exception: " << e.what() << std::endl;
		yyerror("We threw a bad_cast when making an lvalue.");
	}

	return variable;
}

Expression* SymbolTable::lValueToExpression(Variable* variable) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# loading an lvalue into a register." << std::endl;

	Expression* expression = load(variable->location);
	expression->type = variable->type;
	return expression;
}

Expression* SymbolTable::integerConstToExpression(int value) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# loading a constant integer into a register." << std::endl;

	return loadImmediateInt(value);
}

Expression* SymbolTable::charConstToExpression(std::string value) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# loading a constant character into a register." << std::endl;

	return loadImmediateChar(value);
}

Expression* SymbolTable::stringConstToExpression(std::string value) {
	std::string label = addStringConstant(value);

	int location = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# loading a constant string into a register." << std::endl
			<< "la $" << location << ", " << label << std::endl
			<< std::endl;

	Expression* expression = new Expression(location);
	expression->type = dynamic_cast<Type*>(lookup("string"));
	return expression;
}

Expression* SymbolTable::identToExpression(std::string identifier) {
	Symbol* symbol = lookup(identifier);

	std::cout << "Figure out what to do here.................................................." << std::endl;

	return new Expression(10);
}

Constant* SymbolTable::lookupConstant(std::string identifier) {
	return dynamic_cast<Constant*>(lookup(identifier));
}

Expression* SymbolTable::expression(Expression* left, std::string op, Expression* right){
	if(left->type == right->type) {
		if(op == "or") {
			Expression* expression = eval(left, right, "or");
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		} else if(op == "and") {
			Expression* expression = eval(left, right, "and");
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		} else if (op == "equal") {
			Expression* expression = left;
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		} else if (op == "<>") {
			Expression* expression = left;
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		} else if (op == "<=") {
			Expression* expression = left;
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		} else if (op == ">=") {
			Expression* expression = left;
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		} else if (op == "<") {
			Expression* expression = eval(left, right, "slt");
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		} else if (op == ">") {
			Expression* expression = eval(right, left, "slt");
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		} else if (op == "mult") {
			return evalMult(left, right);
		} else if (op == "div") {
			return evalDiv(left, right);
		} else if (op == "mod") {
			return evalMod(left, right);
		} else {
			return eval(left, right, op);
		}
	} else {
		yyerror("We have a type problem. You must have matching types in expressions.");
	}
}

Expression* SymbolTable::expression(std::string, Expression* right){
	// std::cout << "Expression: op, symbol" << std::endl;
	return right;
}

Expression* SymbolTable::function_call(std::string identifier) {
	// std::cout << "Function Call: no params." << std::endl;
	return new Expression(0);
}

Expression* SymbolTable::function_call(std::string identifier, std::deque<Expression*>*) {
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

std::deque<Expression*>* SymbolTable::makeExpressionList(Expression* expression, std::deque<Expression*>* expressions) {
	if(expressions == NULL) {
		expressions = new std::deque<Expression*>;
	}

	expressions->push_front(expression);

	return expressions;
}

std::deque<Variable*>* SymbolTable::makeVariableList(Variable* variable, std::deque<Variable*>* variables) {
	if(variables == NULL) {
		variables = new std::deque<Variable*>;
	}

	variables->push_front(variable);

	return variables;
}

std::deque<Symbol*>* SymbolTable::makeSymboldeque(Symbol* symbol, std::deque<Symbol*>* symbols) {
	if(symbols == NULL) {
		symbols = new std::deque<Symbol*>;
	}

	symbols->push_back(symbol);

	return symbols;
}

std::deque<Type*>* SymbolTable::makeTypeList(std::deque<Symbol*>* identifiers, Type* type, std::deque<Type*>* symbols) {
	if(symbols == NULL) {
		symbols = new std::deque<Type*>;
	}

	for(int i = 0; i < (int)identifiers->size(); i++) {
		Type* newType = new Type(*type);
		newType->name = (*identifiers)[i]->name;
		symbols->push_front(newType);
	}

	return symbols;
}

std::deque<std::string>* SymbolTable::makeIdentList(std::string identifier, std::deque<std::string>* identList) {
	if(identList == NULL) {
		identList = new std::deque<std::string>;
	}

	identList->push_front(identifier);

	return identList;
}

std::deque<std::pair<std::deque<std::string>, Type*> >* SymbolTable::makeRecordItem(std::deque<std::string>* identList, Type* type, std::deque<std::pair<std::deque<std::string>, Type*> >* recordItem) {
	if(recordItem == NULL) {
		recordItem = new std::deque<std::pair<std::deque<std::string>, Type*> >;
	}

	std::pair<std::deque<std::string>, Type*> thePair = std::make_pair(*identList, type);

	recordItem->push_back(thePair);

	return recordItem;
}

////////////////////////// Strings //////////////////////////////////////

std::string SymbolTable::addStringConstant(std::string value) {
	std::string label = getStringConstantLabel();
	getInstance()->stringConstants[label] = value;
	return label;
}

void SymbolTable::printStringConstants() {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# This is the string area." << std::endl
			<< "\t.data" << std::endl;

	for(std::pair<std::string, std::string> pair : getInstance()->stringConstants) {
		outFile << pair.first << ":\t.asciiz " << pair.second << std::endl;
	}
}

std::string SymbolTable::getStringConstantLabel() {
	return "str" + std::to_string(currentConstString++);
}

////////////////////////////////////////////////////////////////////////////

////////////////////////// Statements //////////////////////////////////////

void SymbolTable::endStatement() {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# this is the end of a statement." << std::endl
			<< std::endl;

	currentRegister = 8;
}

////////////////////////////////////////////////////////////////////////////


///////////////////////// Initialize Assembly //////////////////////////////

void SymbolTable::initAssembly() {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\t.text" << std::endl
			<< "\t.globl main" << std::endl
			<< "main: "//la $gp, GA" << std::endl
			// << "\tb _begin" << std::endl
			<< std::endl;

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
}

////////////////////////////////////////////////////////////////////////////

//////////////////////////// Read and Write ////////////////////////////////

void SymbolTable::write(std::deque<Expression*>* expressionList) {
	for(Expression* expression: *expressionList) {
		int location = lookup(expression);

		if(expression->type == dynamic_cast<Type*>(lookup("integer"))) {
			writeInteger(location);
		} 
		else if(expression->type == dynamic_cast<Type*>(lookup("string"))) {
			writeString(location);
		}
		else if(expression->type == dynamic_cast<Type*>(lookup("char"))) {
			writeCharacter(location);
		}
		else if(expression->type == dynamic_cast<Type*>(lookup("boolean"))) {
			writeInteger(location);
		}
		else {
			std::ofstream& outFile = getInstance()->getFileStream();

			outFile << "# Trying to write an unknown type.\n";
		}
	}
}

void SymbolTable::writeInteger(int location) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
		  	<< "# Writing an integer to the console." << std::endl
		  	<< "li	$v0, 1" << std::endl
		  	<< "move	$a0, $" << location << std::endl
		  	<< "syscall" << std::endl
		  	<< std::endl;
}

void SymbolTable::writeCharacter(int location) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# Writing a character to the console." << std::endl
			<< "li	$v0, 11" << std::endl
			<< "move	$a0, $" << location << std::endl
			<< "syscall" << std::endl
			<< std::endl;
}

void SymbolTable::writeString(int location) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# Writing a string to the console." << std::endl
			<< "li	$v0, 4" << std::endl
			<< "move	$a0, $" << location << std::endl
			<< "syscall" << std::endl
			<< std::endl;
}

void SymbolTable::read(std::deque<Variable*>* variableList) {
	for(Variable* variable : * variableList) {
		if(variable->type == dynamic_cast<Type*>(lookup("integer"))) {
			readInteger(variable);
		} 
		else if(variable->type == dynamic_cast<Type*>(lookup("char"))) {
			readCharacter(variable);
		}
		else {
			std::ofstream& outFile = getInstance()->getFileStream();

			outFile << "Trying to read an unknown type..\n";
		}
	}
}

void SymbolTable::readInteger(Variable* variable) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# Reading an integer from the console." << std::endl
			<< "li		$v0, 5" << std::endl
			<< "syscall" << std::endl
			<< "sw	$v0, " << variable->location << "($sp)" << std::endl
			<< std::endl;
}

void SymbolTable::readString() {
	// TODO: figure out how to do this.

	// std::ofstream& outFile = getInstance()->getFileStream();

	// outFile << std::endl
	// 		<< "# Reading an integer from the console." << std::endl
	// 		<< "li		$v0, 5" << std::endl
	// 		<< "syscall" << std::endl
	// 		<< "move	$" << location << ", $v0" << std::endl
	// 		<< std::endl;
}

void SymbolTable::readCharacter(Variable* variable) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# Reading a character from the console." << std::endl
			<< "li		$v0, 12" << std::endl
			<< "syscall" << std::endl
			<< "sw	$v0, " << variable->location << "($sp)" << std::endl
			<< std::endl;
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

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# evaluating an expression. " << std::endl
			<< operation << " $" << resultLocation
			<< ", $" << leftLocation 
			<< ", $" << rightLocation
			<< std::endl;

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::evalMult(Expression* left, Expression* right) {
	int leftLocation = lookup(left);
	int rightLocation = lookup(right);
	int resultLocation = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# evaluating multiply expression. " << std::endl
			<< "mult $" << leftLocation << ", $" << rightLocation << std::endl
			<< "mflo $" << resultLocation << std::endl
			<< std::endl;

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::evalDiv(Expression* left, Expression* right) {
	int leftLocation = lookup(left);
	int rightLocation = lookup(right);
	int resultLocation = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# evaluating divide expression. " << std::endl
			<< "div $" << leftLocation << ", $" << rightLocation << std::endl
			<< "mflo $" << resultLocation << std::endl
			<< std::endl;

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::evalMod(Expression* left, Expression* right) {
	int leftLocation = lookup(left);
	int rightLocation = lookup(right);
	int resultLocation = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# evaluating mod expression. " << std::endl
			<< "div $" << leftLocation << ", $" << rightLocation << std::endl
			<< "mfhi $" << resultLocation << std::endl
			<< std::endl;

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::load(int location) {
	int resultLocation = getRegister();
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "lw $" << resultLocation
			<< ", " << location << "($sp)"
			<< std::endl;

	return new Expression(resultLocation);
}

Expression* SymbolTable::load(std::string name) {
	std::pair<int, int> offset = lookupExpression(name);
	int resultLocation = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "lw $" << resultLocation << ", $" << offset.second << "(" << offset.first << ")" << std::endl
			<< std::endl;

	return new Expression(resultLocation); 
}

Expression* SymbolTable::loadImmediateInt(int value) {
	int location = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "li	$" << location << ", " << value << std::endl 
			<< std::endl;
	
	Expression* expression = new Expression(location);
	expression->type = dynamic_cast<Type*>(lookup("integer"));
	return expression;
}

Expression* SymbolTable::loadImmediateChar(std::string value) {
	int location = getRegister();
	
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "li	$" << location << ", " << value << std::endl 
			<< std::endl;
	
	Expression* expression = new Expression(location);
	expression->type = dynamic_cast<Type*>(lookup("char"));
	return expression;
}

////////////////////////////////////////////////////////////////////////////

/*	strings

	.data 

	string1: asciz "STRING"

	GA: 
	.space
*/

//// End SymbolTable ////