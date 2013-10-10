
#include "cpsl.h"

int yyparse();

int main(int argc, char** argv){
	++argv, --argc;  /* skip over program name */
	if ( argc > 0 )
	     freopen( argv[0], "r", stdin );

	yyparse();

	return 0;
}

int yyerror(const char *s)
{
	extern int yylineno;  
    extern char *yytext; 
    
	std::cout << "ERROR: " << s << " at symbol \"" << yytext;
    std::cout << "\" on line " << yylineno << std::endl;

    exit(1);
    
    return 1;
}