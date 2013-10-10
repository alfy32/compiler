
%{

#include "cpsl.h"

%}

%union {
    int int_val;
    char* str_val;

    void* op;
    void* token;
    void* keyword;

    Symbol* symbol_val;
    Const* const_val;
    Type* type_val;

    std::vector<Symbol*>* symbolVector_val;
    std::vector<Type*>* typeVector_val;
}

%start Program
 
%token <op> NEG_SYM
%token <op> MULTIPLY_SYM DIVIDE_SYM MOD_SYM
%token <op> ADD_SYM SUBTRACT_SYM
%token <op> EQUAL_SYM   NOT_EQUAL_SYM 
            LT_SYM      LT_EQ_SYM
            GT_SYM      GT_EQ_SYM
%token <op> TILDE_SYM
%token <op> AND_SYM
%token <op> OR_SYM

%token <token>      COLON_SYM
                    SEMICOLON_SYM
                    L_PAREN_SYM
                    R_PAREN_SYM
                    L_BRACKET_SYM
                    R_BRACKET_SYM
                    ASSIGNMENT_SYM
                    COMMA_SYM
                    DOT_SYM

%token <keyword>    ARRAY_SYM   BEGIN_SYM   CHR_SYM     CONST_SYM   DO_SYM          DOWNTO_SYM
                    ELSE_SYM    ELSEIF_SYM  END_SYM     FOR_SYM     FORWARD_SYM     FUNCTION_SYM
                    IF_SYM      OF_SYM      ORD_SYM     PRED_SYM    PROCEDURE_SYM   READ_SYM
                    RECORD_SYM  REPEAT_SYM  RETURN_SYM  STOP_SYM    SUCC_SYM        THEN_SYM
                    TO_SYM      TYPE_SYM    UNTIL_SYM   VAR_SYM     WHILE_SYM       WRITE_SYM

%token <str_val>    IDENT_SYM
%token <int_val>    INT_CONST_SYM
%token <str_val>    CHAR_CONST_SYM
%token <str_val>    STR_CONST_SYM

%type <const_val> ConstExpression
%type <symbol_val> Expression
%type <symbolVector_val> ExpressionList IdentList
%type <typeVector_val> RecordItem
%type <int_val> LValue
%type <type_val> Type SimpleType ArrayType RecordType

%right          NEG_SYM
%left           MULTIPLY_SYM DIVIDE_SYM MOD_SYM
%left           SUBTRACT_SYM ADD_SYM
%nonassoc       EQUAL_SYM NOT_EQUAL_SYM LT_SYM LT_EQ_SYM GT_SYM GT_EQ_SYM
%right          TILDE_SYM
%left           AND_SYM
%left           OR_SYM

%%

Program:        SubCDecl SubTDecl SubVDecl SubPFDecl Block DOT_SYM
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

SubPFDecl:      ProcedureDecl SubPFDecl
                | FunctionDecl SubPFDecl
                |
                ;

/* 3.1.1 Constant Declarations */

ConstantDecl:   CONST_SYM SubConstDecl
                ;

SubConstDecl:   IDENT_SYM EQUAL_SYM ConstExpression SEMICOLON_SYM                   { SymbolTable::constDecl($1, $3); }
                | SubConstDecl IDENT_SYM EQUAL_SYM ConstExpression SEMICOLON_SYM    { SymbolTable::constDecl($2, $4); }

/* 3.1.2 Procedure and Function Declarations */

ProcedureDecl:  PROCEDURE_SYM IDENT_SYM L_PAREN_SYM FormalParameters R_PAREN_SYM SEMICOLON_SYM FORWARD_SYM SEMICOLON_SYM    
                | PROCEDURE_SYM IDENT_SYM L_PAREN_SYM FormalParameters R_PAREN_SYM SEMICOLON_SYM Body SEMICOLON_SYM         
                ;

FunctionDecl:   FUNCTION_SYM IDENT_SYM L_PAREN_SYM FormalParameters R_PAREN_SYM COLON_SYM Type SEMICOLON_SYM FORWARD_SYM SEMICOLON_SYM
                | FUNCTION_SYM IDENT_SYM L_PAREN_SYM FormalParameters R_PAREN_SYM COLON_SYM Type SEMICOLON_SYM Body SEMICOLON_SYM     
                ;

