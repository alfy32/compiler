#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <fstream>
#include <deque>
#include <exception>
#include <iomanip>

#include "Symbol.hpp"

extern int yyerror(const char*);

class Expression {
public:
	Type* type;
	std::string location;
	Constant* constant;

	Expression(int location);
	Expression(Constant*, Type* type);
	int getLocation();
};

class LValue {
public:
	Type* type;
	Variable* variable;
	Constant* constant;

	LValue(std::string identifier);
};

class For {
	static int labelCount;
public:
	Variable* var;
	std::string forLabel;
	std::string endLabel;
	std::string to;

	For(Variable* variable) {
		var = variable;
		forLabel = "FOR" + std::to_string(labelCount);
		endLabel = "END_FOR" + std::to_string(labelCount);
		labelCount++;
	}
};

class While {
	static int labelCount;
public:
	std::string whileLabel;
	std::string endLabel;

	While() {
		whileLabel = "WHILE" + std::to_string(labelCount);
		endLabel = "END_WHILE" + std::to_string(labelCount);
		labelCount++;
	}
};

class Repeat {
	static int labelCount;
public:
	std::string repeatLabel;
	std::string endLabel;

	Repeat() {
		repeatLabel = "REPEAT" + std::to_string(labelCount);
		endLabel = "END_REPEAT" + std::to_string(labelCount);
		labelCount++;
	}
};

class Table {
public: 
	std::map<std::string, Symbol*> tableMap;


	Table();

	void add(std::string identifier, Symbol* symbol);
	Symbol* lookup(std::string name);
	virtual void print();
};

class SymbolTable {
/* Singleton Stuff */
private:
	static SymbolTable* symbolTableInstance;

public:
	static SymbolTable* getInstance();
/* End Singleton Stuff */

/* Assembly file stuff */
private:
	std::ofstream outputFile;
	std::ofstream verboseFile;
public:
	std::string asmFileName;
	void openFile();
	std::ofstream& getFileStream();
	std::ostream& getErrorStream();
	std::ostream& getVerboseStream();
/* End Assembly file stuff */

public:
	std::deque<Table> symbolTable; 
	std::map<std::string, std::string> stringConstants;

	SymbolTable();

	static int currentOffset;
	static int globalOffset;
	static int currentConstString;

	static void add(std::string identifier, Symbol* symbol);
	static Symbol* lookup(std::string name);
	static void pop();
	static Table initializedMainTable();
	static bool isLocalVar();
	static void addVar(std::deque<std::string>* identList, Type* type);
	static Variable* addVar(std::string identifier, Type* type);
	static void constDecl(std::string identifier, Constant* constExpression);
	static void typeDecl(std::string identifier, Type* type);
	static void funcProcDecl(std::string identifier, FunctionProcedure*);
	static void funcProcEnd(FunctionProcedure*, bool);
	static void returnStatement(Expression*);
	static void addNewScope();
	static Constant* evalConstant(Constant* left, std::string oper, Constant* right);
	static Constant* evalConstant(std::string oper, Constant* right);
	static Constant* evalIntConstant(Constant* left, std::string oper, Constant* right);
	static Constant* evalCharConstant(Constant* left, std::string oper, Constant* right);
	static Constant* evalStrConstant(Constant* left, std::string oper, Constant* right);
	
	static Expression* lValueToExpression(LValue*);
	static Expression* integerConstToExpression(int value);
	static Expression* charConstToExpression(std::string value);
	static Expression* stringConstToExpression(std::string value);
	static Constant* lookupConstant(std::string identifier);
	static Expression* expression(Expression* left, std::string op, Expression* right);
	static Expression* expression(std::string, Expression* right);
	static int spillRegs(int);
	static void unspillRegs(int);
	static void setUpArgs(std::deque<std::pair<std::string, Type*> > signature, std::deque<Expression*> expressionList);
	static Expression* function_call(std::string identifier, std::deque<Expression*>*);
	static void procedure_call(std::string identifier, std::deque<Expression*>*);
	static Expression* chr(Expression* symbol);
	static Expression* ord(Expression* symbol);
	static Expression* pred(Expression* symbol);
	static Expression* succ(Expression* symbol);
	static std::deque<Expression*>* makeExpressionList(Expression* expression, std::deque<Expression*>* expressions);
	static std::deque<Variable*>* makeVariableList(Variable*, std::deque<Variable*>*);
	static std::deque<Symbol*>* makeSymboldeque(Symbol* symbol, std::deque<Symbol*>* symbols);
	static std::deque<Type*>* makeTypeList(std::deque<Symbol*>* identifiers, Type* type, std::deque<Type*>* symbols);
	static std::deque<std::string>* makeIdentList(std::string identifier, std::deque<std::string>* identList);
	static std::deque<std::pair<std::deque<std::string>, Type*> >* makeRecordItem(std::deque<std::string>* identList, Type* type, std::deque<std::pair<std::deque<std::string>, Type*> >* recordItem);

