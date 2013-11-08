#include "SymbolTable.hpp"

extern bool verbose;
extern int lineNumber;

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
	this->location = std::to_string(location);
}

Expression::Expression(Constant* constant, Type* type) {
	this->constant = constant;
	this->type = type;
}

int Expression::getLocation() {
	return std::stoi(this->location);
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
		SymbolTable::error("Symbol already defined.");
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
	
	error("The symbol(" + name + ") is not in the table so I will die.");
	
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
	Boolean* boolTrue = new Boolean(true);
	Boolean* boolFalse = new Boolean(false);
	
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
		error("Add var function found a null.");
	}

	for(std::string identifier : *identList) {
		Variable* var = new Variable(identifier, type, currentOffset);
		add(identifier, var);
		currentOffset += type->size;
	}
}

Variable* SymbolTable::addVar(std::string identifier, Type* type) {
	if(type == NULL) {
		error("Add var function found a null.");
	}

	Variable* var = new Variable(identifier, type, currentOffset);
	add(identifier, var);
	currentOffset += type->size;

	return var;
}

void SymbolTable::constDecl(std::string identifier, Constant* constExpression) {
	if(constExpression == NULL) {
		error("constDecl Function got a null.");
	}

	constExpression->name = identifier;
	add(identifier, constExpression);
}

void SymbolTable::typeDecl(std::string identifier, Type* type) {
	if(type == NULL) {
		error("typeDecl function got an null type");
	}

	type->name = identifier;
	add(identifier, type);
}

void SymbolTable::funcDecl(std::string identifier, Func* func) {
	if(func == NULL) {
		error("funcDecl function got a null");
	}

	add(identifier, func);

	std::ostream& outFile = getInstance()->getFileStream();

	outFile << std::endl 
			<< "# ******** " << identifier << " function ********" << std::endl;
	printLabel(identifier);
			
	addNewScope();
	getInstance()->functionStack.push_back(func);

	// int stackSize = 0;

	// for(std::pair<std::string, Type*> pair : func->signature) {
	// 	stackSize += pair.second->size; 
	// }

	// outFile << "\taddi\t$sp, $sp, -" << stackSize+4 << std::endl;

	// int aReg = 0;

	// for(std::pair<std::string, Type*> pair : func->signature) {
	// 	Variable* var = addVar(pair.first, pair.second);

	// 	std::string reg = "a" + std::to_string(aReg++);

	// 	store(var, reg);
	// }

	// outFile << "\tsw\t$ra, " << currentOffset << "($sp)" << std::endl;
}

void SymbolTable::funcEnd(Func* function, bool isForward) {
	if(function == NULL) {
		error("This function is supposed to end but it is null.");
	}

	function->isForward = isForward;

	// std::ostream& outFile = getInstance()->getFileStream();

	// outFile << "\tlw\t$ra " << currentOffset << "($sp)" << std::endl
	// 		<< "\taddi\t$sp, $sp, " << currentOffset+4 << std::endl
	// 		<< "\tjr $ra \t# ******** end " << function->name << " function ********" << std::endl;

	pop();
}

void SymbolTable::returnStatement(Expression* expression) {
	if(expression == NULL) {
		error("The return expression is null.");
	}

	printInstruction("move", "$v0, $" + expression->location, "return statement.");

	if(getInstance()->functionStack.size() > 0) {

		printInstruction("lw", "$ra, " + std::to_string(blockOffset-4) + "($sp)");
		printInstruction("addi", "$sp, $sp, " + std::to_string(blockOffset));
		printInstruction("jr", "$ra");
	} 
}

void SymbolTable::procDecl(std::string identifier, Proc* proc) {
	if(proc == NULL) {
		error("procDecl got a null proc");
	}

	add(identifier, proc);

	addNewScope();

	for(std::pair<std::string, Type*> pair : proc->signature) {
		add(pair.first, pair.second);
	}
}

void SymbolTable::addNewScope() {
	SymbolTable* tableInstance = getInstance();

	Table table;
	tableInstance->symbolTable.push_back(table);

	currentOffset = 0;
}

Constant* SymbolTable::evalConstant(Constant* left, std::string oper, Constant* right) {
	if(left == NULL || right == NULL) {
		error("evalConstant got a null Constant pointer.");
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
			error("Unknown Constant type.");
		}
	} else {
		error("We quit. You are trying to add constants of different types.");
	}
}

