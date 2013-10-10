/* scanner for CPSL written by Alan Christensen */

%{
#include "cpsl.h"
#include "cpsl.tab.h"

extern YYSTYPE yylval;

int yyLineCount = 1;

bool verboseLex = false;


void keyword(std::string word);
char* identifier(std::string ident);
void oper();
int octal();
int hex();
int decimal();
char* charConst();
char* strConst();
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


integer					{ yylval.str_val = identifier("integer"); 	return IDENT_SYM;}
char					{ yylval.str_val = identifier("char"); 		return IDENT_SYM;}
boolean					{ yylval.str_val = identifier("boolean");	return IDENT_SYM;}
string	 				{ yylval.str_val = identifier("string"); 	return IDENT_SYM;}
true					{ yylval.str_val = identifier("true"); 		return IDENT_SYM;}
false					{ yylval.str_val = identifier("false"); 	return IDENT_SYM;}

[a-zA-Z][a-zA-Z0-9_]*	{ yylval.str_val = identifier("variable"); 	return IDENT_SYM;}


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


0[0-7]+					{ yylval.int_val = octal(); 	return INT_CONST_SYM;}
0x[0-9a-fA-f]+			{ yylval.int_val = hex(); 		return INT_CONST_SYM;} 
[0-9]+					{ yylval.int_val = decimal(); 	return INT_CONST_SYM;} 


'[ -~]'					{ yylval.str_val = charConst();	return CHAR_CONST_SYM;}
'\\n'|'\\r'|'\\b'		{ yylval.str_val = charConst();	return CHAR_CONST_SYM;}
'\\t'|'\\f'				{ yylval.str_val = charConst();	return CHAR_CONST_SYM;}

["][ !#-~]*["]			{ yylval.str_val = strConst(); 	return STR_CONST_SYM;}


$.*						{ comment(); }


[ \t]*					/* eat up whitespace */

[\n]					{ endOfLine(); }


<<EOF>>					{ endOfFile(); 	return 0;}


.						{ unrecognized(); }

%%

int yywrap() {
	return 1;
}

// int main( int argc, char **argv ) {
// 	++argv, --argc;  
// 	if ( argc > 0 )
// 	     yyin = fopen( argv[0], "r" );
// 	else
// 	     yyin = stdin;

// 	yylex();
// }

void keyword(std::string word) {
	if(verboseLex) {
		std::cout << "Keyword: " << word << " (" << yytext << ")" << std::endl;
	}
}

char* identifier(std::string ident) {
	if(verboseLex) {
		std::cout << "Identifier: " << ident << " (" << yytext << ")" << std::endl;
	}

	std::string label(yytext);
	
	return strcpy(new char[label.size()], label.c_str());
}

void oper() {
	if(verboseLex) {
		std::cout << "Operator: " << yytext << std::endl;
	}
}

int octal() {
	if(verboseLex) {
		std::cout << "Octal Number: " << yytext << std::endl;
	}

	return std::stoi(yytext, nullptr, 8);
}
int hex() {
	if(verboseLex) {
		std::cout << "Hex Number: " << yytext << std::endl;
	}

	return std::stoi(yytext, nullptr, 16);
}
int decimal() {
	if(verboseLex) {
		std::cout << "Decimal Number: " << yytext << std::endl;
	}

	return std::stoi(yytext, nullptr, 10);
}

char* charConst() {
	if(verboseLex) {
		std::cout << "Character Constant: " << yytext << std::endl;
	}

	std::string text(yytext);

	return strcpy(new char[text.size()], text.c_str());
}
char* strConst() {
	if(verboseLex) {
		std::cout << "String Constant: " << yytext << std::endl;
	}

	std::string text(yytext);

	return strcpy(new char[text.size()], text.c_str());
}

void comment() {
	if(verboseLex) {
		std::cout << "Comment: " << yytext << std::endl; 
	}
}

void endOfLine() {
	if(verboseLex) {
		std::cout << "End of line: " << yylineno++ << std::endl;
	}
}

void endOfFile() {
	if(verboseLex) {
		std::cout << "This is the end!\n";
	}
}

void unrecognized() {
	if(verboseLex) {
		std::cout << "************UNRECOGNIZED CHARACTER: " << yytext << "************\n";
	}

	yyerror("Scanner: Invalid Character.");
}