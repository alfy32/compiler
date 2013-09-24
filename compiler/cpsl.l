/* scanner for CPSL written by Alan Christensen */

%{
#include "cpsl.h"
#include "cpsl.tab.h"

int yyerror(const char* s);
int yyLineCount = 1;
%}

%%
	
ARRAY|array				{ printf( "Keyword: ARRAY (%s)\n", yytext); return ARRAY_SYM;}
ELSE|else				{ printf( "Keyword: ELSE (%s)\n", yytext); return ELSE_SYM;}
IF|if					{ printf( "Keyword: IF (%s)\n", yytext); return IF_SYM;}
RECORD|record			{ printf( "Keyword: RECORD (%s)\n", yytext); return RECORD_SYM;}
TO|to					{ printf( "Keyword: TO (%s)\n", yytext); return TO_SYM;}
BEGIN|begin				{ printf( "Keyword: BEGIN (%s)\n", yytext); return BEGIN_SYM;}
ELSEIF|elseif			{ printf( "Keyword: ELSEIF (%s)\n", yytext); return ELSEIF_SYM;}
OF|of					{ printf( "Keyword: OF (%s)\n", yytext); return OF_SYM;}
REPEAT|repeat			{ printf( "Keyword: REPEAT (%s)\n", yytext); return REPEAT_SYM;}
TYPE|type				{ printf( "Keyword: TYPE (%s)\n", yytext); return TYPE_SYM;}
CHR|chr					{ printf( "Keyword: CHR (%s)\n", yytext); return CHR_SYM;}
END|end					{ printf( "Keyword: END (%s)\n", yytext); return END_SYM;}
ORD|ord					{ printf( "Keyword: ORD (%s)\n", yytext); return ORD_SYM;}
RETURN|return			{ printf( "Keyword: RETURN (%s)\n", yytext); return RETURN_SYM;}
UNTIL|until				{ printf( "Keyword: UNTIL (%s)\n", yytext); return UNTIL_SYM;}
CONST|const				{ printf( "Keyword: CONST (%s)\n", yytext); return CONST_SYM;}
FOR|for					{ printf( "Keyword: FOR (%s)\n", yytext); return FOR_SYM;}
PRED|pred				{ printf( "Keyword: PRED (%s)\n", yytext); return PRED_SYM;}
STOP|stop				{ printf( "Keyword: STOP (%s)\n", yytext); return STOP_SYM;}
VAR|var					{ printf( "Keyword: VAR (%s)\n", yytext); return VAR_SYM;}
DO|do					{ printf( "Keyword: DO (%s)\n", yytext); return DO_SYM;}
FORWARD|forward			{ printf( "Keyword: FORWARD (%s)\n", yytext); return FORWARD_SYM;}
PROCEDURE|procedure		{ printf( "Keyword: PROCEDURE (%s)\n", yytext); return PROCEDURE_SYM;}
SUCC|succ				{ printf( "Keyword: SUCC (%s)\n", yytext); return SUCC_SYM;}
WHILE|while				{ printf( "Keyword: WHILE (%s)\n", yytext); return WHILE_SYM;}
DOWNTO|downto			{ printf( "Keyword: DOWNTO (%s)\n", yytext); return DOWNTO_SYM;}
FUNCTION|function		{ printf( "Keyword: FUNCTION (%s)\n", yytext); return FUNCTION_SYM;}
READ|read				{ printf( "Keyword: READ (%s)\n", yytext); return READ_SYM;}
THEN|then				{ printf( "Keyword: THEN (%s)\n", yytext); return THEN_SYM;}
WRITE|write				{ printf( "Keyword: WRITE (%s)\n", yytext); return WRITE_SYM;}


integer					{ printf( "Identifier: integer (%s)\n", yytext); return IDENT_SYM;}
char					{ printf( "Identifier: char (%s)\n", yytext); return IDENT_SYM;}
boolean					{ printf( "Identifier: boolean (%s)\n", yytext); return IDENT_SYM;}
string	 				{ printf( "Identifier: string (%s)\n", yytext); return IDENT_SYM;}
true					{ printf( "Identifier: true (%s)\n", yytext); return IDENT_SYM;}
false					{ printf( "Identifier: false (%s)\n", yytext); return IDENT_SYM;}

