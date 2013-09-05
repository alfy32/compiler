/* scanner for CPSL written by Alan Christensen */

%{

%}

%%
	
ARRAY|array   			{ printf( "Keyword: ARRAY (%s)\n", yytext);}
ELSE|else   			{ printf( "Keyword: ELSE (%s)\n", yytext);}
IF|if   				{ printf( "Keyword: IF (%s)\n", yytext);}
RECORD|record  			{ printf( "Keyword: RECORD (%s)\n", yytext);}
TO|to   				{ printf( "Keyword: TO (%s)\n", yytext);}
BEGIN|begin   			{ printf( "Keyword: BEGIN (%s)\n", yytext);}
ELSEIF|elseif   		{ printf( "Keyword: ELSEIF (%s)\n", yytext);}
OF|of   				{ printf( "Keyword: OF (%s)\n", yytext);}
REPEAT|repeat   		{ printf( "Keyword: REPEAT (%s)\n", yytext);}
TYPE|type   			{ printf( "Keyword: TYPE (%s)\n", yytext);}
CHR|chr   				{ printf( "Keyword: CHR (%s)\n", yytext);}
END|end   				{ printf( "Keyword: END (%s)\n", yytext);}
ORD|ord   				{ printf( "Keyword: ORD (%s)\n", yytext);}
RETURN|return   		{ printf( "Keyword: RETURN (%s)\n", yytext);}
UNTIL|until   			{ printf( "Keyword: UNTIL (%s)\n", yytext);}
CONST|const   			{ printf( "Keyword: CONST (%s)\n", yytext);}
FOR|for   				{ printf( "Keyword: FOR (%s)\n", yytext);}
PRED|pred   			{ printf( "Keyword: PRED (%s)\n", yytext);}
STOP|stop   			{ printf( "Keyword: STOP (%s)\n", yytext);}
VAR|var   				{ printf( "Keyword: VAR (%s)\n", yytext);}
DO|do   				{ printf( "Keyword: DO (%s)\n", yytext);}
FORWARD|forward   		{ printf( "Keyword: FORWARD (%s)\n", yytext);}
PROCEDURE|procedure 	{ printf( "Keyword: PROCEDURE (%s)\n", yytext);}
SUCC|succ   			{ printf( "Keyword: SUCC (%s)\n", yytext);}
WHILE|while   			{ printf( "Keyword: WHILE (%s)\n", yytext);}
DOWNTO|downto   		{ printf( "Keyword: DOWNTO (%s)\n", yytext);}
FUNCTION|function   	{ printf( "Keyword: FUNCTION (%s)\n", yytext);}
READ|read   			{ printf( "Keyword: READ (%s)\n", yytext);}
THEN|then   			{ printf( "Keyword: THEN (%s)\n", yytext);}
WRITE|write 			{ printf( "Keyword: WRITE (%s)\n", yytext);}


integer 				{ printf( "Identifier: integer (%s)\n", yytext);}
char 					{ printf( "Identifier: char (%s)\n", yytext);}
boolean 				{ printf( "Identifier: boolean (%s)\n", yytext);}
string	 				{ printf( "Identifier: string (%s)\n", yytext);}
true 					{ printf( "Identifier: true (%s)\n", yytext);}
false 					{ printf( "Identifier: false (%s)\n", yytext);}

[a-zA-Z][a-zA-Z0-9_]*	{ printf( "Identifier: (%s)\n", yytext);}


"+"|"-"|"*"|"/"			{ printf( "An operator: %s\n", yytext );}
"&"|"|"|"~"|"="			{ printf( "An operator: %s\n", yytext );}
"<>"|"<"|"<="|">"		{ printf( "An operator: %s\n", yytext );}
">="|"."|","|":"		{ printf( "An operator: %s\n", yytext );}
";"|"("|")"|"["			{ printf( "An operator: %s\n", yytext );}
"]"|":="|"%"			{ printf( "An operator: %s\n", yytext );}


0[0-7]+					{ printf( "An Octal Number: (%s)\n", yytext);}
0x[0-9a-fA-f]+			{ printf( "A Hex Number: (%s)\n", yytext);}
[0-9]+					{ printf( "A Decimal Number: (%s)\n", yytext);}


'[ -~]' 				{ printf( "A Character Constant: (%s)\n", yytext);}


["][ !#-~]*["]			{ printf( "A String Constant: (%s)\n", yytext);}


$.*						{ printf( "A Comment: (%s)\n", yytext);}


[ \t\n]+          		/* eat up whitespace */


<<EOF>>    				{ printf( "This is the end!\n", yytext); return 0;}


.           			{ printf( "************UNRECOGNIZED CHARACTER: (%s)************\n", yytext );}

%%

int yywrap() {
	return 1;
}

int main( int argc, char **argv ) {
	++argv, --argc;  /* skip over program name */
	if ( argc > 0 )
	     yyin = fopen( argv[0], "r" );
	else
	     yyin = stdin;

	yylex();
}