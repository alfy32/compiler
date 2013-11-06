#include "SymbolTable.hpp"

extern bool verbose;

int For::labelCount = 1;
int While::labelCount = 1;
int Repeat::labelCount = 1;

SymbolTable* SymbolTable::symbolTableInstance = NULL;

int SymbolTable::currentRegister = 8;

int SymbolTable::currentOffset = 0;

int SymbolTable::currentConstString = 1;

//// Expression ////

Expression::Expression(int location) {
	this->constant = NULL;
	this->location = location;
}

Expression::Expression(Constant* constant, Type* type) {
	this->constant = constant;
	this->type = type;
}

int Expression::getLocation() {
	return this->location;
}

//// End Expression ////

LValue::LValue(std::string identifier) {
	Symbol* symbol = SymbolTable::lookup(identifier);

	if(symbol->isConstant) {
		constant = dynamic_cast<Constant*>(symbol);
		variable = NULL;
	} else {
		variable = dynamic_cast<Variable*>(symbol);
		constant = NULL;
	}
}

//// Table ////

Table::Table() {
	
}

void Table::add(std::string identifier, Symbol* symbol) {
	if(symbol == NULL)
		return;

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

	for(std::pair<std::string, Symbol*> pair : tableMap) {
		std::ostream& out = SymbolTable::getInstance()->getVerboseStream();

		out << " Name: " << pair.first << " Symbol: " << pair.second << std::endl;
		
		if(pair.second != NULL)
			pair.second->print(out);
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
	outputFile.open(asmFileName);
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
	if(symbol == NULL)
		return;

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
	if(identList == NULL || type == NULL) {
		yyerror("Add var function found a null.");
		return;
	}

	for(std::string identifier : *identList) {
		Variable* var = new Variable(identifier, type, currentOffset);
		add(identifier, var);
		currentOffset += type->size;
	}
}

void SymbolTable::constDecl(std::string identifier, Constant* constExpression) {
	if(constExpression == NULL) {
		yyerror("constDecl Function got a null.");
	}

	constExpression->name = identifier;
	add(identifier, constExpression);
}

void SymbolTable::typeDecl(std::string identifier, Type* type) {
	if(type == NULL) {
		yyerror("typeDecl function got an null type");
	}

	type->name = identifier;
	add(identifier, type);
}

void SymbolTable::funcDecl(std::string identifier, Func* func) {
	if(func == NULL) {
		yyerror("funcDecl function got a null");
	}

	add(identifier, func);

	addNewScope(func);
}

void SymbolTable::addNewScope(Func* func) {
	if(func == NULL) {
		yyerror("addNewScope got a null func");
	}

	SymbolTable* tableInstance = getInstance();

	Table table;
	tableInstance->symbolTable.push_back(table);

	currentOffset = 0;

	for(std::pair<std::string, Type*> pair : func->signature) {
		add(pair.first, pair.second);
	}
}

void SymbolTable::procDecl(std::string identifier, Proc* proc) {
	if(proc == NULL) {
		yyerror("procDecl got a null proc");
	}

	add(identifier, proc);

	addNewScope(proc);
}

void SymbolTable::addNewScope(Proc* proc) {
	if(proc == NULL) {
		yyerror("addNewScope got a null proc");
	}

	SymbolTable* tableInstance = getInstance();

	Table table;		
	tableInstance->symbolTable.push_back(table);
	
	for(std::pair<std::string, Type*> pair : proc->signature) {
		add(pair.first, pair.second);
	}
}

Constant* SymbolTable::evalConstant(Constant* left, std::string oper, Constant* right) {
	if(left == NULL || right == NULL) {
		yyerror("evalConstant got a null Constant pointer.");
	}

	if(left->constType == right->constType) {
		switch(left->constType)
		{
		case CONST_INT:
			evalIntConstant(left, oper, right);
			break;
		case CONST_CHAR:
			evalCharConstant(left, oper, right);
			break;
		case CONST_STRING:
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
	if(right == NULL) {
		yyerror("evalConstant got a null right.");
	}

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
	if(left == NULL || right == NULL) {
		yyerror("evalIntConstant got a null Constant pointer.");
	}

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
	if(left == NULL || right == NULL) {
		yyerror("evalCharConstant got a null Constant pointer.");
	}

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
	if(left == NULL || right == NULL) {
		yyerror("evalStrConstant got a null Constant pointer.");
	}
	
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



Expression* SymbolTable::lValueToExpression(LValue* lvalue) {
	if(lvalue == NULL) {
		yyerror("lValueToExpression got a null lvalue.");
	}

	Expression* expression = NULL;

	if(lvalue->variable != NULL) {
		Variable* variable = lvalue->variable;

		std::ofstream& outFile = getInstance()->getFileStream();

		expression = load(variable->location);
		expression->type = variable->type;

		return expression;
	} else {
		//TODO: get the right constant.

		//ConstType constType = lvalue->constant->constType;

		IntegerConstant* intConst = dynamic_cast<IntegerConstant*>(lvalue->constant);
			expression = integerConstToExpression(intConst->val);
			expression->type = dynamic_cast<Type*>(lookup("integer"));
			return expression;

		// switch(constType) {
		// case CONST_STRING:
			
		// 	StringConstant* strConst = dynamic_cast<StringConstant*>(lvalue->constant);
		// 	expression = stringConstToExpression(strConst->val);
		// 	expression->type = dynamic_cast<Type*>(lookup("string"));
		// 	return expression;

		// 	break;
		// case CONST_INT:

		// 	IntegerConstant* intConst = dynamic_cast<IntegerConstant*>(lvalue->constant);
		// 	expression = intConstToExpression(intConst->val);
		// 	expression->type = dynamic_cast<Type*>(lookup("integer"));
		// 	return expression;

		// 	break;
		// case CONST_CHAR:
		// 	break;
		// };

		std::cout << "We found a const LVALUE" << std::endl;
		yyerror("CONST LVALUE");
	}
	return expression;
}

Expression* SymbolTable::integerConstToExpression(int value) {
	return loadImmediateInt(value);

	//TODO: figure out constant folding
	// Type* type = dynamic_cast<Type*>(lookup("integer"));
	// return new Expression(new IntegerConstant(value), type);
}

Expression* SymbolTable::charConstToExpression(std::string value) {
	return loadImmediateChar(value);

	//TODO: figure out constant folding.
	// Type* type = dynamic_cast<Type*>(lookup("char"));
	// return new Expression(new CharacterConstant(value), type);
}

Expression* SymbolTable::stringConstToExpression(std::string value) {
	// Type* type = dynamic_cast<Type*>(lookup("string"));
	// return new Expression(new StringConstant(value), type);

	std::string label = addStringConstant(value);

	int location = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tla\t$" << location << ", " << label << "\t # const str to reg." << std::endl;

	Expression* expression = new Expression(location);
	expression->type = dynamic_cast<Type*>(lookup("string"));
	return expression;
}

Constant* SymbolTable::lookupConstant(std::string identifier) {
	return dynamic_cast<Constant*>(lookup(identifier));
}

Expression* SymbolTable::expression(Expression* left, std::string op, Expression* right){
	if(left == NULL || right == NULL) {
		yyerror("expression funciton got a null expression.");
	}

	if(left->type == right->type) {
		if(op == "or") {
			Expression* expression = eval(left, right, "or");
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		} else if(op == "and") {
			Expression* expression = eval(left, right, "and");
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		} else if (op == "seq" || op == "sne" || 
				   op == "sle" || op == "sge" ||
				   op == "slt" || op == "sgt") {
			Expression* expression = eval(left, right, op);
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
	if(right == NULL) {
		yyerror("expression string right got a null");
	}

	// std::cout << "Expression: op, symbol" << std::endl;
	return right;
}

Expression* SymbolTable::function_call(std::string identifier) {
	Symbol* symbol = lookup(identifier);
	if(symbol == NULL) {
		yyerror("the function is not in the symbol table.");
	}

	Func* function = dynamic_cast<Func*>(symbol);

	Expression* expression = new Expression(getRegister());
	expression->type = function->returnType;

	std::ostream& outFile = getInstance()->getFileStream();

	outFile << "jal\t" << identifier << "\t# function call" << std::endl
			<< "\tmove $" << expression->location << ", $v0" << "\t# move return value to new register." << std::endl;
	
	return expression;
}

Expression* SymbolTable::function_call(std::string identifier, std::deque<Expression*>* expressionList) {
	if(expressionList == NULL) {
		yyerror("the expression list for the function is null. I quit.");
	}

	Symbol* symbol = lookup(identifier);
	if(symbol == NULL) {
		yyerror("this function is not in the symbol table.");
	}

	Func* function = dynamic_cast<Func*>(symbol);

	if(expressionList->size() != function->signature.size()) {
		yyerror("The argument list is a different size than the parameter list.");
	}

	Expression* expression = new Expression(getRegister());
	expression->type = function->returnType;

	std::ostream& outFile = getInstance()->getFileStream();

	for(int i = 0; i < function->signature.size(); i++) {
		std::pair<std::string, Type*> parameter = function->signature[i];
		Expression* argument = (*expressionList)[i];

		if(argument == NULL) {
			yyerror("Argument is a null pointer. Talk to your compiler writer.");
		}

		if(parameter.second != argument->type) {
			yyerror("Argument of the function is the wrong type.");
		}

		outFile << "\tmove $a" << i << ", $" << argument->location << "\t# moving value to argument register." << std::endl;
	}

	outFile << "jal\t" << identifier << "\t# function call" << std::endl
			<< "\tmove $" << expression->location << ", $v0" << "\t# move return value to new register." << std::endl;
	
	return expression;
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


void SymbolTable::assignment(LValue* lvalue, Expression* expression) {
	if(lvalue == NULL || expression == NULL) {
		yyerror("assignment got a null");
	}

	int varLoc = lvalue->variable->location;
	int expLoc = expression->location;

	std::ostream& outFile = getInstance()->getFileStream();

	outFile << "\tsw\t$" << expLoc << ", " << varLoc << "($sp)" << "\t# assignment statement." << std::endl;
}

////////////////////////// Lvalue //////////////////////////////////////////

LValue* SymbolTable::makeLValue(std::string identifier) {
	return new LValue(identifier);
}

std::deque<LValue*>* SymbolTable::makeLValueList(LValue* lvalue, std::deque<LValue*>* lvalues) {
	if(lvalues == NULL) {
		lvalues = new std::deque<LValue*>;
	}

	lvalues->push_front(lvalue);

	return lvalues;
}

////////////////////////////////////////////////////////////////////////////

////////////////////////// Strings //////////////////////////////////////

std::string SymbolTable::addStringConstant(std::string value) {
	std::string label = getStringConstantLabel();
	getInstance()->stringConstants[label] = value;
	return label;
}

void SymbolTable::printStringConstants() {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "# This is the string area." << std::endl
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
	// std::ofstream& outFile = getInstance()->getFileStream();

	// outFile << "\t# this is the end of a statement." << std::endl;

	currentRegister = 8;
}

////////////////////////////////////////////////////////////////////////////


///////////////////////// Initialize Assembly //////////////////////////////

void SymbolTable::initAssembly() {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\t.text" << std::endl
			<< "\t.globl main" << std::endl
			<< "main: " //la $gp, GA" << std::endl
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
	if(expressionList == NULL) {
		yyerror("write got a null deque");
	}

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
			yyerror("Trying to write an unknown type.\n");
		}
	}
}

void SymbolTable::writeInteger(int location) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tli	$v0, 1" << "\t# write int." << std::endl
		  	<< "\tmove	$a0, $" << location << std::endl
		  	<< "\tsyscall" << std::endl;
}

void SymbolTable::writeCharacter(int location) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tli	$v0, 11" << "\t# write char." << std::endl
			<< "\tmove	$a0, $" << location << std::endl
			<< "\tsyscall" << std::endl;
}

void SymbolTable::writeString(int location) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tli	$v0, 4" << "\t# write string." << std::endl
			<< "\tmove	$a0, $" << location << std::endl
			<< "\tsyscall" << std::endl;
}

void SymbolTable::read(std::deque<LValue*>* lvalueList) {
	if(lvalueList == NULL) {
		yyerror("read got a null deque");
	}

	for(LValue* lvalue : *lvalueList) {
		Variable* variable = lvalue->variable;

		if(variable->type == dynamic_cast<Type*>(lookup("integer"))) {
			readInteger(variable);
		} 
		else if(variable->type == dynamic_cast<Type*>(lookup("char"))) {
			readCharacter(variable);
		}
		else {
			yyerror("Trying to read an unknown type..\n");
		}
	}
}

void SymbolTable::readInteger(Variable* variable) {
	if(variable == NULL) {
		yyerror("readInteger got a null variable");
	}

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tli	$v0, 5" << "\t# read int." << std::endl
			<< "\tsyscall" << std::endl
			<< "\tsw	$v0, " << variable->location << "($sp)" << std::endl;
}

void SymbolTable::readString() {
	// TODO: figure out how to do this.

	// std::ofstream& outFile = getInstance()->getFileStream();

	// outFile << std::endl
	// 		<< "# Reading an integer from the console." << std::endl
	// 		<< "li		$v0, 5" << std::endl
	// 		<< "syscall" << std::endl
	// 		<< "move	$" << location << ", $v0" << std::endl;
}

void SymbolTable::readCharacter(Variable* variable) {
	if(variable == NULL) {
		yyerror("readCharacter got a null variable");
	}

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tli	$v0, 12" << "\t# read char." << std::endl
			<< "\tsyscall" << std::endl
			<< "\tsw	$v0, " << variable->location << "($sp)" << std::endl;
}

////////////////////////////////////////////////////////////////////////////

///////////////////////////// Expressoins //////////////////////////////////


int SymbolTable::getRegister() {
	return currentRegister++;
}

int SymbolTable::lookup(Expression* expression) {
	if(expression == NULL) {
		yyerror("lookup got a null expression");
	}

	return expression->location;
}

std::pair<int,int> SymbolTable::lookupExpression(std::string name) {
	int location, offset;
	//TODO: find the location given a string.
	return std::make_pair(location, offset);
}

Expression* SymbolTable::eval(Expression* left, Expression* right, std::string operation) {
	if(left == NULL || right == NULL) {
		yyerror("eval got a null expression.");
	}

	int leftLocation = lookup(left);
	int rightLocation = lookup(right);
	int resultLocation = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\t" << operation << " $" << resultLocation 
			<< ", $" << leftLocation 
			<< ", $" << rightLocation
			<< "\t# evaluating an expression. " << std::endl;

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::evalMult(Expression* left, Expression* right) {
	if(left == NULL || right == NULL) {
		yyerror("evalMult got a null expression.");
	}

	int leftLocation = lookup(left);
	int rightLocation = lookup(right);
	int resultLocation = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tmult	$" << leftLocation << ", $" << rightLocation << "\t# evaluating multiply expression. " << std::endl
			<< "\tmflo	$" << resultLocation << std::endl;

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::evalDiv(Expression* left, Expression* right) {
	if(left == NULL || right == NULL) {
		yyerror("evalDiv got a null expression.");
	}

	int leftLocation = lookup(left);
	int rightLocation = lookup(right);
	int resultLocation = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tdiv\t$" << leftLocation << ", $" << rightLocation << "\t# evaluating divide expression. " << std::endl
			<< "\tmflo\t$" << resultLocation << std::endl;

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::evalMod(Expression* left, Expression* right) {
	if(left == NULL || right == NULL) {
		yyerror("evalMod got a null expression.");
	}

	int leftLocation = lookup(left);
	int rightLocation = lookup(right);
	int resultLocation = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tdiv\t$" << leftLocation << ", $" << rightLocation << "\t# evaluating mod expression. " << std::endl
			<< "\tmfhi\t$" << resultLocation << std::endl;

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::load(int location) {
	int resultLocation = getRegister();
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tlw\t$" << resultLocation
			<< ", " << location << "($sp)"
			<< std::endl;

	return new Expression(resultLocation);
}

Expression* SymbolTable::load(std::string name) {
	std::pair<int, int> offset = lookupExpression(name);
	int resultLocation = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "lw $" << resultLocation << ", $" << offset.second << "(" << offset.first << ")" << std::endl;

	return new Expression(resultLocation); 
}

Expression* SymbolTable::loadImmediateInt(int value) {
	int location = getRegister();

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tli	$" << location << ", " << value << "\t# load const int." << std::endl;
	
	Expression* expression = new Expression(location);
	expression->type = dynamic_cast<Type*>(lookup("integer"));
	return expression;
}

Expression* SymbolTable::loadImmediateChar(std::string value) {
	int location = getRegister();
	
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tli	$" << location << ", " << value << "\t# load const char." << std::endl;
	
	Expression* expression = new Expression(location);
	expression->type = dynamic_cast<Type*>(lookup("char"));
	return expression;
}

void SymbolTable::store(Variable* variable, Expression* expression) {
	if(variable == NULL || expression == NULL) {
		yyerror("store got a null.");
	}

	if(variable->type != expression->type) {
		yyerror("Type mismatch on the store.");
	}

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tsw	$" << expression->location << ", " << variable->location << "($sp)" << std::endl;
}

////////////////////////////////////////////////////////////////////////////

/////////////////////////////// If Statements //////////////////////////////

int SymbolTable::elseCount = 1;
int SymbolTable::endCount = 1;

void SymbolTable::pushElse() {
	getInstance()->elseStack.push_front("ELSE" + std::to_string(elseCount++));
}

void SymbolTable::pushEnd() {
	getInstance()->endStack.push_front("END" + std::to_string(endCount++));
}

void SymbolTable::popElse() {
	getInstance()->elseStack.pop_front();
}

void SymbolTable::popEnd() {
	getInstance()->endStack.pop_front();
}

std::string SymbolTable::elseFront() {
	return getInstance()->elseStack.front();
}

std::string SymbolTable::endFront() {
	return getInstance()->endStack.front();
}

void SymbolTable::afterIf() {
	std::ofstream& outFile = getInstance()->getFileStream();

	while(getInstance()->endStack.size() > 0) {
		outFile << endFront() << ":" << std::endl;
		popEnd();
	}
}

void SymbolTable::ifStatement(Expression* expression) {
	if(expression->type != lookup("boolean")) {
		yyerror("You must have a boolean expression in an if statement.");
	}	

	std::ofstream& outFile = getInstance()->getFileStream();

	pushElse();

	outFile << "\tbeqz $" << expression->location << " " << elseFront() << "\t# if statement." << std::endl;
}

void SymbolTable::thenStatement() {
	std::ofstream& outFile = getInstance()->getFileStream();

	pushEnd();

	outFile << "\tj " << endFront() << std::endl
			<< elseFront() << ":" << std::endl;

	popElse();
}

void SymbolTable::elseIfStatement(Expression* expression) {
	std::ofstream& outFile = getInstance()->getFileStream();

}

void SymbolTable::elseStatement() {
	std::ofstream& outFile = getInstance()->getFileStream();
}


////////////////////////////////////////////////////////////////////////////

/////////////////////////////// For Statements /////////////////////////////

void SymbolTable::initFor(std::string identifier, Expression* expression) {
	if(expression == NULL) {
		yyerror("initFor got a null expression.");
	}

	Variable* variable = dynamic_cast<Variable*>(lookup(identifier));

	if(variable->type != expression->type) {
		yyerror("We have a problem. The initial variable of this for loop is being assigned the wrong type.");
	}

	For forVar(variable);

	getInstance()->forStack.push_back(forVar);

	store(variable, expression);
}

void SymbolTable::forLabel(std::string to) {
	std::ofstream& outFile = getInstance()->getFileStream();

	getInstance()->forStack.back().to = to;
	std::string forLabel = getInstance()->forStack.back().forLabel;	

	outFile << forLabel << ":" << std::endl;
}

void SymbolTable::forEval(Expression* expression) {
	if(expression == NULL) {
		yyerror("forEval got a null expression.");
	}

	std::ofstream& outFile = getInstance()->getFileStream();

	For forVar = getInstance()->forStack.back();

	Expression* counter = load(forVar.var->location);
	counter->type = forVar.var->type;

	if(forVar.to == "UP") {
		outFile << "\tbgt $" << counter->location << ", $" << expression->location << ", " << forVar.endLabel << std::endl;
	} else if(forVar.to == "DOWN") {
		outFile << "\tblt $" << counter->location << ", $" << expression->location << ", " << forVar.endLabel << std::endl;
	} else {
		yyerror("We don't know if this is an up to or down to for loop.");
	}
}

void SymbolTable::forEnd() {
	std::ofstream& outFile = getInstance()->getFileStream();

	For forVar = getInstance()->forStack.back();

	Expression* expression = load(forVar.var->location);
	expression->type = forVar.var->type;
	int location = expression->location;

	if(forVar.to == "UP") {
		outFile << "\taddi	$" << location << ", $" << location << ", 1" << std::endl;
	} else if(forVar.to == "DOWN") {
		outFile << "\tsubi	$" << location << ", $" << location << ", 1" << std::endl;
	} else {
		yyerror("We don't know if this is an up to or down to for loop.");
	}

	store(forVar.var, expression);

	outFile	<< "\tb\t" << forVar.forLabel << std::endl
			<< forVar.endLabel << ":" << std::endl;

	getInstance()->forStack.pop_back();	
}

////////////////////////////////////////////////////////////////////////////

///////////////////////////// While Statements /////////////////////////////

void SymbolTable::whileInit() {
	std::ofstream& outFile = getInstance()->getFileStream();

	While whileVar;

	getInstance()->whileStack.push_back(whileVar);

	outFile << whileVar.whileLabel << ":" << std::endl;
}

void SymbolTable::whileBranch(Expression* expression) {
	if(expression == NULL) {
		yyerror("whileBranch got a null expression.");
	}

	std::ofstream& outFile = getInstance()->getFileStream();

	While whileVar = getInstance()->whileStack.back();

	outFile << "\tbeqz\t $" << expression->location << ", " << whileVar.endLabel << std::endl;
}

void SymbolTable::whileRepeat() {
	std::ofstream& outFile = getInstance()->getFileStream();

	While whileVar = getInstance()->whileStack.back();

	outFile << "\tb\t" << whileVar.whileLabel << std::endl;

	outFile << whileVar.endLabel << ":" << std::endl;

	getInstance()->whileStack.pop_back();
}

////////////////////////////////////////////////////////////////////////////

//////////////////////////// Repeat Statements /////////////////////////////

void SymbolTable::repeatInit() {
	std::ofstream& outFile = getInstance()->getFileStream();

	Repeat repeat;

	getInstance()->repeatStack.push_back(repeat);

	outFile << repeat.repeatLabel << ":" << std::endl;
}

void SymbolTable::repeatEnd(Expression* expression) {
	if(expression == NULL) {
		yyerror("repeatEnd got a null expression.");
	}

	std::ofstream& outFile = getInstance()->getFileStream();

	Repeat repeat = getInstance()->repeatStack.back();

	outFile << "\tbeqz\t$" << expression->location << ", " << repeat.repeatLabel << std::endl;

	getInstance()->repeatStack.pop_back();
}

////////////////////////////////////////////////////////////////////////////

void SymbolTable::stop() {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tli  \t$v0, 10		# stop" << std::endl
			<< "\tsyscall" << std::endl;
}

//// End SymbolTable ////