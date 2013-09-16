
%{
#define YYSTYPE double
#include <math.h>
%}

/* BISON Declarations */
%start Program
%union {
        int intVal;
        char* charVal;
}
%token NUM
%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation        */

/* Grammar follows */
%%

Program:	CDecl TDecl VDecl PFDecl Block

CDecl:          ConstantDecl
                | /* nothing */

TDecl:          TypeDecl
                | /* nothing */

VDecl:          VarDecl
                | /* nothing */

PFDecl:         PFDecl PFDecl
                | ProcedureDecl
                | FunctionDecl
                | /* nothing */



ConstantDecl:   const <ident> = ConstExpression ; ConstantDecl
		| const <ident> = ConstExpression ; 



ProcedureDecl: 	procedure <ident> ( FormalParameters ) ; forward ;
		| procedure <ident> ( FormalParameters ) ; Body ;

FunctionDecl:	function <ident> ( FormalParameters ) : Type ; for-ward ;
		| function <ident> ( FormalParameters ) : Type ; Body ;

FormalParameters: /* nothing */
		| FormalParameter
                | FormalParameter ; FormalParameter

FormalParameter: var IdentList : Type
                | IdentList : Type

Body:           CDecl TDecl VDecl Block

Block:          begin StatementSequence end




TypeDecl:       type ident = Type ;
                | type ident = Type ; TypeDecl

Type:           SimpleType
                | RecordType
                | ArrayType

SimpleType:     ident

RecordType:     record RecordItem end

RecordItem:     /* nothing */
                | IdentList : Type ;
                | IdentList : Type ; RecordItem

ArrayType:      array [ ConstExpression : ConstExpression ] of Type

IdentList:      ident
                | ident IdentList



VarDecl:        var VarDeclStuff

VarDeclStuff:   IdentList : Type ;
                | IdentList : Type ; VarDeclStuff



StatementSequence: Statement
                | Statement ; Statement

Statement:      Assignemnt
                | IfStatement
                | WhileStatement
                | RepeatStatement
                | ForStatement
                | StopStatement
                | ReturnStatement
                | ReadStatement
                | WriteStatement
                | ProcedureCall
                | NullStatement

Assignemnt:     LValue := Expression 

IfStatement:    if Expression then StatementSequence ElseIfPart ElsePart end

ElseIfPart:     /* nothing */
                | elseif Expression then StatementSequence
                | ElseIfPart ElseIfPart

ElsePart:       /* nothing */
                | else StatementSequence

WhileStatement: while Expression do StatementSequence end

RepeatStatement: repeat StatementSequence until Expression

ForStatement:   for ident := Expression to Expression do StatementSequence end

StopStatement:  stop

ReturnStatement: return 
                | return Expression

ReadStatement:  read ( LValStuff )

LValStuff:      LValue
                | LValue , LValStuff

WriteStatement: write ( ExpressionList )

ExpressionList: Expression
                | Expression , ExpressionList

ProcedureCall: ident ( )
                | ident ( ExpressionList )

NullStatement:  /* nothing */



Expression:      Expression | Expression
                | Expression & Expression
                | Expression = Expression
                | Expression <> Expression
                | Expression <= Expression
                | Expression >= Expression
                | Expression < Expression
                | Expression > Expression
                | Expression + Expression
                | Expression - Expression
                | Expression * Expression
                | Expression / Expression
                | Expression % Expression
                | ~ Expression
                | - Expression
                | ( Expression )
                | ident ( )
                | ident ( ExpressionList )
                | chr ( Expression )
                | ord ( Expression )
                | pred ( Expression )
                | succ ( Expression )
                | LValue
                | ConstExpression

LValue:         ident
                | ident . ident
                | ident [ Expression ]

ConstExpression: ConstExpression | ConstExpression
                | ConstExpression & ConstExpression
                | ConstExpression = ConstExpression
                | ConstExpression <> ConstExpression
                | ConstExpression <= ConstExpression
                | ConstExpression >= ConstExpression
                | ConstExpression < ConstExpression
                | ConstExpression > ConstExpression
                | ConstExpression + ConstExpression
                | ConstExpression - ConstExpression
                | ConstExpression * ConstExpression
                | ConstExpression / ConstExpression
                | ConstExpression % ConstExpression
                | ~ ConstExpression
                | - ConstExpression
                | ( ConstExpression )
                | intconst
                | charconst
                | strconst
                | ident


%%