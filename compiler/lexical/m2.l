/* scanner for a toy Pascal-like language */

%{
/* need this for the call to atof() below */
#include <math.h>
%}

%%

ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
BEGIN|begin 		{ printf("Keyword: BEGIN (%s)", yytext);}
CHR|chr 	 		{ printf("Keyword: CHR (%s)", yytext);}
CONST|const 		{ printf("Keyword: CONST (%s)", yytext);}
DO|do 				{ printf("Keyword: DO (%s)", yytext);}
DOWNTO|downto 		{ printf("Keyword: DOWNTO (%s)", yytext);}
ELSE|else 	 		{ printf("Keyword: ELSE (%s)", yytext);}
ELSEIF|elseif 		{ printf("Keyword: ELSEIF (%s)", yytext);}
END|end 	 		{ printf("Keyword: END (%s)", yytext);}
FOR|for 	 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}
ARRAY|array 		{ printf("Keyword: ARRAY (%s)", yytext);}

else|if|record|to|begin|elseif|of 	{ printf("Identifier: %s", yytext); }

repeat|type|chr|end|ord|return|until		{ printf("Identifier: %s", yytext); }

const|for|pred|stop|var|do|forward 			{ printf("Identifier: %s", yytext); }

procedure|succ|while|downto|function		{ printf("Identifier: %s", yytext); }

read|then|write								{ printf("Identifier: %s", yytext); }




{DIGIT}+    		{ printf( "An integer: %s (%d)\n", yytext, atoi( yytext ) ); }

{DIGIT}+"."{DIGIT}*        {
         printf( "A float: %s (%g)\n", yytext,
                 atof( yytext ) );
         }

if|then|begin|end|procedure|function        {
         printf( "A keyword: %s\n", yytext );
         }

{ID}        printf( "An identifier: %s\n", yytext );

"+"|"-"|"*"|"/"   printf( "An operator: %s\n", yytext );

"{"[\^{}}\n]*"}"     /* eat up one-line comments */

[ \t\n]+          /* eat up whitespace */

.           printf( "Unrecognized character: %s\n", yytext );

%%

int yywrap() {
	printf("We'er done!");
	return 0;
}

int main( int argc, char **argv ) {
	++argv, --argc;  /* skip over program name */
	if ( argc > 0 )
	     yyin = fopen( argv[0], "r" );
	else
	     yyin = stdin;

	yylex();
}