FormalParameters: /* nothing */  
                | SubVar IdentList COLON_SYM Type  
                | SubVar IdentList COLON_SYM Type SEMICOLON_SYM FormalParameters 
                ;

SubVar:         VAR_SYM
                | /* nothing */
                ;

Body:           SubCDecl SubTDecl SubVDecl Block
                ;

Block:          BEGIN_SYM StatementSequence END_SYM 
                ;

/* 3.1.3 Type Declarations */

TypeDecl:       TYPE_SYM SubTypeDecl
                ;

SubTypeDecl:    IDENT_SYM EQUAL_SYM Type SEMICOLON_SYM                  { SymbolTable::typeDecl($1,$3); }            
                | IDENT_SYM EQUAL_SYM Type SEMICOLON_SYM SubTypeDecl    { SymbolTable::typeDecl($1,$3); }

Type:           SimpleType      { $$ = $1; }
                | RecordType    { $$ = $1; } 
                | ArrayType     { $$ = $1; } 
                ;

SimpleType:     IDENT_SYM       { $$ = new SimpleType($1); $$->print(); }
                ;

RecordType:     RECORD_SYM RecordItem END_SYM  { $$ = new Record($2); }
                ;

RecordItem:     IdentList COLON_SYM Type SEMICOLON_SYM                  { $$ = SymbolTable::makeTypeList($1, $3, NULL); }
                | IdentList COLON_SYM Type SEMICOLON_SYM RecordItem     { $$ = SymbolTable::makeTypeList($1, $3, $5); }
                ;

ArrayType:      ARRAY_SYM L_BRACKET_SYM ConstExpression COLON_SYM ConstExpression R_BRACKET_SYM OF_SYM Type { $$ = new Array($3, $5, $8); }
                ;

IdentList:      IDENT_SYM                           { $$ = SymbolTable::makeSymbolVector(new Symbol($1), NULL); }
                | IDENT_SYM COMMA_SYM IdentList     { $$ = SymbolTable::makeSymbolVector(new Symbol($1), $3); }
                ;

/* 3.1.4 Variable Declarations */

VarDecl:        VAR_SYM SubVarDecl
                ;

SubVarDecl:     IdentList COLON_SYM Type SEMICOLON_SYM
                | IdentList COLON_SYM Type SEMICOLON_SYM SubVarDecl
                ;

/* 3.2 CPSL Statements */

StatementSequence: Statement
                | Statement SEMICOLON_SYM StatementSequence
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

SubElseIf:      ELSEIF_SYM Expression THEN_SYM StatementSequence SubElseIf
                | /* nothing */
                ;

SubElse:        ELSE_SYM StatementSequence
                | /* nothing */
                ;

WhileStatement: WHILE_SYM Expression DO_SYM StatementSequence END_SYM 
                ;

RepeatStatement: REPEAT_SYM StatementSequence UNTIL_SYM Expression
                ;

ForStatement:   FOR_SYM IDENT_SYM ASSIGNMENT_SYM Expression TO_SYM Expression DO_SYM StatementSequence END_SYM 
                | FOR_SYM IDENT_SYM ASSIGNMENT_SYM Expression DOWNTO_SYM Expression DO_SYM StatementSequence END_SYM 
                ;

StopStatement:  STOP_SYM
                ;

ReturnStatement: RETURN_SYM  
                | RETURN_SYM Expression
                ;

ReadStatement:  READ_SYM L_PAREN_SYM LValueList R_PAREN_SYM
                ;

LValueList:      LValue
                | LValue COMMA_SYM LValueList
                ;

WriteStatement: WRITE_SYM L_PAREN_SYM ExpressionList R_PAREN_SYM
                ;

ExpressionList: Expression                              { $$ = SymbolTable::makeSymbolVector($1, NULL); }
                | Expression COMMA_SYM ExpressionList   { $$ = SymbolTable::makeSymbolVector($1, $3); }
                ;

ProcedureCall: IDENT_SYM L_PAREN_SYM R_PAREN_SYM
                | IDENT_SYM L_PAREN_SYM ExpressionList R_PAREN_SYM
                ;

