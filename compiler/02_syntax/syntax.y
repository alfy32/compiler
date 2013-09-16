
%{
#define YYSTYPE double
#include <math.h>
%}

/* BISON Declarations */
%token NUM
%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation        */

/* Grammar follows */
%%

Program:	ConstantDecl TypeDecl VarDecl 
		| 

ConstantDecl: 	const <ident> = ConstExpression ; ConstantDecl
				| const <ident> = ConstExpression ; 

ProcedureDecl: 	procedure <ident> ( FormalParameters ) ; forward ;
				| procedure <ident> ( FormalParameters ) ; Body ;

FunctionDecl:	function <ident> ( FormalParameters ) : Type ; for-ward ;
				| function <ident> ( FormalParameters ) : Type ; Body ;

FormalParameters: /* empty string */
				| FormalParameters var? IdentList : Type (; var? IdentList : Type )*
				| var? IdentList : Type (; var? IdentList : Type )*

Body:			ConstantDecl TypeDecl VarDecl Block

Block:			begin StatementSequence end


input:    /* empty string */
        | input line
;

line:     '\n'
        | exp '\n'  { printf ("\t%.10g\n", $1); }
;

exp:      NUM                { $$ = $1;         }
        | exp '+' exp        { $$ = $1 + $3;    }
        | exp '-' exp        { $$ = $1 - $3;    }
        | exp '*' exp        { $$ = $1 * $3;    }
        | exp '/' exp        { $$ = $1 / $3;    }
        | '-' exp  %prec NEG { $$ = -$2;        }
        | exp '^' exp        { $$ = pow ($1, $3); }
        | '(' exp ')'        { $$ = $2;         }
;
%%