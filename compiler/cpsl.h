
#define YY_NO_UNPUT

#include <iostream>
#include <string>

#include "symbol/Symbol.hpp"

enum OPERATOR { 
	OP_NEG, 
	OP_MULTIPLY, 	OP_DIVIDE, 		OP_MOD,
	OP_ADD, 		OP_SUBTRACT,
	OP_EQUAL,   	OP_NOT_EQUAL, 
	OP_LT,      	OP_LT_EQ,
	OP_GT,      	OP_GT_EQ,
	OP_TILDE,
	OP_AND,
	OP_OR
};