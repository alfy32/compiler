#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <fstream>
#include <deque>

#include "Symbol.hpp"

extern int yyerror(const char*);

class Expression {
public:
	Type* type;
	int location;

	Expression(int location);
	int getLocation();
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
public:
	void openFile();
	std::ofstream& getFileStream();
	std::ostream& getErrorStream();
/* End Assembly file stuff */

public:
	std::deque<Table> symbolTable; 
	std::map<std::string, std::string> stringConstants;

	SymbolTable();

	static int currentOffset;
	static int currentConstString;

	static void add(std::string identifier, Symbol* symbol);
	static Symbol* lookup(std::string name);
	static void pop();
	static Table initializedMainTable();
	static void addVar(std::deque<std::string>* identList, Type* type);
	static void constDecl(std::string identifier, Constant* constExpression);
	static void typeDecl(std::string identifier, Type* type);
	static void funcDecl(std::string identifier, Func* func);
	static void addNewScope(Func* func);
	static void procDecl(std::string identifier, Proc* proc);
	static void addNewScope(Proc* proc);
	static void forStatement(std::string identifier, Expression* initalValue, Expression* expr, std::string UpToOrDownTo);
	static Constant* evalConstant(Constant* left, std::string oper, Constant* right);
	static Constant* evalConstant(std::string oper, Constant* right);
	static Constant* evalIntConstant(Constant* left, std::string oper, Constant* right);
	static Constant* evalCharConstant(Constant* left, std::string oper, Constant* right);
	static Constant* evalStrConstant(Constant* left, std::string oper, Constant* right);
	static Variable* makeLvalue(std::string identifier);
	static Expression* lValueToExpression(Variable*);
	static Expression* integerConstToExpression(int value);
	static Expression* charConstToExpression(std::string value);
	static Expression* stringConstToExpression(std::string value);
	static Constant* lookupConstant(std::string identifier);
	static Expression* expression(Expression* left, std::string op, Expression* right);
	static Expression* expression(std::string, Expression* right);
	static Expression* function_call(std::string identifier);
	static Expression* function_call(std::string identifier, std::deque<Expression*>*);
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

	////////////////////////////////////////////////////////////////////////////

	//////////////////////////// Read and Write ////////////////////////////////

	static void write(std::deque<Expression*>* expressionList);
	static void writeInteger(int location);
	static void writeCharacter(int location);
	static void writeString(int location);
	static void read(std::deque<Variable*>*);
	static void readInteger(Variable* variable);
	static void readString();
	static void readCharacter(Variable* variable);

	////////////////////////////////////////////////////////////////////////////

	///////////////////////////// Expressoins //////////////////////////////////

	static int currentRegister;

	static int getRegister();
	static int lookup(Expression* expression);
	static std::pair<int,int> lookupExpression(std::string name);
	static Expression* eval(Expression* left, Expression* right, std::string operation);
	static Expression* evalMult(Expression* left, Expression* right);
	static Expression* evalDiv(Expression* left, Expression* right);
	static Expression* evalMod(Expression* left, Expression* right);
	static Expression* load(int location);
	static Expression* load(std::string name);
	static Expression* loadImmediateInt(int value);
	static Expression* loadImmediateChar(std::string value);

	////////////////////////////////////////////////////////////////////////////
};

#endif