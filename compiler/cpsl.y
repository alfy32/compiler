
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
 
%token  NEG_SYM
        MULTIPLY_SYM
        DIVIDE_SYM
        MOD_SYM
        SUBTRACT_SYM
        ADD_SYM
        EQUAL_SYM
        LT_GT_SYM
        LT_SYM
        LT_EQ_SYM
        GT_SYM
        GT_EQ_SYM
        TILDE_SYM
        AND_SYM
        OR_SYM
        COLON_SYM
        SEMICOLON_SYM
        L_PAREN_SYM
        R_PAREN_SYM
        L_BRACKET_SYM
        R_BRACKET_SYM
        ASSIGNMENT_SYM
        COMMA_SYM
        DOT_SYM

        CONST_SYM
        PROCEDURE_SYM
        FUNCTION_SYM
        IDENT_SYM
        BEGIN_SYM
        END_SYM
        IF_SYM
        ELSE_SYM
        ELSEIF_SYM
        WHILE_SYM
        DO_SYM
        FOR_SYM
        RETURN_SYM
        READ_SYM 
        ARRAY_SYM
        INT_CONST_SYM
        CHAR_CONST_SYM
        STR_CONST_SYM
        CHAR_SYM
        FORWARD_SYM
        OF_SYM
        
        ORD_SYM
        PRED_SYM
        RECORD_SYM
        REPEAT_SYM
        STOP_SYM
        SUCC_SYM
        THEN_SYM
        TO_SYM
        UNTIL_SYM
        VAR_SYM
        WRITE_SYM

%right          NEG_SYM
%left           MULTIPLY_SYM DIVIDE_SYM MOD_SYM
%left           SUBTRACT_SYM ADD_SYM
%nonassoc       EQUAL_SYM LT_GT_SYM LT_SYM LT_EQ_SYM GT_SYM GT_EQ_SYM
%right          TILDE_SYM
%left           AND_SYM
%left           OR_SYM

%%

Program:        SubCDecl SubTDecl SubVDecl SubPFDecl Block 
                ;

SubCDecl:       ConstantDecl
                | /* nothing */
                ;

SubTDecl:       TypeDecl
                | /* nothing */
                ;

SubVDecl:       VarDecl
                | /* nothing */
                ;

SubPFDecl:      SubSubPFDecl
                | SubSubPFDecl SubPFDecl
                ;

SubSubPFDecl:   ProcedureDecl
                | FunctionDecl
                | /* nothing */
                ;

/* 3.1.1 Constant Declarations */

ConstantDecl:   CONST_SYM  IDENT_SYM  EQUAL_SYM ConstExpression SEMICOLON_SYM ConstantDecl
                | CONST_SYM  IDENT_SYM  EQUAL_SYM ConstExpression SEMICOLON_SYM 
                ;

/* 3.1.2 Procedure and Function Declarations */

ProcedureDecl:  PROCEDURE_SYM  IDENT_SYM  L_PAREN_SYM FormalParameters R_PAREN_SYM SEMICOLON_SYM FORWARD_SYM ;
                | PROCEDURE_SYM  IDENT_SYM  L_PAREN_SYM FormalParameters R_PAREN_SYM SEMICOLON_SYM Body SEMICOLON_SYM
                ;

FunctionDecl:   FUNCTION_SYM  IDENT_SYM  L_PAREN_SYM FormalParameters R_PAREN_SYM COLON_SYM Type SEMICOLON_SYM FORWARD_SYM SEMICOLON_SYM
                | FUNCTION_SYM  IDENT_SYM  L_PAREN_SYM FormalParameters R_PAREN_SYM COLON_SYM Type SEMICOLON_SYM Body SEMICOLON_SYM
                ;

FormalParameters: /* nothing */
                | FormalParameter
                | FormalParameter SEMICOLON_SYM FormalParameter
                ;

FormalParameter: VAR_SYM IdentList COLON_SYM Type
                | IdentList COLON_SYM Type
                ;

Body:           SubCDecl SubTDecl SubVDecl Block
                ;

Block:          BEGIN_SYM StatementSequence END_SYM 
                ;

/* 3.1.3 Type Declarations */

TypeDecl:       Type IDENT_SYM  EQUAL_SYM Type SEMICOLON_SYM
                | Type IDENT_SYM  EQUAL_SYM Type SEMICOLON_SYM TypeDecl
                ;

Type:           SimpleType
                | RecordType
                | ArrayType
                ;

SimpleType:     IDENT_SYM 
                ;

RecordType:     RECORD_SYM RecordItem END_SYM 
                ;

RecordItem:     IdentList COLON_SYM Type SEMICOLON_SYM
                | IdentList COLON_SYM Type SEMICOLON_SYM RecordItem
                | /* nothing */
                ;

ArrayType:      ARRAY_SYM  L_BRACKET_SYM ConstExpression COLON_SYM ConstExpression R_BRACKET_SYM OF_SYM Type
                ;

