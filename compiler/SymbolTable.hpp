#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include "Symbol.hpp"

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
	static int currentOffset;

public:
	static SymbolTable* getInstance();
/* End Singleton Stuff */

public:
	std::vector<Table> symbolTable; 

	static void add(std::string identifier, Symbol* symbol);
	static Symbol* lookup(std::string name);
	static void pop();
	static Table initializedMainTable();
	static void addVar(std::vector<std::string>* identList, Type* type);
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
	static Expression* lValueToExpression(std::string identifier);
	static Expression* integerConstToExpression(int value);
	static Constant* lookupConstant(std::string identifier);
	static Expression* expression(Expression* left, std::string op, Expression* right);
	static Expression* expression(std::string, Expression* right);
	static Expression* expressionLvalue(Expression* symbol);
	static Expression* function_call(std::string identifier);
	static Expression* function_call(std::string identifier, std::vector<Expression*>*);
	static Expression* chr(Expression* symbol);
	static Expression* ord(Expression* symbol);
	static Expression* pred(Expression* symbol);
	static Expression* succ(Expression* symbol);
	static std::vector<Expression*>* makeExpressionList(Expression* expression, std::vector<Expression*>* expressions);
	static std::vector<Symbol*>* makeSymbolVector(Symbol* symbol, std::vector<Symbol*>* symbols);
	static std::vector<Type*>* makeTypeList(std::vector<Symbol*>* identifiers, Type* type, std::vector<Type*>* symbols);
	static std::vector<std::string>* makeIdentList(std::string identifier, std::vector<std::string>* identList);
	static std::vector<std::pair<std::vector<std::string>, Type*> >* makeRecordItem(std::vector<std::string>* identList, Type* type, std::vector<std::pair<std::vector<std::string>, Type*> >* recordItem);

	///////////////////////// Initialize Assembly //////////////////////////////

	static void initAssembly();

	////////////////////////////////////////////////////////////////////////////

	//////////////////////////// Read and Write ////////////////////////////////

	static void write(std::vector<Expression*>* expressionList);
	static void writeInteger(int location);
	static void writeCharacter(int location);
	static void writeString(std::string label);
	static Expression* read(Expression* expression);
	static Expression* readInteger(int location);

	////////////////////////////////////////////////////////////////////////////

	///////////////////////////// Expressoins //////////////////////////////////

	static int currentRegister;

	static int getRegister();
	static int lookup(Expression* expression);
	static std::pair<int,int> lookupExpression(std::string name);
	static Expression* eval(Expression* left, Expression* right, std::string operation);
	static Expression* load(int location);
	static Expression* load(std::string name);
	static Expression* loadImmediateInt(int value);
	static Expression* loadImmediateChar(std::string value);

	////////////////////////////////////////////////////////////////////////////
};

#endif