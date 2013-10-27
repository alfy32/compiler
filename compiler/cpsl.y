
%{

#include "cpsl.h"

extern int lineNumber;    

%}

%union {
    int int_val;
    char* str_val;

    void* op;
    void* token;
    void* keyword;

    Symbol* symbol_val;
    Constant* const_val;
    Type* type_val;
    Func* functionSig;
    Proc* procedureSig;
    Expression* expression;
    Variable* variable;

    std::deque<Symbol*>* symboldeque_val;
    std::deque<std::string>* identList;

    std::deque<std::pair<std::deque<std::string>, Type*> >* recordItem;

    std::deque<Expression*>* expressionList;
    std::deque<Variable*>* variableList;
}

%type <const_val> ConstExpression
%type <expression> Expression 
%type <variable> LValue
%type <expressionList> ExpressionList 
%type <variableList> LValueList
%type <identList> IdentList
%type <recordItem> RecordItem FormalParameters
%type <type_val> Type SimpleType ArrayType RecordType
%type <functionSig> FunctionSig
%type <procedureSig> ProcedureSig

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

%right          NEG_SYM
%left           MULTIPLY_SYM DIVIDE_SYM MOD_SYM
%left           SUBTRACT_SYM ADD_SYM
%nonassoc       EQUAL_SYM NOT_EQUAL_SYM LT_SYM LT_EQ_SYM GT_SYM GT_EQ_SYM
%right          TILDE_SYM
%left           AND_SYM
%left           OR_SYM

%%

Program:        Init SubCDecl SubTDecl SubVDecl SubPFDecl Block DOT_SYM { SymbolTable::pop(); SymbolTable::pop(); SymbolTable::printStringConstants(); }
                ;

Init:           { SymbolTable::initAssembly(); }
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

ProcedureDecl:  ProcedureSig FORWARD_SYM SEMICOLON_SYM  { SymbolTable::pop(); }  
                | ProcedureSig Body SEMICOLON_SYM       { SymbolTable::pop(); }  
                ;

ProcedureSig:   PROCEDURE_SYM IDENT_SYM L_PAREN_SYM FormalParameters R_PAREN_SYM SEMICOLON_SYM  { $$ = new Proc($2, $4); SymbolTable::procDecl($2, $$); }
                ;

FunctionDecl:   FunctionSig FORWARD_SYM SEMICOLON_SYM   { SymbolTable::pop(); }
                | FunctionSig Body SEMICOLON_SYM        { SymbolTable::pop(); }
                ;

FunctionSig:    FUNCTION_SYM IDENT_SYM L_PAREN_SYM FormalParameters R_PAREN_SYM COLON_SYM Type SEMICOLON_SYM { $$ = new Func($2, $4, $7); SymbolTable::funcDecl($2, $$); }
                ;

FormalParameters: /* nothing */                                                     { $$ = NULL; }
                | SubVar IdentList COLON_SYM Type                                   { $$ = SymbolTable::makeRecordItem($2, $4, NULL); }
                | SubVar IdentList COLON_SYM Type SEMICOLON_SYM FormalParameters    { $$ = SymbolTable::makeRecordItem($2, $4, $6); }
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
                | SubTypeDecl IDENT_SYM EQUAL_SYM Type SEMICOLON_SYM    { SymbolTable::typeDecl($2,$4); }

Type:           SimpleType      { $$ = $1; }
                | RecordType    { $$ = $1; } 
                | ArrayType     { $$ = $1; } 
                ;

SimpleType:     IDENT_SYM       { $$ = (Type*)SymbolTable::lookup($1); }
                ;

RecordType:     RECORD_SYM RecordItem END_SYM  { $$ = new Record($2); }
                ;

RecordItem:     IdentList COLON_SYM Type SEMICOLON_SYM                  { $$ = SymbolTable::makeRecordItem($1, $3, NULL); }
                | IdentList COLON_SYM Type SEMICOLON_SYM RecordItem     { $$ = SymbolTable::makeRecordItem($1, $3, $5); }
                ;

ArrayType:      ARRAY_SYM L_BRACKET_SYM ConstExpression COLON_SYM ConstExpression R_BRACKET_SYM OF_SYM Type { $$ = new Array($3, $5, $8); }
                ;

IdentList:      IDENT_SYM                           { $$ = SymbolTable::makeIdentList($1, NULL); }
                | IDENT_SYM COMMA_SYM IdentList     { $$ = SymbolTable::makeIdentList($1, $3); }
                ;

/* 3.1.4 Variable Declarations */

VarDecl:        VAR_SYM SubVarDecl             
                ;

SubVarDecl:     IdentList COLON_SYM Type SEMICOLON_SYM                  { SymbolTable::addVar($1, $3); }
                | SubVarDecl IdentList COLON_SYM Type SEMICOLON_SYM     { SymbolTable::addVar($2, $4); }
                ;

/* 3.2 CPSL Statements */

