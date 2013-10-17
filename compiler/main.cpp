
#include "cpsl.h"

int yyparse();

bool verbose = false;

int main(int argc, char** argv){
	++argv, --argc;  /* skip over program name */
	if ( argv[0] && std::string(argv[0]) == "-v") {
		verbose = true;
		++argv, --argc;
	}
	if ( argc > 0 )
	     freopen( argv[0], "r", stdin );

	yyparse();

	return 0;
}