Constant* SymbolTable::evalConstant(std::string oper, Constant* right) {
	if(right == NULL) {
		error("evalConstant got a null right.");
	}

	if(right->constType == CONST_INT) {
		IntegerConstant* value = dynamic_cast<IntegerConstant*>(right);

		if(oper == "~") {
			return new Boolean(!value->val);
		} else if(oper == "neg") {
			return new IntegerConstant(-value->val);
		}
	}

	error("I don't know how to evaluate this constant.");
}

Constant* SymbolTable::evalIntConstant(Constant* left, std::string oper, Constant* right) {
	if(left == NULL || right == NULL) {
		error("evalIntConstant got a null Constant pointer.");
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

	error("That was an invalid operator for integers. I quit.\n");
	exit(1); 

	return NULL;
}

Constant* SymbolTable::evalCharConstant(Constant* left, std::string oper, Constant* right) {
	if(left == NULL || right == NULL) {
		error("evalCharConstant got a null Constant pointer.");
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

	error("That was an invalid operator for characters. I quit.");

	return NULL;
}

Constant* SymbolTable::evalStrConstant(Constant* left, std::string oper, Constant* right) {
	if(left == NULL || right == NULL) {
		error("evalStrConstant got a null Constant pointer.");
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

	error("That was an invalid operator for strings. I quit.");

	return NULL;
}



Expression* SymbolTable::lValueToExpression(LValue* lvalue) {
	if(lvalue == NULL) {
		error("lValueToExpression got a null lvalue.");
	}

	Expression* expression = NULL;

	if(lvalue->variable != NULL) {
		Variable* variable = lvalue->variable;

		std::ofstream& outFile = getInstance()->getFileStream();

		expression = load(variable->location);
		expression->type = variable->type;

		return expression;
	} else {

		ConstType constType = lvalue->constant->constType;

		if(constType == CONST_INT) {
			IntegerConstant* intConst = dynamic_cast<IntegerConstant*>(lvalue->constant);
			expression = integerConstToExpression(intConst->val);
			expression->type = dynamic_cast<Type*>(lookup("integer"));
			return expression;
		} 
		else if(constType == CONST_STRING) {
			StringConstant* strConst = dynamic_cast<StringConstant*>(lvalue->constant);
			expression = stringConstToExpression(strConst->val);
			expression->type = dynamic_cast<Type*>(lookup("string"));
			return expression;
		} 
		else if(constType == CONST_CHAR) {
			CharacterConstant* charConst = dynamic_cast<CharacterConstant*>(lvalue->constant);
			expression = charConstToExpression(charConst->val);
			expression->type = dynamic_cast<Type*>(lookup("char"));
			return expression;
		} 
		else if(constType == CONST_BOOL) {
			Boolean* boolean = dynamic_cast<Boolean*>(lvalue->constant);
			expression = integerConstToExpression(boolean->val == 1 ? 1 : 0);
			expression->type = dynamic_cast<Type*>(lookup("boolean"));
			return expression;
		}

		error("lValueToExpression got a constant with an unknown constant type.");
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
		error("expression funciton got a null expression.");
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
		error("We have a type problem. You must have matching types in expressions.");
	}
}

Expression* SymbolTable::expression(std::string, Expression* right){
	if(right == NULL) {
		error("expression string right got a null");
	}

	// std::cout << "Expression: op, symbol" << std::endl;
	return right;
}

Expression* SymbolTable::function_call(std::string identifier) {
	std::ostream& outFile = getInstance()->getFileStream();

	Symbol* symbol = lookup(identifier);
	if(symbol == NULL) {
		error("the function is not in the symbol table.");
	}

	Func* function = dynamic_cast<Func*>(symbol);

	Expression* expression = new Expression(getRegister());
	expression->type = function->returnType;

	outFile << "\tjal\t" << identifier << "\t# function call" << std::endl
			<< "\tmove $" << expression->location << ", $v0" << "\t# move return value to new register." << std::endl;
	
	return expression;
}

Expression* SymbolTable::function_call(std::string identifier, std::deque<Expression*>* expressionList) {
	std::ostream& outFile = getInstance()->getFileStream();

	if(expressionList == NULL) {
		error("the expression list for the function is null. I quit.");
	}

	Symbol* symbol = lookup(identifier);
	if(symbol == NULL) {
		error("this function is not in the symbol table.");
	}

	Func* function = dynamic_cast<Func*>(symbol);

	if(expressionList->size() != function->signature.size()) {
		error("The argument list for function (" + function->name + ") is a different size than the parameter list.");
	}

	int regsToSave = 0;

	currentRegister -= expressionList->size();
	if(currentRegister < 8 )
		currentRegister = 8;

	if(currentRegister > 8) {
		while(currentRegister > 8) {
			regsToSave++;
			currentRegister--;
			outFile << "\tsw $" << currentRegister << ", -" << regsToSave*4 << "($sp)" << std::endl;
		}
		outFile << "\taddi $sp, $sp, -" << regsToSave*4 << std::endl;
	}

	for(int i = 0; i < function->signature.size(); i++) {
		std::pair<std::string, Type*> parameter = function->signature[i];
		Expression* argument = (*expressionList)[i];

		if(argument == NULL) {
			error("Argument " + std::to_string(i) + " is a null pointer. Talk to your compiler writer.");
		}

		if(parameter.second != argument->type) {
			error("Argument " + std::to_string(i) + " of the function " + function->name + " is the wrong type.");
		}

		outFile << "\tmove $a" << i << ", $" << argument->location << "\t# moving value to argument register." << std::endl;
	}

	outFile << "\tjal\t" << identifier << "\t# function call" << std::endl;

	if(regsToSave) {
		outFile << "\taddi $sp, $sp, " << regsToSave*4 << std::endl;
	}

	while(regsToSave) {
		outFile << "\tlw $" << currentRegister << ", -" << regsToSave*4 << "($sp)" << std::endl;
		regsToSave--;
		currentRegister++;
	}

	Expression* expression = new Expression(getRegister());
	expression->type = function->returnType;

	outFile	<< "\tmove $" << expression->location << ", $v0" << "\t# move return value to new register." << std::endl;
	
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

void SymbolTable::error(std::string e) {
	std::cout << "Error line(" << lineNumber << "): " << e << std::endl;
}


void SymbolTable::assignment(LValue* lvalue, Expression* expression) {
	if(lvalue == NULL || expression == NULL) {
		error("assignment got a null");
	}

	int varLoc = lvalue->variable->location;
	int expLoc = expression->getLocation();

	std::ostream& outFile = getInstance()->getFileStream();

	outFile << "\tsw\t$" << expLoc << ", " << varLoc << "($sp)" << "\t# assignment statement." << std::endl;
}

////////////////////////// Blocks //////////////////////////////////////////

int SymbolTable::blockOffset = 0;

void SymbolTable::beginBlock() {
	std::ostream& outFile = getInstance()->getFileStream();

	blockOffset = currentOffset + 4;

	Func* function = NULL;
	if(getInstance()->functionStack.size() > 0) {
		function = getInstance()->functionStack.back();
	
		for(std::pair<std::string, Type*> pair : function->signature) {
			blockOffset += pair.second->size; 
		}
	}

	outFile << "\taddi\t$sp, $sp, -" << blockOffset << std::endl;

	if(function) {
		int aReg = 0;

		for(std::pair<std::string, Type*> pair : function->signature) {
			Variable* var = addVar(pair.first, pair.second);

			std::string reg = "a" + std::to_string(aReg++);

			store(var, reg);
		}
	}

	outFile << "\tsw  \t$ra, " << blockOffset-4 << "($sp)" << std::endl;
		
}

void SymbolTable::endBlock() {
	std::ostream& outFile = getInstance()->getFileStream();

	if(getInstance()->functionStack.size() > 0) {
		Func* function = getInstance()->functionStack.back();

		outFile << "# ******** " + function->name + " function ********" << std::endl
				<< std::endl;

		getInstance()->functionStack.pop_back();

	// 	printInstruction("lw", "$ra, " + std::to_string(blockOffset-4) + "($sp)");
	// 	printInstruction("addi", "$sp, $sp, " + std::to_string(blockOffset));
	// 	printInstruction("jr", "$ra");
	} 
	else 
	{
		// outFile << "\t# end of program." << std::endl;
	}

	blockOffset = 0;
}

////////////////////////////////////////////////////////////////////////////

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
			<< "main: " << std::endl
			<< "\tb begin" << "\t#jump to main." << std::endl;

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

void SymbolTable::startMain() {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl
			<< "# ******** main function ********" << std::endl
			<< "begin: " << std::endl;
}

void SymbolTable::endMain() {
	std::ofstream& outFile = getInstance()->getFileStream();

	// outFile << "\tlw\t$ra, " << currentOffset << "($sp)" << "\t# Epilog." << std::endl
	// 		<< "\taddi\t$sp, $sp, " << currentOffset+4 << "\t# End Epilog." << std::endl;

	outFile << "# ******** end main function ********" << std::endl
			<< std::endl;

	pop(); 
	pop(); 
	printStringConstants();
}

////////////////////////////////////////////////////////////////////////////

//////////////////////////// Read and Write ////////////////////////////////

void SymbolTable::write(std::deque<Expression*>* expressionList) {
	if(expressionList == NULL) {
		error("write got a null deque");
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
			error("Trying to write an unknown type.\n");
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
		error("read got a null deque");
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
			error("Trying to read an unknown type..\n");
		}
	}
}

void SymbolTable::readInteger(Variable* variable) {
	if(variable == NULL) {
		error("readInteger got a null variable");
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
		error("readCharacter got a null variable");
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
		error("lookup got a null expression");
	}

	return expression->getLocation();
}

std::pair<int,int> SymbolTable::lookupExpression(std::string name) {
	int location, offset;
	//TODO: find the location given a string.
	return std::make_pair(location, offset);
}

Expression* SymbolTable::eval(Expression* left, Expression* right, std::string operation) {
	if(left == NULL || right == NULL) {
		error("eval got a null expression.");
	}

	std::string leftLocation = std::to_string(lookup(left));
	std::string rightLocation = std::to_string(lookup(right));
	int resultLocation = getRegister();
	std::string result = std::to_string(resultLocation);

	printInstruction(operation, "$" + result 
			+ ", $" + leftLocation 
			+ ", $" + rightLocation,
			"evaluating an expression.");

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::evalMult(Expression* left, Expression* right) {
	if(left == NULL || right == NULL) {
		error("evalMult got a null expression.");
	}

	std::string leftLocation = std::to_string(lookup(left));
	std::string rightLocation = std::to_string(lookup(right));
	int resultLocation = getRegister();
	std::string result = std::to_string(resultLocation);

	printInstruction("mult", "$" + leftLocation + ", $" + rightLocation, "evaluating multiply expression.");
	printInstruction("mflo", "$" + result);

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::evalDiv(Expression* left, Expression* right) {
	if(left == NULL || right == NULL) {
		error("evalDiv got a null expression.");
	}

	std::string leftLocation = std::to_string(lookup(left));
	std::string rightLocation = std::to_string(lookup(right));
	int resultLocation = getRegister();
	std::string result = std::to_string(resultLocation);

	printInstruction("div", "$" + leftLocation + ", $" + rightLocation, "evaluating divide expression. ");
	printInstruction("mflo", "$" + result);

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::evalMod(Expression* left, Expression* right) {
	if(left == NULL || right == NULL) {
		error("evalMod got a null expression.");
	}

	std::string leftLocation = std::to_string(lookup(left));
	std::string rightLocation = std::to_string(lookup(right));
	int resultLocation = getRegister();
	std::string result = std::to_string(resultLocation);

	printInstruction("div", "$" + leftLocation + ", $" + rightLocation, "evaluating mod expression. ");
	printInstruction("mfhi", "$" + result);

	Expression* expression = new Expression(resultLocation);
	expression->type = left->type;
	return expression;
}

Expression* SymbolTable::load(int location) {
	int resultLocation = getRegister();

	std::string result = std::to_string(resultLocation);
	std::string loc = std::to_string(location);

	printInstruction("lw" ,"$" + result + ", " + loc + "($sp)");

	return new Expression(resultLocation);
}

Expression* SymbolTable::load(std::string name) {
	std::pair<int, int> location = lookupExpression(name);
	int resultLocation = getRegister();

	std::string result = std::to_string(resultLocation);
	std::string offset = std::to_string(location.first);
	std::string loc = std::to_string(location.second);

	printInstruction("lw", "$" + result + ", $" + loc + "(" + offset + ")" );

	return new Expression(resultLocation); 
}

Expression* SymbolTable::loadImmediateInt(int value) {
	int location = getRegister();

	std::string loc = std::to_string(location);
	std::string val = std::to_string(value);

	printInstruction("li", "$" + loc + ", " + val, "load const int.");
	
	Expression* expression = new Expression(location);
	expression->type = dynamic_cast<Type*>(lookup("integer"));
	return expression;
}

Expression* SymbolTable::loadImmediateChar(std::string value) {
	int location = getRegister();

	std::string loc = std::to_string(location);

	printInstruction("li", "$" + loc + ", " + value, "load const char.");
	
	Expression* expression = new Expression(location);
	expression->type = dynamic_cast<Type*>(lookup("char"));
	return expression;
}

void SymbolTable::store(Variable* variable, Expression* expression) {
	if(variable == NULL || expression == NULL) {
		error("store got a null.");
	}

	if(variable->type != expression->type) {
		error("Type mismatch on the store.");
	}

	std::string varLoc = std::to_string(variable->location);

	printInstruction("sw", "t$" + expression->location + ", " + varLoc + "($sp)");
}

void SymbolTable::store(Variable* variable, std::string reg) {
	if(variable == NULL) {
		error("store got a null.");
	}

	std::string varLoc = std::to_string(variable->location);

	printInstruction("sw", "$" + reg + ", " + varLoc + "($sp)");
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
		printLabel(endFront());
		popEnd();
	}
}

void SymbolTable::ifStatement(Expression* expression) {
	if(expression->type != lookup("boolean")) {
		error("You must have a boolean expression in an if statement.");
	}	

	pushElse();

	printInstruction("beqz", "$" + expression->location + " " + elseFront(), "if statement.");
}

void SymbolTable::thenStatement() {
	pushEnd();

	printInstruction("j", endFront());
	printLabel(elseFront());

	popElse();
}

void SymbolTable::elseIfStatement(Expression* expression) {


}

void SymbolTable::elseStatement() {

}


////////////////////////////////////////////////////////////////////////////

/////////////////////////////// For Statements /////////////////////////////

void SymbolTable::initFor(std::string identifier, Expression* expression) {
	if(expression == NULL) {
		error("initFor got a null expression.");
	}

	Variable* variable = dynamic_cast<Variable*>(lookup(identifier));

	if(variable->type != expression->type) {
		error("We have a problem. The initial variable of this for loop is being assigned the wrong type.");
	}

	For forVar(variable);

	getInstance()->forStack.push_back(forVar);

	store(variable, expression);
}

void SymbolTable::forLabel(std::string to) {
	getInstance()->forStack.back().to = to;
	std::string forLabel = getInstance()->forStack.back().forLabel;	

	printLabel(forLabel);
}

void SymbolTable::forEval(Expression* expression) {
	if(expression == NULL) {
		error("forEval got a null expression.");
	}

	For forVar = getInstance()->forStack.back();

	Expression* counter = load(forVar.var->location);
	counter->type = forVar.var->type;

	if(forVar.to == "UP") {
		printInstruction("bgt", "$" + counter->location + ", $" + expression->location + ", " + forVar.endLabel);
	} else if(forVar.to == "DOWN") {
		printInstruction("blt", "$" + counter->location + ", $" + expression->location + ", " + forVar.endLabel);
	} else {
		error("We don't know if this is an up to or down to for loop.");
	}
}

void SymbolTable::forEnd() {
	For forVar = getInstance()->forStack.back();

	Expression* expression = load(forVar.var->location);
	expression->type = forVar.var->type;
	std::string location = expression->location;

	if(forVar.to == "UP") {
		printInstruction("addi", "$" + location + ", $" + location + ", 1");
	} else if(forVar.to == "DOWN") {
		printInstruction("subi", "$" + location + ", $" + location + ", 1");
	} else {
		error("We don't know if this is an up to or down to for loop.");
	}

	store(forVar.var, expression);

	printInstruction("b", forVar.forLabel);
	printLabel(forVar.endLabel);

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
		error("whileBranch got a null expression.");
	}

	While whileVar = getInstance()->whileStack.back();

	printInstruction("beqz", "$" + expression->location + ", " + whileVar.endLabel);
}

void SymbolTable::whileRepeat() {
	While whileVar = getInstance()->whileStack.back();

	printInstruction("b", whileVar.whileLabel);
	printLabel(whileVar.endLabel);

	getInstance()->whileStack.pop_back();
}

////////////////////////////////////////////////////////////////////////////

//////////////////////////// Repeat Statements /////////////////////////////

void SymbolTable::repeatInit() {
	Repeat repeat;

	getInstance()->repeatStack.push_back(repeat);

	printLabel(repeat.repeatLabel);
}

void SymbolTable::repeatEnd(Expression* expression) {
	if(expression == NULL) {
		error("repeatEnd got a null expression.");
	}

	Repeat repeat = getInstance()->repeatStack.back();

	printInstruction("beqz", "$" + expression->location + ", " + repeat.repeatLabel);

	getInstance()->repeatStack.pop_back();
}

////////////////////////////////////////////////////////////////////////////

void SymbolTable::stop() {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\tli  \t$v0, 10		# stop" << std::endl
			<< "\tsyscall" << std::endl;
}


void SymbolTable::printLabel(std::string label, std::string comment) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << label << ":" << "  " << comment << std::endl;
}

void SymbolTable::printLabel(std::string label) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << label << ":" << std::endl;
}

void SymbolTable::printInstruction(std::string instruction, std::string registers, std::string comment) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\t" << std::left << std::setw(8) << instruction << registers << "\t# " << comment << std::endl;
}

void SymbolTable::printInstruction(std::string instruction, std::string registers) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\t" << std::left << std::setw(8) << instruction << registers << std::endl;
}

//// End SymbolTable ////