StatementSequence: Statement                                    
                | Statement SEMICOLON_SYM StatementSequence     
                ;

Statement:      Assignemnt          
                | IfStatement       { SymbolTable::endStatement(); }
                | WhileStatement    { SymbolTable::endStatement(); }
                | RepeatStatement   { SymbolTable::endStatement(); }
                | ForStatement      { SymbolTable::endStatement(); }
                | StopStatement     
                | ReturnStatement   
                | ReadStatement     { SymbolTable::endStatement(); }
                | WriteStatement    { SymbolTable::endStatement(); }
                | ProcedureCall     
                | NullStatement     
                ;

Assignemnt:     LValue ASSIGNMENT_SYM Expression    { SymbolTable::assignment($1, $3); }
                ;

IfStatement:    If Then ElseIfThen Else END_SYM     { SymbolTable::afterIf(); }
                ;

If:             IF_SYM Expression                   { SymbolTable::ifStatement($2); }
                ;

Then:           THEN_SYM StatementSequence          { SymbolTable::thenStatement(); }
                ;

ElseIfThen:     ElseIf Then ElseIfThen              
                | 
                ;

ElseIf:         ELSEIF_SYM Expression               { SymbolTable::ifStatement($2); }
                ;

Else:           ELSE_SYM StatementSequence          { SymbolTable::elseStatement(); }
                |
                ;

WhileStatement: WhileInit WhileBranch DO_SYM StatementSequence WhileRepeat 
                ;

WhileInit:      WHILE_SYM       { SymbolTable::whileInit(); }
                ;

WhileBranch:    Expression      { SymbolTable::whileBranch($1); }
                ;

WhileRepeat:    END_SYM         { SymbolTable::whileRepeat(); }
                ;

RepeatStatement: RepeatInit StatementSequence UNTIL_SYM Expression  { SymbolTable::repeatEnd($4); }
                ;

RepeatInit:     REPEAT_SYM      { SymbolTable::repeatInit(); }
                ;

ForStatement:   ForInit ForLabel ForEval DO_SYM StatementSequence ForEnd 
                ;

ForInit:        FOR_SYM IDENT_SYM ASSIGNMENT_SYM Expression         { SymbolTable::initFor($2,$4); }
                ;

ForLabel:       TO_SYM                          { SymbolTable::forLabel("UP"); }
                | DOWNTO_SYM                    { SymbolTable::forLabel("DOWN"); }
                ;

ForEval:        Expression                      { SymbolTable::forEval($1); }
                ;

ForEnd:         END_SYM                         { SymbolTable::forEnd(); }            
                ;

StopStatement:  STOP_SYM
                ;

ReturnStatement: RETURN_SYM  
                | RETURN_SYM Expression
                ;

ReadStatement:  READ_SYM L_PAREN_SYM LValueList R_PAREN_SYM    { SymbolTable::read($3); }
                ;

LValueList:      LValue                             { $$ = SymbolTable::makeVariableList($1, NULL); }
                | LValue COMMA_SYM LValueList       { $$ = SymbolTable::makeVariableList($1, $3); }
                ;

WriteStatement: WRITE_SYM L_PAREN_SYM ExpressionList R_PAREN_SYM { SymbolTable::write($3); }
                ;

ExpressionList: Expression                              { $$ = SymbolTable::makeExpressionList($1, NULL); }
                | Expression COMMA_SYM ExpressionList   { $$ = SymbolTable::makeExpressionList($1, $3); }
                ;

ProcedureCall: IDENT_SYM L_PAREN_SYM R_PAREN_SYM
                | IDENT_SYM L_PAREN_SYM ExpressionList R_PAREN_SYM
                ;

NullStatement: 
                ;

/* 3.3 Expressions */

