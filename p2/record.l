/*
  Use the following global variable to retrieve the text of the current 
  symbol (the one just matched):

    char *yytext

  Use the following global variable to send information to the parser
      
    extern YYSTYPE yylval;

  The union YYSTYPE is the union declared in record.y.
  It is defined in y.tab.h (which is included by parser.h).
  You can use a union just like a struct (which is a lot like a class),
  except that only one member variable can be used at a time.
  y.tab.h is generated by bison from record.y.

*/

/* the %{ indicates the start of C/C++ code section */
/* you can put whatever C/C++ code in here that you want */
/* this code is copied to the generated file */
%{

#include "error.h"    // gpl file, using now to get use to it
#include "parser.h"   // gpl file, using now to get use to it

// #include <ctype.h>
#include <iostream>
using namespace std;

Error error_handler; // error reporting object

// The following variable keeps track of what input line flex is reading.
// It will be used for reporting the line numbers of errors.
// 
// It will be used by other parts of the program via the "extern" construct
//   If another .cpp file declares "extern int line_count" that line_count
//   in that file will be the line_count in this file (it becomes global)
int line_count = 1;

void get_char_value()
{
	// assumes that the string in yytext is one character long
	// put the first character into the global union used to send values to the parser
	// the union and its union_char field are declared in record.y
	yylval.union_char = yytext[0];
}

void get_int_value()
{
  // assumes that the string in yytext is an integer
  // convert the string yytext into an integer
  // put the int into global union used to send values to the parser (record.y)
  // the union and its union_int field are declared in record.y
  yylval.union_int = atoi(yytext);
}

void get_double_value()
{
	// assumes that the string in yytext is a double
	// convert the string yytext into a double
	// put the double into global union used to send values to the parser
	// the union and its union_double field are declared in record.y
	yylval.union_double = atof(yytext);
}
 

void get_string_value()
{
  // Would like to put a string in the union, but the way the union works
  // it has to be a pointer to a string.
  // This is a potential memory leak.  Think carefully about where and when
  // this memory should be deleted.
  yylval.union_string = new string(yytext);
}

void get_string_constant()
{
  // Need to get the string constant in the form: "<string_constant>"
  // We want top get the substring of yytext, keeping all but the first and last characters
  int total_len = strlen(yytext);
  char temp[total_len - 1]; //minus 2 for the quotes, +1 for the ending \0
  memcpy(temp, yytext + 1, total_len -2);
  temp[total_len -2] = '\0';

  yylval.union_string = new string(temp);
}

void error()
{
  // have the error handling object report the problem
  // note: it uses line_count as a global variable
  error_handler.error(Error::ILLEGAL_TOKEN, yytext);

  // create a string to hold the current text
  // put that string in the yylval union so it can be used by parser (.y file)
  yylval.union_string = new string(yytext);
}


// the %} below indicates the end of the C/C++ code section

%}

/*
The %% below indicates the start of the rules flex/lex will use
The left-hand-side (LHS) specifies a regular expression
The right-hand-side (RHS) specifies an action (code to execute) when
  the regular expression is matched.
*/

%%

[\n]            line_count++; // count the lines for error reporting
[ \t\r]         ; // ignore spaces, tabs and the microsoft line feed (\r)

"record"        return T_RECORD;
"="             return T_ASSIGN;
"{"             return T_LBRACE;
"}"             return T_RBRACE;
";"             return T_SEMIC;


[0-9]+		{
		get_int_value();
		return T_INT_CONSTANT;
	}

([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+) 	{
		get_double_value(); 
		return T_DOUBLE_CONSTANT;
	}

(JAN)|(FEB)|(MAR)|(APR)|(MAY)|(JUN)|(JUL)|(AUG)|(SEP)|(NOV)|(DEC)	{
		get_string_value();
		return T_MONTH;
	}

[A-Za-z]+[_A-Za-z0-9]* 	{
	       get_string_value(); 
		return T_ID;
	}

\"([^"]|\\\")*\"	{
		get_string_constant();
		return T_STRING_CONSTANT;
	}

,	{
		return T_COMMA;
	}

.	{
		error();
		return T_ERROR;
	}
