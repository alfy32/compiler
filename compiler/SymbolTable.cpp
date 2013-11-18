#include "SymbolTable.hpp"

extern bool verbose;
extern int lineNumber;

int For::labelCount = 1;
int While::labelCount = 1;
int Repeat::labelCount = 1;

SymbolTable* SymbolTable::symbolTableInstance = NULL;

int SymbolTable::currentRegister = 8;

int SymbolTable::currentOffset = 0;
int SymbolTable::globalOffset = 0;

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

LValue::LValue(Type* type, Variable* variable) {
	this->type = type;
	this->variable = variable;
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
	asmFileName = "cpsl.asm";
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

bool SymbolTable::isGlobalStack() {
	return getInstance()->symbolTable.size() <= 2;
}

void SymbolTable::addVar(std::deque<std::string>* identList, Type* type) {
	if(identList == NULL || type == NULL) {
		error("Add var function found a null.");
	}

	for(std::string identifier : *identList) {
		addVariable(identifier, type);
	}
}

Variable* SymbolTable::addVariable(std::string identifier, Type* type) {
	if(type == NULL) {
		error("Add var function found a null.");
	}

	Variable* var = NULL;

	if(isGlobalStack()) {
		var = new Variable(identifier, type, globalOffset, "$gp");
		globalOffset += type->size;
	} else {
		var = new Variable(identifier, type, currentOffset, "$sp");
		currentOffset += type->size;
	}

	add(identifier, var);

	return var;
}

Variable* SymbolTable::initParameter(std::string identifier, Type* type) {
	if(type == NULL) {
		error("Add var function found a null.");
	}

	Variable* var = NULL;

	var = new Variable(identifier, type, currentOffset, "$sp");
	var->isParameter = true;
	currentOffset += 4;

	add(identifier, var);

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

void SymbolTable::funcProcDecl(std::string identifier, FunctionProcedure* funcProc) {
	if(funcProc == NULL) {
		error("funcProcDecl got a null function of procedure");
	}

	add(identifier, funcProc);

	std::ostream& outFile = getInstance()->getFileStream();
	std::string which = "function";
	if(funcProc->returnType == NULL)
		which = "procedure";

	printNewLine();
	outFile << "# ******** " << identifier << " " + which + " ********" << std::endl;
	printLabel(identifier);
			
	addNewScope();
	getInstance()->functionProcedureStack.push_back(funcProc);
}

void SymbolTable::funcProcEnd(FunctionProcedure* funcProc, bool isForward) {
	if(funcProc == NULL) {
		error("NULL pointer.");
	}
	funcProc->isForward = isForward;
	pop();
}

void SymbolTable::returnStatement(Expression* expression) {
	if(getInstance()->functionProcedureStack.size() > 0) {
		FunctionProcedure* functionProcedure = getInstance()->functionProcedureStack.back();

		if(functionProcedure->returnType) {
			if(expression == NULL)
				error("NULL return expression.");

			printInstruction("move", "$v0, $" + expression->location, "return statement.");
		}

		printInstruction("j", functionProcedure->name + "_epilog");
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

		if(variable->type->isRecord || variable->type->isArray) {
			expression = loadAddress(variable);	
		} else {
			expression = load(variable);			
		}

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
	std::string label = addStringConstant(value);

	int location = getRegister();

	std::string loc = std::to_string(location);
	printInstruction("la", "$" + loc + ", " + label, "const str to reg");

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

int SymbolTable::spillRegs(int expressionListSize) {
	int numSpilledRegs = 0;

	currentRegister -= expressionListSize;
	if(currentRegister < 8 )
		currentRegister = 8;

	if(currentRegister > 8) {
		while(currentRegister > 8) {
			numSpilledRegs++;
			currentRegister--;

			std::string reg = std::to_string(currentRegister);
			std::string offset = std::to_string(numSpilledRegs*4);

			printInstruction("sw", "$" + reg + ", -" + offset + "($sp)");
		}
		printInstruction("addi", "$sp, $sp, -" + std::to_string(numSpilledRegs*4));
	}

	return numSpilledRegs;
}

void SymbolTable::unspillRegs(int regsSpilled) {
	printInstruction("addi", "$sp, $sp, " + std::to_string(regsSpilled*4));

	while(regsSpilled) {
		printInstruction("lw", "$" + std::to_string(currentRegister) + ", -" + std::to_string(regsSpilled*4) + "($sp)");
		regsSpilled--;
		currentRegister++;
	}
}

void SymbolTable::setUpArgs(std::deque<std::pair<std::string, Type*> > signature, std::deque<Expression*> expressionList) {
	int offset = 4;
	int numArgs = signature.size()*4 + 4;

	for(int i = signature.size() -1; i >= 0; i--) {
		std::pair<std::string, Type*> parameter = signature[i];
		Expression* argument = expressionList[i];

		if(argument == NULL) {
			error("Argument " + std::to_string(i) + " is a null pointer. Talk to your compiler writer.");
		}

		if(parameter.second != argument->type) {
			error("Argument " + std::to_string(i) + " is the wrong type.");
		}

		offset += 4;

		// if(argument->type->isArray || argument->type->isRecord) {
		// 	printInstruction("la", "$" + argument->location + ", ($" + argument->location + ")", "get the address of the array or record.");
		// } 

		printInstruction("sw", "$" + argument->location + ", -" + std::to_string(offset) + "($sp)");
	}
}

void SymbolTable::procedure_call(std::string identifier, std::deque<Expression*>* expressionList) {
	Symbol* symbol = lookup(identifier);
	if(symbol == NULL) {
		error("this procedure is not in the symbol table.");
	}

	FunctionProcedure* procedure = dynamic_cast<FunctionProcedure*>(symbol);

	if(procedure->returnType)
		error("This identifier is a function. I'm expecting a procedure.");

	int spilledRegs = 0;

	if(expressionList != NULL) {
		if(expressionList->size() != procedure->signature.size()) {
			error("The argument list for procedure (" + procedure->name + ") is a different size than the parameter list.");
		}

		spilledRegs = spillRegs(expressionList->size());

		setUpArgs(procedure->signature, *expressionList);
	}

	printInstruction("jal", identifier, "procedure call.");

	if(spilledRegs) {
		unspillRegs(spilledRegs);
	}
}

Expression* SymbolTable::function_call(std::string identifier, std::deque<Expression*>* expressionList) {
	Symbol* symbol = lookup(identifier);
	if(symbol == NULL) {
		error("this function is not in the symbol table.");
	}

	FunctionProcedure* function = dynamic_cast<FunctionProcedure*>(symbol);

	if(function->returnType == NULL)
		error("This identifier is a procedure not a function.");

	int spilledRegs = 0;

	if(expressionList != NULL) {
		if(expressionList->size() != function->signature.size()) {
			error("The argument list for function (" + function->name + ") is a different size than the parameter list.");
		}

		spilledRegs = spillRegs(expressionList->size());

		setUpArgs(function->signature, *expressionList);
	}

	printInstruction("jal", identifier, "function call.");

	if(spilledRegs) {
		unspillRegs(spilledRegs);
	}

	Expression* expression = new Expression(getRegister());
	expression->type = function->returnType;

	printInstruction("move", "$" + expression->location + ", $v0", "move return value to new register.");
	
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

	recordItem->push_front(thePair);

	return recordItem;
}

void SymbolTable::error(std::string e) {
	std::cout << "Compile error line(" << lineNumber << "): " << e << std::endl;
	exit(1);
}


void SymbolTable::assignment(LValue* lvalue, Expression* expression) {
	if(lvalue == NULL || expression == NULL) {
		error("assignment got a null");
	}

	printInstruction("sw", "$" + expression->location + ", " + lvalue->variable->getFullLocation(), "assignment statement.");
}

////////////////////////// Blocks //////////////////////////////////////////

int SymbolTable::blockOffset = 0;

void SymbolTable::beginBlock() {

	blockOffset = currentOffset + 4;
	
	FunctionProcedure* functionProcedure = NULL;
	if(getInstance()->functionProcedureStack.size() > 0) {
		functionProcedure = getInstance()->functionProcedureStack.back();

		// for(std::pair<std::string, Type*> pair : functionProcedure->signature) {
		// 	blockOffset += pair.second->size; 
		// }

		blockOffset += functionProcedure->signature.size()*4;
	}

	printInstruction("addi", "$sp, $sp, -" + std::to_string(blockOffset));

	if(functionProcedure) {
		int aReg = 0;

		for(std::pair<std::string, Type*> pair : functionProcedure->signature) {
			Variable* var = initParameter(pair.first, pair.second);
		}
	}

	printInstruction("sw", "$ra, " + std::to_string(blockOffset-4) + "($sp)");
}

void SymbolTable::endBlock() {
	std::ostream& outFile = getInstance()->getFileStream();

	if(getInstance()->functionProcedureStack.size() > 0) {
		FunctionProcedure* functionProcedure = getInstance()->functionProcedureStack.back();

		getInstance()->functionProcedureStack.pop_back();

		printLabel(functionProcedure->name + "_epilog");
		printInstruction("lw", "$ra, " + std::to_string(blockOffset-4) + "($sp)");
		printInstruction("addi", "$sp, $sp, " + std::to_string(blockOffset));
		printInstruction("jr", "$ra");

		outFile << "# ******** " + functionProcedure->name + " functionProcedure ********" << std::endl
				<< std::endl;
	} 
	else
	{
		outFile << "# ******** end main function ********" << std::endl
				<< std::endl;
	}

	currentOffset = 0;
	blockOffset = 0;
}

////////////////////////////////////////////////////////////////////////////

////////////////////////// Lvalue //////////////////////////////////////////

std::deque<LValue*>* SymbolTable::makeLValueList(LValue* lvalue, std::deque<LValue*>* lvalues) {
	if(lvalues == NULL) {
		lvalues = new std::deque<LValue*>;
	}

	lvalues->push_front(lvalue);

	return lvalues;
}

LValue* SymbolTable::makeLValue(std::string identifier) {
	return new LValue(identifier);
}


LValue* SymbolTable::makeRecordLValue(LValue* lvalue, std::string identifier) {
	if(lvalue == NULL)
		error("makeRecordLValue got a null lvalue.");
	if(lvalue->variable == NULL)
		error("This lvalue is not a record variable.");
	if(lvalue->variable->type == NULL)
		error("This variable doesn't have a type.");
	if(lvalue->variable->type->isRecord == false) 
		error("You can't use the dot symbol on a non record type.");
	
	Record* record = dynamic_cast<Record*>(lvalue->variable->type);

	if(record->recordMap.find(identifier) == record->recordMap.end()) 
		error("The record doesn't have the identifier " + identifier + " in it.");

	std::pair<Type*, int> recordItem = record->recordMap[identifier];

	int offset = lvalue->variable->getOffset() + recordItem.second;
	std::string pointer = lvalue->variable->getPointer();

	if(lvalue->variable->getPointer() == "$sp"){
		Expression* expression = NULL;
		if(lvalue->variable->isParameter) {
			expression = load(lvalue->variable);
		} else {
			expression = loadAddress(lvalue->variable);
		}

		offset = recordItem.second;
		pointer = "$" + expression->location;
	}

	Variable* variable = new Variable(identifier, recordItem.first, offset, pointer);

	return new LValue(recordItem.first, variable);
}

LValue* SymbolTable::makeArrayLValue(LValue* lvalue, Expression* expression) {
	if(lvalue == NULL)
		error("makeRecordLValue got a null lvalue.");
	if(lvalue->variable == NULL)
		error("This lvalue is not a array variable.");
	if(lvalue->variable->type == NULL)
		error("This variable doesn't have a type.");
	if(lvalue->variable->type->isArray == false) 
		error("You can't use the brackets on a non array type.");

	Array* array = dynamic_cast<Array*>(lvalue->variable->type);
	LValue* returnLvalue = NULL;

	// int reg = getRegister();
	// int tempReg = getRegister();
	// int tempReg2 = getRegister();

	// //get the address
	// if(isLocalVar()) {
	// 	printInstruction("lw", "$" + std::to_string(reg) + ", " + lvalue->variable->getFullLocation(), "getting array offset.");
	// 	printInstruction("la", "$" + std::to_string(reg) + ", ($" + std::to_string(reg) + ")", "getting array offset.");
	// } else {
	// 	printInstruction("la", "$" + std::to_string(reg) + ", " + lvalue->variable->getFullLocation(), "getting array offset.");
	// }
	
	// int offset = array->type->size;	
	// //add the offset
	// printInstruction("li", "$" + std::to_string(tempReg) + ", " + std::to_string(offset));
	// printInstruction("mult", "$" + expression->location + ", $" + std::to_string(tempReg));
	// printInstruction("mflo", "$" + std::to_string(tempReg2));
	// printInstruction("add", "$" + std::to_string(reg) + ", $" + std::to_string(reg) + ", $" + std::to_string(tempReg2), "end getting array offset.");

	// // tempReg is temp
	// currentRegister-=2;

	// Variable* variable = new Variable("Element of " + array->name, array->type, 0, "$" + std::to_string(reg));
	// LValue* item = new LValue(array->type, variable);

	return returnLvalue;
}

////////////////////////////////////////////////////////////////////////////

////////////////////////// Strings //////////////////////////////////////

std::string SymbolTable::addStringConstant(std::string value) {
	std::string label = getStringConstantLabel();
	getInstance()->stringConstants[label] = value;
	return label;
}

void SymbolTable::printStringConstants() {
	printComment("This is the string area:");

	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\t.data" << std::endl;

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
			<< "\t.globl main" << std::endl;

	printLabel("main");
	printInstruction("b", "begin", "jump to main.");
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
	std::string loc = std::to_string(location);
	printInstruction("li", 		"$v0, 1", 		"int to console");
	printInstruction("move", 	"$a0, $" + loc);
	printInstruction("syscall", "", 			"end int to console.");
}

void SymbolTable::writeCharacter(int location) {
	std::string loc = std::to_string(location);
	printInstruction("li", 		"$v0, 11", 		"char to console stuff");
	printInstruction("move", 	"$a0, $" + loc);
	printInstruction("syscall", "", 			"End char to console.");
}

void SymbolTable::writeString(int location) {
	std::string loc = std::to_string(location);
	printInstruction("li", 		"$v0, 4", 		"str to console stuff");
	printInstruction("move", 	"$a0, $" + loc, " ");
	printInstruction("syscall", "", 			"End str to console.");
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

	printInstruction("li", "$v0, 5", "read int");
	printInstruction("syscall", "");
	printInstruction("sw", "$v0, " + variable->getFullLocation());
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

	printInstruction("li", "$v0, 12", "read char");
	printInstruction("syscall", " ");
	printInstruction("sw", "$v0, " + variable->getFullLocation(), "char read");
}

////////////////////////////////////////////////////////////////////////////

///////////////////////////// Expressions //////////////////////////////////


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

Expression* SymbolTable::loadAddress(Variable* var) {
	int resultLocation = getRegister();

	std::string result = std::to_string(resultLocation);

	printInstruction("la" ,"$" + result + ", " + var->getFullLocation());
	
	return new Expression(resultLocation);
}

Expression* SymbolTable::load(Variable* var) {
	int resultLocation = getRegister();

	std::string result = std::to_string(resultLocation);

	printInstruction("lw" ,"$" + result + ", " + var->getFullLocation());

	return new Expression(resultLocation);
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

	printInstruction("sw", "$" + expression->location + ", " + variable->getFullLocation());
}

void SymbolTable::store(Variable* variable, std::string reg) {
	if(variable == NULL) {
		error("store got a null.");
	}

	printInstruction("sw", "$" + reg + ", " + variable->getFullLocation());
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

	Expression* counter = load(forVar.var);
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

	Expression* expression = load(forVar.var);
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
	While whileVar;

	getInstance()->whileStack.push_back(whileVar);

	printLabel(whileVar.whileLabel);
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
	printInstruction("li", "$v0, 10", "stop");
	printInstruction("syscall", "");
}

void SymbolTable::printNewLine() {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << std::endl;
}

void SymbolTable::printComment(std::string comment) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\t" << "# " << comment << std::endl;
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

	outFile << "\t" << std::left << std::setw(8) << instruction << std::setw(15) << registers << "# " << comment << std::endl;
}

void SymbolTable::printInstruction(std::string instruction, std::string registers) {
	std::ofstream& outFile = getInstance()->getFileStream();

	outFile << "\t" << std::left << std::setw(8) << instruction << std::setw(15) << registers << std::endl;
}

//// End SymbolTable ////