Expression:     Expression OR_SYM Expression            { $$ = SymbolTable::expression($1, "or", $3); }
                | Expression AND_SYM Expression         { $$ = SymbolTable::expression($1, "and", $3); }
                | Expression EQUAL_SYM Expression       { $$ = SymbolTable::expression($1, "seq", $3); }
                | Expression NOT_EQUAL_SYM Expression   { $$ = SymbolTable::expression($1, "sne", $3); }
                | Expression LT_EQ_SYM Expression       { $$ = SymbolTable::expression($1, "sle", $3); }
                | Expression GT_EQ_SYM Expression       { $$ = SymbolTable::expression($1, "sge", $3); }
                | Expression LT_SYM Expression          { $$ = SymbolTable::expression($1, "slt", $3); }
                | Expression GT_SYM Expression          { $$ = SymbolTable::expression($1, "sgt", $3); }
                | Expression ADD_SYM Expression         { $$ = SymbolTable::expression($1, "add", $3); }
                | Expression SUBTRACT_SYM Expression    { $$ = SymbolTable::expression($1, "sub", $3); }
                | Expression MULTIPLY_SYM Expression    { $$ = SymbolTable::expression($1, "mult", $3); }
                | Expression DIVIDE_SYM Expression      { $$ = SymbolTable::expression($1, "div", $3); }
                | Expression MOD_SYM Expression         { $$ = SymbolTable::expression($1, "mod", $3); }
                | TILDE_SYM Expression                  { $$ = SymbolTable::expression("~", $2); }
                | NEG_SYM Expression                    { $$ = SymbolTable::expression("Neg", $2); }
                | L_PAREN_SYM Expression R_PAREN_SYM    { $$ = $2; }
                | IDENT_SYM L_PAREN_SYM R_PAREN_SYM     { $$ = SymbolTable::function_call($1); }
                | IDENT_SYM L_PAREN_SYM ExpressionList R_PAREN_SYM  { $$ = SymbolTable::function_call($1, $3); }
                | CHR_SYM L_PAREN_SYM Expression R_PAREN_SYM        { $$ = SymbolTable::chr($3); }
                | ORD_SYM L_PAREN_SYM Expression R_PAREN_SYM        { $$ = SymbolTable::ord($3); }
                | PRED_SYM L_PAREN_SYM Expression R_PAREN_SYM       { $$ = SymbolTable::pred($3); }
                | SUCC_SYM L_PAREN_SYM Expression R_PAREN_SYM       { $$ = SymbolTable::succ($3); }
                | LValue                                            { $$ = SymbolTable::lValueToExpression($1); }
                // | ConstExpression
                // I added this to make it work. Figure out how to fix better later. 
                | INT_CONST_SYM     { $$ = SymbolTable::integerConstToExpression($1); }
                | CHAR_CONST_SYM    { $$ = SymbolTable::charConstToExpression($1); }
                | STR_CONST_SYM     { $$ = SymbolTable::stringConstToExpression($1); }
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


LValue:         IDENT_SYM                       { $$ = SymbolTable::makeLvalue($1); }
                | IDENT_SYM SubLValueStar       { $$ = SymbolTable::makeLvalue($1); }
              /*  | IDENT_SYM DOT_SYM IDENT_SYM */
                /*| LValue DOT_SYM LValue*/
                /*| IDENT_SYM L_BRACKET_SYM Expression R_BRACKET_SYM*/
                ;

SubLValueStar:  SubLValue                       
                | SubLValue SubLValueStar       
                ;

SubLValue:      DOT_SYM IDENT_SYM                          
                | L_BRACKET_SYM Expression R_BRACKET_SYM    
                ;

ConstExpression: ConstExpression OR_SYM ConstExpression             { $$ = SymbolTable::evalConstant($1,"|",$3); }
                | ConstExpression AND_SYM ConstExpression           { $$ = SymbolTable::evalConstant($1,"&",$3); }
                | ConstExpression EQUAL_SYM ConstExpression         { $$ = SymbolTable::evalConstant($1,"=",$3); }
                | ConstExpression NOT_EQUAL_SYM ConstExpression     { $$ = SymbolTable::evalConstant($1,"<>",$3); }
                | ConstExpression LT_EQ_SYM ConstExpression         { $$ = SymbolTable::evalConstant($1,"<=",$3); }
                | ConstExpression GT_EQ_SYM ConstExpression         { $$ = SymbolTable::evalConstant($1,">=",$3); }
                | ConstExpression LT_SYM ConstExpression            { $$ = SymbolTable::evalConstant($1,"<",$3); }
                | ConstExpression GT_SYM ConstExpression            { $$ = SymbolTable::evalConstant($1,">",$3); }
                | ConstExpression ADD_SYM ConstExpression           { $$ = SymbolTable::evalConstant($1,"+",$3); }
                | ConstExpression SUBTRACT_SYM ConstExpression      { $$ = SymbolTable::evalConstant($1,"-",$3); }
                | ConstExpression MULTIPLY_SYM ConstExpression      { $$ = SymbolTable::evalConstant($1,"*",$3); }
                | ConstExpression DIVIDE_SYM ConstExpression        { $$ = SymbolTable::evalConstant($1,"/",$3); }
                | ConstExpression MOD_SYM ConstExpression           { $$ = SymbolTable::evalConstant($1,"%",$3); }
                | TILDE_SYM ConstExpression                         { $$ = SymbolTable::evalConstant("~", $2); }
                | NEG_SYM ConstExpression                           { $$ = SymbolTable::evalConstant("neg", $2); }
                | L_PAREN_SYM ConstExpression R_PAREN_SYM           { $$ = $2; }
                | INT_CONST_SYM                                     { $$ = new IntegerConstant($1); }
                | CHAR_CONST_SYM                                    { $$ = new CharacterConstant($1); }
                | STR_CONST_SYM                                     { $$ = new StringConstant($1); }
                | IDENT_SYM                                         { $$ = SymbolTable::lookupConstant($1); }
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

int yyerror(const char *s)
{
    std::cout << "ERROR line " << lineNumber << ": " << s << std::endl;

    exit(1);
    
    return 1;
}