[a-zA-Z][a-zA-Z0-9_]*	{ printf( "Identifier: (%s)\n", yytext); return IDENT_SYM;}


"+"						{ printf( "An operator: %s\n", yytext ); return ADD_SYM;}
"-"						{ printf( "An operator: %s\n", yytext ); return SUBTRACT_SYM;}
"*"						{ printf( "An operator: %s\n", yytext ); return MULTIPLY_SYM;}
"/"						{ printf( "An operator: %s\n", yytext ); return DIVIDE_SYM;}
"&"						{ printf( "An operator: %s\n", yytext ); return AND_SYM;}
"|"						{ printf( "An operator: %s\n", yytext ); return OR_SYM;}
"~"						{ printf( "An operator: %s\n", yytext ); return TILDE_SYM;}
"="						{ printf( "An operator: %s\n", yytext ); return EQUAL_SYM;}
"<>"					{ printf( "An operator: %s\n", yytext ); return NOT_EQUAL_SYM;}
"<"						{ printf( "An operator: %s\n", yytext ); return LT_SYM;}
"<="					{ printf( "An operator: %s\n", yytext ); return LT_EQ_SYM;}
">"						{ printf( "An operator: %s\n", yytext ); return GT_SYM;}
">="					{ printf( "An operator: %s\n", yytext ); return GT_EQ_SYM;}
"."						{ printf( "An operator: %s\n", yytext ); return DOT_SYM;}
","						{ printf( "An operator: %s\n", yytext ); return COMMA_SYM;}
":"						{ printf( "An operator: %s\n", yytext ); return COLON_SYM;}
";"						{ printf( "An operator: %s\n", yytext ); return SEMICOLON_SYM;}
"("						{ printf( "An operator: %s\n", yytext ); return L_PAREN_SYM;}
")"						{ printf( "An operator: %s\n", yytext ); return R_PAREN_SYM;}
"["						{ printf( "An operator: %s\n", yytext ); return L_BRACKET_SYM;}
"]"						{ printf( "An operator: %s\n", yytext ); return R_BRACKET_SYM;}
":="					{ printf( "An operator: %s\n", yytext ); return ASSIGNMENT_SYM;}
"%"						{ printf( "An operator: %s\n", yytext ); return MOD_SYM;}


0[0-7]+					{ printf( "An Octal Number: (%s)\n", yytext); return INT_CONST_SYM;}
0x[0-9a-fA-f]+			{ printf( "A Hex Number: (%s)\n", yytext); return INT_CONST_SYM;} /* CHECK Is this right? */
[0-9]+					{ printf( "A Decimal Number: (%s)\n", yytext); return INT_CONST_SYM;} /* CHECK Is this right? */


'[ -~]'					{ printf( "A Character Constant: (%s)\n", yytext); return CHAR_CONST_SYM;}
'\\n'|'\\r'|'\\b'		{ printf( "A Character Constant: (%s)\n", yytext); return CHAR_CONST_SYM;}
'\\t'|'\\f'				{ printf( "A Character Constant: (%s)\n", yytext); return CHAR_CONST_SYM;}

["][ !#-~]*["]			{ printf( "A String Constant: (%s)\n", yytext); return STR_CONST_SYM;}


$.*						{ printf( "A Comment: (%s)\n", yytext);}


[ \t]*					/* eat up whitespace */

[\n]					{ printf ( "End of line: %i\n", yylineno++);}


<<EOF>>					{ printf( "This is the end!\n", yytext); return 0;}


.						{ printf( "************UNRECOGNIZED CHARACTER: (%s)************\n", yytext );  yyerror("Scanner: Invalid Character.");}

%%

int yywrap() {
	return 1;
}

/*int main( int argc, char **argv ) {
	++argv, --argc;  
	if ( argc > 0 )
	     yyin = fopen( argv[0], "r" );
	else
	     yyin = stdin;

	yylex();
}*/
