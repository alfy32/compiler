
%{

#include "cpsl.h"
int yyerror(const char*);
int yylex(void);

SymbolTab symbolTab;

%}

%union {
    int int_val;
    char char_val;
    std::string* str_val;

    int op;
    int token;
    int keyword;

    std::string* indent;

    Symbol* symbol;
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

%token <indent>     IDENT_SYM
%token <int_val>    INT_CONST_SYM
%token <char_val>   CHAR_CONST_SYM
%token <str_val>    STR_CONST_SYM

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

SubConstDecl:   IDENT_SYM EQUAL_SYM ConstExpression SEMICOLON_SYM
                | IDENT_SYM EQUAL_SYM ConstExpression SEMICOLON_SYM SubConstDecl

/* 3.1.2 Procedure and Function Declarations */

ProcedureDecl:  PROCEDURE_SYM IDENT_SYM L_PAREN_SYM FormalParameters R_PAREN_SYM SEMICOLON_SYM FORWARD_SYM SEMICOLON_SYM   // { SymbolTab::add(std::make_shared<Func>($2,$4,true)); }
                | PROCEDURE_SYM IDENT_SYM L_PAREN_SYM FormalParameters R_PAREN_SYM SEMICOLON_SYM Body SEMICOLON_SYM        // { SymbolTab::add(std::make_shared<Func>($2,$4,true)); }
                ;

FunctionDecl:   FUNCTION_SYM IDENT_SYM L_PAREN_SYM FormalParameters R_PAREN_SYM COLON_SYM Type SEMICOLON_SYM FORWARD_SYM SEMICOLON_SYM // { SymbolTab::add(std::make_shared<Func>($2,$4);}
                | FUNCTION_SYM IDENT_SYM L_PAREN_SYM FormalParameters R_PAREN_SYM COLON_SYM Type SEMICOLON_SYM Body SEMICOLON_SYM      // { SymbolTab::addFuncProc($2,$4);}
                ;

FormalParameters: /* nothing */
                | SubVar IdentList COLON_SYM Type //{ $$ = SymbolTab::makeFormalParams($2, $4); }
                | SubVar IdentList COLON_SYM Type SEMICOLON_SYM FormalParameters //{ $$ = SymbolTab::makeFormalParams($2, $4, $6); }
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

SubTypeDecl:    IDENT_SYM EQUAL_SYM Type SEMICOLON_SYM //{ SymbolTab::addType($3, $1); }
                | IDENT_SYM EQUAL_SYM Type SEMICOLON_SYM SubTypeDecl //{ SymbolTab::addType($3, $1); }

Type:           SimpleType      //{ $$ = $1; }
                | RecordType    //{ $$ = $1; }
                | ArrayType     //{ $$ = $1; }
                ;

SimpleType:     IDENT_SYM       //{ $$ = $1; }
                ;

RecordType:     RECORD_SYM RecordItem END_SYM   //{ $$ = SymbolTab::makeRecordType($2); }
                ;

RecordItem:     IdentList COLON_SYM Type SEMICOLON_SYM RecordItem //{ $$ = SymbolTab::makeRecordItem($1, $3, $5);}
                | /* nothing */
                ;

ArrayType:      ARRAY_SYM L_BRACKET_SYM ConstExpression COLON_SYM ConstExpression R_BRACKET_SYM OF_SYM Type //{ $$ = SymbolTab::makeArrayType($3,$5,$8); }
                ;

IdentList:      IDENT_SYM                           //{ $$ = SymbolTab::makeIdentList($1); }
                | IDENT_SYM COMMA_SYM IdentList     //{ $$ = SymbolTab::makeIdentList($1, $3); }
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

ExpressionList: Expression
                | Expression COMMA_SYM ExpressionList
                ;

ProcedureCall: IDENT_SYM L_PAREN_SYM R_PAREN_SYM
                | IDENT_SYM L_PAREN_SYM ExpressionList R_PAREN_SYM
                ;

NullStatement: 
                ;

/* 3.3 Expressions */

Expression:     Expression OR_SYM Expression
                | Expression AND_SYM Expression
                | Expression EQUAL_SYM Expression
                | Expression NOT_EQUAL_SYM Expression
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
                | IDENT_SYM L_PAREN_SYM R_PAREN_SYM
                | IDENT_SYM L_PAREN_SYM ExpressionList R_PAREN_SYM
                | CHR_SYM L_PAREN_SYM Expression R_PAREN_SYM
                | ORD_SYM L_PAREN_SYM Expression R_PAREN_SYM
                | PRED_SYM L_PAREN_SYM Expression R_PAREN_SYM
                | SUCC_SYM L_PAREN_SYM Expression R_PAREN_SYM
                | LValue
                /*| ConstExpression*/
                | INT_CONST_SYM   /* I added this to make it work. Figure out how to fix better later. */
                | CHAR_CONST_SYM
                | STR_CONST_SYM
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


LValue:         IDENT_SYM 
                | IDENT_SYM SubLValueStar
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

ConstExpression: ConstExpression OR_SYM ConstExpression
                | ConstExpression AND_SYM ConstExpression
                | ConstExpression EQUAL_SYM ConstExpression
                | ConstExpression NOT_EQUAL_SYM ConstExpression
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

int yyerror(string s)
{
        extern int yylineno;  
        extern char *yytext;  

        cerr << "ERROR: " << s << " at symbol \"" << yytext;
        cerr << "\" on line " << yylineno << endl;
        exit(1);
}

int yyerror(const char *s)
{
        return yyerror(string(s));
}