	static void error(std::string);

	static void assignment(LValue*, Expression*);

	////////////////////////// Blocks //////////////////////////////////////////

	static int blockOffset;

	std::deque<FunctionProcedure*> functionProcedureStack;

	static void beginBlock();
	static void endBlock();

	////////////////////////////////////////////////////////////////////////////	

	////////////////////////// Lvalue //////////////////////////////////////////

	static LValue* makeLValue(std::string identifier);
	static std::deque<LValue*>* makeLValueList(LValue*, std::deque<LValue*>*);

	////////////////////////////////////////////////////////////////////////////

	////////////////////////// Strings //////////////////////////////////////

	static std::string addStringConstant(std::string);
	static void printStringConstants();
	static std::string getStringConstantLabel();

	////////////////////////////////////////////////////////////////////////////

	////////////////////////// Statements //////////////////////////////////////

	static void endStatement();

	////////////////////////////////////////////////////////////////////////////

	///////////////////////// Initialize Assembly //////////////////////////////

	static void initAssembly();
	static void startMain();
	static void endMain();

	////////////////////////////////////////////////////////////////////////////

	//////////////////////////// Read and Write ////////////////////////////////

	static void write(std::deque<Expression*>* expressionList);
	static void writeInteger(int location);
	static void writeCharacter(int location);
	static void writeString(int location);
	static void read(std::deque<LValue*>*);
	static void readInteger(Variable* variable);
	static void readString();
	static void readCharacter(Variable* variable);

	////////////////////////////////////////////////////////////////////////////

	///////////////////////////// Expressions //////////////////////////////////

	static int currentRegister;

	static int getRegister();
	static int lookup(Expression* expression);
	static std::pair<int,int> lookupExpression(std::string name);
	static Expression* eval(Expression* left, Expression* right, std::string operation);
	static Expression* evalMult(Expression* left, Expression* right);
	static Expression* evalDiv(Expression* left, Expression* right);
	static Expression* evalMod(Expression* left, Expression* right);
	static Expression* load(Variable* var);
	static Expression* load(int location);
	static Expression* load(std::string name);
	static Expression* loadImmediateInt(int value);
	static Expression* loadImmediateChar(std::string value);
	static void store(Variable*, Expression*);
	static void store(Variable*, std::string reg);

	////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// If Statements //////////////////////////////

	std::deque<std::string> elseStack;
	std::deque<std::string> endStack;

	static int elseCount;
	static int endCount;

	static void pushElse();
	static void pushEnd();
	static void popElse();
	static void popEnd();
	static std::string elseFront();
	static std::string endFront();

	static void afterIf();
	static void ifStatement(Expression*);
	static void thenStatement();
	static void elseIfStatement(Expression*);
	static void elseStatement();

	////////////////////////////////////////////////////////////////////////////

	/////////////////////////////// For Statements /////////////////////////////

	std::deque<For> forStack;

	static void initFor(std::string identifier, Expression*);
	static void forLabel(std::string);
	static void forEval(Expression*);
	static void forEnd();

	////////////////////////////////////////////////////////////////////////////

	///////////////////////////// While Statements /////////////////////////////

	std::deque<While> whileStack;

	static void whileInit();
	static void whileBranch(Expression*);
	static void whileRepeat();

	////////////////////////////////////////////////////////////////////////////

	//////////////////////////// Repeat Statements /////////////////////////////

	std::deque<Repeat> repeatStack;

	static void repeatInit();
	static void repeatEnd(Expression*);
	// static void whileRepeat();

	////////////////////////////////////////////////////////////////////////////

	static void stop();

	static void printNewLine();
	static void printComment(std::string comment);
	static void printLabel(std::string label, std::string comment);
	static void printLabel(std::string label);
	static void printInstruction(std::string instruction, std::string registers, std::string comment);
	static void printInstruction(std::string instruction, std::string registers);
};

#endif