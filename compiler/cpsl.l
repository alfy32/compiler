/* scanner for CPSL written by Alan Christensen */

%{
#include "cpsl.h"
#include "cpsl.tab.h"

int yyerror(const char* s);
int yyLineCount = 1;

void keyword(std::string word, std::string text);
%}

%%
	
ARRAY|array				{ keyword("ARRAY", yytext); 	return ARRAY_SYM;}
ELSE|else				{ keyword("ELSE", yytext); 		return ELSE_SYM;}
IF|if					{ keyword("IF", yytext); 		return IF_SYM;}
RECORD|record			{ keyword("RECORD", yytext); 	return RECORD_SYM;}
TO|to					{ keyword("TO", yytext); 		return TO_SYM;}
BEGIN|begin				{ keyword("BEGIN", yytext); 	return BEGIN_SYM;}
ELSEIF|elseif			{ keyword("ELSEIF", yytext); 	return ELSEIF_SYM;}
OF|of					{ keyword("OF", yytext); 		return OF_SYM;}
REPEAT|repeat			{ keyword("REPEAT", yytext); 	return REPEAT_SYM;}
TYPE|type				{ keyword("TYPE", yytext); 		return TYPE_SYM;}
CHR|chr					{ keyword("CHR", yytext); 		return CHR_SYM;}
END|end					{ keyword("END", yytext); 		return END_SYM;}
ORD|ord					{ keyword("ORD", yytext);       return ORD_SYM;}
RETURN|return			{ keyword("RETURN", yytext);    return RETURN_SYM;}
UNTIL|until				{ keyword("UNTIL", yytext);     return UNTIL_SYM;}
CONST|const				{ keyword("CONST", yytext);     return CONST_SYM;}
FOR|for					{ keyword("FOR", yytext);       return FOR_SYM;}
PRED|pred				{ keyword("PRED", yytext);      return PRED_SYM;}
STOP|stop				{ keyword("STOP", yytext);      return STOP_SYM;}
VAR|var					{ keyword("VAR", yytext);       return VAR_SYM;}
DO|do					{ keyword("DO", yytext);        return DO_SYM;}
FORWARD|forward			{ keyword("FORWARD", yytext);   return FORWARD_SYM;}
PROCEDURE|procedure		{ keyword("PROCEDURE", yytext); return PROCEDURE_SYM;}
SUCC|succ				{ keyword("SUCC", yytext);      return SUCC_SYM;}
WHILE|while				{ keyword("WHILE", yytext);     return WHILE_SYM;}
DOWNTO|downto			{ keyword("DOWNTO", yytext);    return DOWNTO_SYM;}
FUNCTION|function		{ keyword("FUNCTION", yytext);  return FUNCTION_SYM;}
READ|read				{ keyword("READ", yytext);      return READ_SYM;}
THEN|then				{ keyword("THEN", yytext);      return THEN_SYM;}
WRITE|write				{ keyword("WRITE", yytext);     return WRITE_SYM;}


integer					{ yylval.     printf( "Identifier: integer (%s)\n", yytext); return IDENT_SYM;}
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

void keyword(std::string word, std::string text) {
	std::cout << "Keyword: " << word << " (" << text << ")" << std::endl;
}
