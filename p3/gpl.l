/** gpl.l
 ** A flex script that provides the rules for
 ** tokenizing a program written in the Game
 ** Programming Language (GPL)
 **/

/********************************************
	C++ L E X E R  C O D E
********************************************/
%{
#include <iostream>
#include "error.h"
#include "parser.h"

/*#define YY_DECL \
	extern "C" int yylex()*/

int line_count = 0;
Error error_handler;


void capture_int()
{

}

void capture_double()
{

}

void capture_string()
{

}


%}

/*********************************************
	L E X E R  R U L E S
*********************************************/
%%




%%
/*********************************************
	O T H E R  C++  C O D E
*********************************************/
/*main()
{
	yylex();
}
*/
