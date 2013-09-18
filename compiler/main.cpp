
#include "heading.h"

int yyparse();

int main(int argc, char** argv){
	++argv, --argc;  /* skip over program name */
	if ( argc > 0 )
	     freopen( argv[0], "r", stdin );

	yyparse();

	return 0;
}