IdentList:      IDENT_SYM 
                | IDENT_SYM  IdentList
                ;

/* 3.1.4 Variable Declarations */

VarDecl:        VAR_SYM SubVerDecl
                ;

SubVerDecl:     IdentList COLON_SYM Type SEMICOLON_SYM
                | IdentList COLON_SYM Type SEMICOLON_SYM SubVerDecl
                ;

/* 3.2 CPSL Statements */

StatementSequence: Statement
                | Statement SEMICOLON_SYM Statement
                ;

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
                ;

Assignemnt:     LValue ASSIGNMENT_SYM Expression 
                ;

IfStatement:    IF_SYM Expression THEN_SYM StatementSequence SubElseIf SubElse END_SYM
                ;

SubElseIf:      ELSEIF_SYM  Expression THEN_SYM StatementSequence
                | ELSEIF_SYM  Expression THEN_SYM StatementSequence SubElseIf
                | /* nothing */
                ;

SubElse:        ELSE_SYM StatementSequence
                | /* nothing */
                ;

WhileStatement: WHILE_SYM  Expression DO_SYM  StatementSequence END_SYM 
                ;

RepeatStatement: REPEAT_SYM StatementSequence UNTIL_SYM Expression
                ;

ForStatement:   FOR_SYM  IDENT_SYM  ASSIGNMENT_SYM Expression TO_SYM Expression DO_SYM  StatementSequence END_SYM 
                ;

StopStatement:  STOP_SYM
                ;

ReturnStatement: RETURN_SYM  
                | RETURN_SYM  Expression
                ;

ReadStatement:  READ_SYM  L_PAREN_SYM LValStuff R_PAREN_SYM
                ;

LValStuff:      LValue
                | LValue COMMA_SYM LValStuff
                ;

WriteStatement: WRITE_SYM L_PAREN_SYM ExpressionList R_PAREN_SYM
                ;

ExpressionList: Expression
                | Expression COMMA_SYM ExpressionList
                ;

ProcedureCall: IDENT_SYM  L_PAREN_SYM R_PAREN_SYM
                | IDENT_SYM  L_PAREN_SYM ExpressionList R_PAREN_SYM
                ;

NullStatement:  /* nothing */
                ;

/* 3.3 Expressions */

Expression:      Expression OR_SYM Expression
                | Expression AND_SYM Expression
                | Expression EQUAL_SYM Expression
                | Expression LT_GT_SYM Expression
                | Expression LT_EQ_SYM Expression
                | Expression GT_EQ_SYM Expression
                | Expression LT_SYM Expression
                | Expression GT_SYM Expression
                | Expression ADD_SYM Expression
                | Expression SUBTRACT_SYM Expression
                | Expression MULTIPLY_SYM Expression
                | Expression DIVIDE_SYM Expression
                | Expression MOD_SYM Expression
                | TILDE_SYM Expression
                | NEG_SYM Expression
                | L_PAREN_SYM Expression R_PAREN_SYM
                | IDENT_SYM  L_PAREN_SYM R_PAREN_SYM
                | IDENT_SYM  L_PAREN_SYM ExpressionList R_PAREN_SYM
                | CHAR_SYM L_PAREN_SYM Expression R_PAREN_SYM
                | ORD_SYM  L_PAREN_SYM Expression R_PAREN_SYM
                | PRED_SYM  L_PAREN_SYM Expression R_PAREN_SYM
                | SUCC_SYM L_PAREN_SYM Expression R_PAREN_SYM
                | LValue
                | ConstExpression
                ;

LValue:         IDENT_SYM 
                | IDENT_SYM  DOT_SYM IDENT_SYM 
                | IDENT_SYM  L_BRACKET_SYM Expression R_BRACKET_SYM
                ;

ConstExpression: ConstExpression OR_SYM ConstExpression
                | ConstExpression AND_SYM ConstExpression
                | ConstExpression EQUAL_SYM ConstExpression
                | ConstExpression LT_GT_SYM ConstExpression
                | ConstExpression LT_EQ_SYM ConstExpression
                | ConstExpression GT_EQ_SYM ConstExpression
                | ConstExpression LT_SYM ConstExpression
                | ConstExpression GT_SYM ConstExpression
                | ConstExpression ADD_SYM ConstExpression
                | ConstExpression SUBTRACT_SYM ConstExpression
                | ConstExpression MULTIPLY_SYM ConstExpression
                | ConstExpression DIVIDE_SYM ConstExpression
                | ConstExpression MOD_SYM ConstExpression
                | TILDE_SYM ConstExpression
                | NEG_SYM ConstExpression
                | L_PAREN_SYM ConstExpression R_PAREN_SYM
                | INT_CONST_SYM
                | CHAR_CONST_SYM
                | STR_CONST_SYM
                | IDENT_SYM 
                ;

%%