NullStatement: 
                ;

/* 3.3 Expressions */

Expression:     Expression OR_SYM Expression            { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression AND_SYM Expression         { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression EQUAL_SYM Expression       { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression NOT_EQUAL_SYM Expression   { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression LT_EQ_SYM Expression       { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression GT_EQ_SYM Expression       { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression LT_SYM Expression          { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression GT_SYM Expression          { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression ADD_SYM Expression         { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression SUBTRACT_SYM Expression    { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression MULTIPLY_SYM Expression    { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression DIVIDE_SYM Expression      { $$ = SymbolTable::expression($1, $2, $3); }
                | Expression MOD_SYM Expression         { $$ = SymbolTable::expression($1, $2, $3); }
                | TILDE_SYM Expression                  { $$ = SymbolTable::expression($1, $2); }
                | NEG_SYM Expression                    { $$ = SymbolTable::expression($1, $2); }
                | L_PAREN_SYM Expression R_PAREN_SYM    { $$ = $2; }
                | IDENT_SYM L_PAREN_SYM R_PAREN_SYM     { $$ = SymbolTable::function_call($1); }
                | IDENT_SYM L_PAREN_SYM ExpressionList R_PAREN_SYM { $$ = SymbolTable::function_call($1, $3); }
                | CHR_SYM L_PAREN_SYM Expression R_PAREN_SYM { $$ = SymbolTable::chr($3); }
                | ORD_SYM L_PAREN_SYM Expression R_PAREN_SYM { $$ = SymbolTable::ord($3); }
                | PRED_SYM L_PAREN_SYM Expression R_PAREN_SYM { $$ = SymbolTable::pred($3); }
                | SUCC_SYM L_PAREN_SYM Expression R_PAREN_SYM { $$ = SymbolTable::succ($3); }
                | LValue  { $$ = SymbolTable::expressionLvalue($1); }
                // | ConstExpression
                // I added this to make it work. Figure out how to fix better later. 
                | INT_CONST_SYM     { $<const_val>$ = new Int($1);     }
                | CHAR_CONST_SYM    { $<const_val>$ = new Char($1);    }
                | STR_CONST_SYM     { $<const_val>$ = new String($1);  }
                ;

/*Expression:     NEG_SYM Expression
                | Expression2
                ; 

Expression2:    Expression2 MULTIPLY_SYM Expression2
                | Expression2 DIVIDE_SYM Expression2
                | Expression2 MOD_SYM Expression2
                | Expression3
                ;

Expression3:    Expression3 ADD_SYM Expression3
                | Expression3 SUBTRACT_SYM Expression3
                | Expression4
                ;

Expression4:    Expression4 EQUAL_SYM Expression4
                | Expression4 NOT_EQUAL_SYM Expression4
                | Expression4 LT_SYM Expression4
                | Expression4 LT_EQ_SYM Expression4
                | Expression4 GT_SYM Expression4
                | Expression4 GT_EQ_SYM Expression4
                | Expression5
                ;

Expression5:    TILDE_SYM Expression5 
                | Expression6
                ;

Expression6:    AND_SYM Expression6
                | Expression7
                ;

Expression7:    OR_SYM Expression7
                | Expression8
                ;

Expression8:    L_PAREN_SYM Expression8 R_PAREN_SYM
                | Expression9
                ;

Expression9:    IDENT_SYM L_PAREN_SYM ExpressionList R_PAREN_SYM
                | CHR_SYM L_PAREN_SYM Expression R_PAREN_SYM
                | ORD_SYM L_PAREN_SYM Expression R_PAREN_SYM
                | PRED_SYM L_PAREN_SYM Expression R_PAREN_SYM
                | SUCC_SYM L_PAREN_SYM Expression R_PAREN_SYM
                | LValue
                | ConstExpression
                ;*/


LValue:         IDENT_SYM                       { $$ = 12; }
                | IDENT_SYM SubLValueStar       { $$ = 13; }
              /*  | IDENT_SYM DOT_SYM IDENT_SYM */
                /*| LValue DOT_SYM LValue*/
                /*| IDENT_SYM L_BRACKET_SYM Expression R_BRACKET_SYM*/
                ;

SubLValueStar:  SubLValue                       { $<int_val>$ = 12; }
                | SubLValue SubLValueStar       { $<int_val>$ = 12; }
                ;

SubLValue:      DOT_SYM IDENT_SYM               { $<int_val>$ = 12; }
                | L_BRACKET_SYM Expression R_BRACKET_SYM { $<int_val>$ = 12; }
                ;

ConstExpression: ConstExpression OR_SYM ConstExpression             { $$ = new Const($1,"|",$3); $$->print(); }
                | ConstExpression AND_SYM ConstExpression           { $$ = new Const($1,"&",$3); $$->print(); }
                | ConstExpression EQUAL_SYM ConstExpression         { $$ = new Const($1,"=",$3); $$->print(); }
                | ConstExpression NOT_EQUAL_SYM ConstExpression     { $$ = new Const($1,"<>",$3); $$->print(); }
                | ConstExpression LT_EQ_SYM ConstExpression         { $$ = new Const($1,"<=",$3); $$->print(); }
                | ConstExpression GT_EQ_SYM ConstExpression         { $$ = new Const($1,">=",$3); $$->print(); }
                | ConstExpression LT_SYM ConstExpression            { $$ = new Const($1,"<",$3); $$->print(); }
                | ConstExpression GT_SYM ConstExpression            { $$ = new Const($1,">",$3); $$->print(); }
                | ConstExpression ADD_SYM ConstExpression           { $$ = new Const($1,"+",$3); $$->print(); }
                | ConstExpression SUBTRACT_SYM ConstExpression      { $$ = new Const($1,"-",$3); $$->print(); }
                | ConstExpression MULTIPLY_SYM ConstExpression      { $$ = new Const($1,"*",$3); $$->print(); }
                | ConstExpression DIVIDE_SYM ConstExpression        { $$ = new Const($1,"/",$3); $$->print(); }
                | ConstExpression MOD_SYM ConstExpression           { $$ = new Const($1,"%",$3); $$->print(); }
                | TILDE_SYM ConstExpression                         { $$ = new Const("~", $2); $$->print(); }
                | NEG_SYM ConstExpression                           { $$ = new Const("neg", $2); $$->print(); }
                | L_PAREN_SYM ConstExpression R_PAREN_SYM           { $$ = $2; $$->print(); }
                | INT_CONST_SYM                                     { $$ = new Int($1); $$->print(); }
                | CHAR_CONST_SYM                                    { $$ = new Char($1); $$->print(); }
                | STR_CONST_SYM                                     { $$ = new String($1); $$->print(); }
                | IDENT_SYM                                         { $$ = new Const($1); $$->print(); }
                ;


/*ConstExpression: NEG_SYM ConstExpression
                | ConstExpr1
                ; 

ConstExpr1:     ConstExpression MULTIPLY_SYM ConstExpr1
                | ConstExpression DIVIDE_SYM ConstExpr1
                | ConstExpression MOD_SYM ConstExpr1
                | ConstExpr2
                ;

ConstExpr2:     ConstExpr2 ADD_SYM ConstExpr2
                | ConstExpr2 SUBTRACT_SYM ConstExpr2
                | ConstExpr3
                ;

ConstExpr3:     ConstExpression EQUAL_SYM ConstExpr3
                | ConstExpression NOT_EQUAL_SYM ConstExpr3
                | ConstExpression LT_SYM ConstExpr3
                | ConstExpression LT_EQ_SYM ConstExpr3
                | ConstExpression GT_SYM ConstExpr3
                | ConstExpression GT_EQ_SYM ConstExpr3
                | ConstExpr4
                ;

ConstExpr4:     TILDE_SYM ConstExpression 
                | ConstExpr5
                ;

ConstExpr5:     AND_SYM ConstExpression
                | ConstExpr6
                ;

ConstExpr6:     OR_SYM ConstExpression
                | ConstExpr7
                ;

ConstExpr7:     L_PAREN_SYM ConstExpression R_PAREN_SYM
                | ConstExpr8
                ;

ConstExpr8:     INT_CONST_SYM
                | CHAR_CONST_SYM
                | STR_CONST_SYM
                | IDENT_SYM 
                ;*/

%%