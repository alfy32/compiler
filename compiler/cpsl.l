/* scanner for CPSL written by Alan Christensen */

%{
#include "cpsl.h"
#include "cpsl.tab.h"

int yyerror(const char* s);
int yyLineCount = 1;

void keyword(std::string word);
void identifier(std::string ident);
void oper();
void octal();
void hex();
void decimal();
void charConst();
void strConst();
void comment();
void endOfLine();
void endOfFile();
void unrecognized();
%}

%%
	
ARRAY|array				{ keyword("ARRAY"); 	return ARRAY_SYM;}
ELSE|else				{ keyword("ELSE"); 		return ELSE_SYM;}
IF|if					{ keyword("IF"); 		return IF_SYM;}
RECORD|record			{ keyword("RECORD"); 	return RECORD_SYM;}
TO|to					{ keyword("TO"); 		return TO_SYM;}
BEGIN|begin				{ keyword("BEGIN"); 	return BEGIN_SYM;}
ELSEIF|elseif			{ keyword("ELSEIF"); 	return ELSEIF_SYM;}
OF|of					{ keyword("OF"); 		return OF_SYM;}
REPEAT|repeat			{ keyword("REPEAT"); 	return REPEAT_SYM;}
TYPE|type				{ keyword("TYPE"); 		return TYPE_SYM;}
CHR|chr					{ keyword("CHR"); 		return CHR_SYM;}
END|end					{ keyword("END"); 		return END_SYM;}
ORD|ord					{ keyword("ORD");       return ORD_SYM;}
RETURN|return			{ keyword("RETURN");    return RETURN_SYM;}
UNTIL|until				{ keyword("UNTIL");     return UNTIL_SYM;}
CONST|const				{ keyword("CONST");     return CONST_SYM;}
FOR|for					{ keyword("FOR");       return FOR_SYM;}
PRED|pred				{ keyword("PRED");      return PRED_SYM;}
STOP|stop				{ keyword("STOP");      return STOP_SYM;}
VAR|var					{ keyword("VAR");       return VAR_SYM;}
DO|do					{ keyword("DO");        return DO_SYM;}
FORWARD|forward			{ keyword("FORWARD");   return FORWARD_SYM;}
PROCEDURE|procedure		{ keyword("PROCEDURE"); return PROCEDURE_SYM;}
SUCC|succ				{ keyword("SUCC");      return SUCC_SYM;}
WHILE|while				{ keyword("WHILE");     return WHILE_SYM;}
DOWNTO|downto			{ keyword("DOWNTO");    return DOWNTO_SYM;}
FUNCTION|function		{ keyword("FUNCTION");  return FUNCTION_SYM;}
READ|read				{ keyword("READ");      return READ_SYM;}
THEN|then				{ keyword("THEN");      return THEN_SYM;}
WRITE|write				{ keyword("WRITE");     return WRITE_SYM;}


integer					{ identifier("integer");return IDENT_SYM;}
char					{ identifier("char"); 	return IDENT_SYM;}
boolean					{ identifier("boolean");return IDENT_SYM;}
string	 				{ identifier("string"); return IDENT_SYM;}
true					{ identifier("true"); 	return IDENT_SYM;}
false					{ identifier("false"); 	return IDENT_SYM;}

[a-zA-Z][a-zA-Z0-9_]*	{ identifier("variable"); return IDENT_SYM;}


"+"						{ oper(); return ADD_SYM;}
"-"						{ oper(); return SUBTRACT_SYM;}
"*"						{ oper(); return MULTIPLY_SYM;}
"/"						{ oper(); return DIVIDE_SYM;}
"&"						{ oper(); return AND_SYM;}
"|"						{ oper(); return OR_SYM;}
"~"						{ oper(); return TILDE_SYM;}
"="						{ oper(); return EQUAL_SYM;}
"<>"					{ oper(); return NOT_EQUAL_SYM;}
"<"						{ oper(); return LT_SYM;}
"<="					{ oper(); return LT_EQ_SYM;}
">"						{ oper(); return GT_SYM;}
">="					{ oper(); return GT_EQ_SYM;}
"."						{ oper(); return DOT_SYM;}
","						{ oper(); return COMMA_SYM;}
":"						{ oper(); return COLON_SYM;}
";"						{ oper(); return SEMICOLON_SYM;}
"("						{ oper(); return L_PAREN_SYM;}
")"						{ oper(); return R_PAREN_SYM;}
"["						{ oper(); return L_BRACKET_SYM;}
"]"						{ oper(); return R_BRACKET_SYM;}
":="					{ oper(); return ASSIGNMENT_SYM;}
"%"						{ oper(); return MOD_SYM;}


0[0-7]+					{ octal(); 		return INT_CONST_SYM;}
0x[0-9a-fA-f]+			{ hex(); 		return INT_CONST_SYM;} 
[0-9]+					{ decimal(); 	return INT_CONST_SYM;} 


'[ -~]'					{ charConst(); 	return CHAR_CONST_SYM;}
'\\n'|'\\r'|'\\b'		{ charConst(); 	return CHAR_CONST_SYM;}
'\\t'|'\\f'				{ charConst(); 	return CHAR_CONST_SYM;}

["][ !#-~]*["]			{ strConst(); 	return STR_CONST_SYM;}


$.*						{ comment(); }


[ \t]*					/* eat up whitespace */

[\n]					{ endOfLine(); }


<<EOF>>					{ endOfFile(); 	return 0;}


.						{ unrecognized(); }

%%

int yywrap() {
	return 1;
}

int main( int argc, char **argv ) {
	++argv, --argc;  
	if ( argc > 0 )
	     yyin = fopen( argv[0], "r" );
	else
	     yyin = stdin;

	yylex();
}

void keyword(std::string word) {
	std::cout << "Keyword: " << word << " (" << yytext << ")" << std::endl;
}

void identifier(std::string ident) {
	std::cout << "Identifier: " << ident << " (" << yytext << ")" << std::endl;
}

void oper() {
	std::cout << "Operator: " << yytext << std::endl;
}

void octal() {
	std::cout << "Octal Number: " << yytext << std::endl;

	yylval.int_val = std::stoi(yytext, nullptr, 8);
}
void hex() {
	std::cout << "Hex Number: " << yytext << std::endl;

	yylval.int_val = std::stoi(yytext, nullptr, 16);
}
void decimal() {
	std::cout << "Decimal Number: " << yytext << std::endl;

	yylval.int_val = std::stoi(yytext, nullptr, 10);
}

void charConst() {
	std::cout << "Character Constant: " << yytext << std::endl;

	yylval.char_val = yytext[1];
}
void strConst() {
	std::cout << "String Constant: " << yytext << std::endl;

	yylval.str_val = new std::string(yytext);
}

void comment() {
	std::cout << "Comment: " << yytext << std::endl; 
}

void endOfLine() {
	std::cout << "End of line: " << yylineno++ << std::endl;
}

void endOfFile() {
	std::cout << "This is the end!\n";
}

void unrecognized() {
	std::cout << "************UNRECOGNIZED CHARACTER: " << yytext << "************\n";

	yyerror("Scanner: Invalid Character.");
}