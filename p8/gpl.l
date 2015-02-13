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
#include "gpl_type.h"

int line_count = 1;
extern Error error_handler;


void capture_int_const()
{
	yylval.union_int = atoi(yytext);
}

void capture_double_const()
{
	yylval.union_double = atof(yytext);
}	

void capture_token()
{
	yylval.union_string = new std::string(yytext);
}

void capture_string_const()
{
	int token_len = strlen(yytext);
	char temp[token_len -1];

	memcpy(temp, yytext + 1, token_len -2);
	temp[token_len-2] = '\0';

	yylval.union_string = new std::string(temp);
}

void handle_illegal_token()
{
	error_handler.error(Error::ILLEGAL_TOKEN, yytext);
	capture_token();
}

%}

/*********************************************
	L E X E R  R U L E S
*********************************************/
%%

[\n]		{
			line_count++;
		}

[ \t\r]		{
			//Ignore spaces, tabs, and the MS line feed
		}

	/*************************************
		GPL Built-in Types
	*************************************/
"int"			{
				yylval.union_type = INT;
				return T_INT;
			}

"double"		{
				yylval.union_type = DOUBLE;
				return T_DOUBLE;
			}

"string"		{
				yylval.union_type = STRING;
				return T_STRING;
			}

"triangle"		{
				return T_TRIANGLE;
			}

"pixmap"		{
				return T_PIXMAP;
			}

"circle"		{
				return T_CIRCLE;
			}

"rectangle"		{
				return T_RECTANGLE;
			}

"textbox"		{
				return T_TEXTBOX;
			}

"forward"		{
				return T_FORWARD;
			}

"initialization"	{
				return T_INITIALIZATION;
			}

	/*************************************
		GPL Event Keywords
	*************************************/
"on"			{
				return T_ON;
			}

"space"			{
				return T_SPACE;
			}

"leftarrow"		{
				return T_LEFTARROW;
			}

"rightarrow"		{
				return T_RIGHTARROW;
			}

"uparrow"		{
				return T_UPARROW;
			}

"downarrow"		{
				return T_DOWNARROW;
			}

"leftmouse_down"	{
				return T_LEFTMOUSE_DOWN;
			}

"leftmouse_up"		{
				return T_LEFTMOUSE_UP;
			}

"middlemouse_down"	{
				return T_MIDDLEMOUSE_DOWN;
			}

"middlemouse_up"	{
				return T_MIDDLEMOUSE_UP;
			}

"rightmouse_down"	{
				return T_RIGHTMOUSE_DOWN;
			}

"rightmouse_up"		{
				return T_RIGHTMOUSE_UP;
			}

"mouse_move"		{
				return T_MOUSE_MOVE;
			}

"mouse_drag"		{
				return T_MOUSE_DRAG;
			}

"f1"			{
				return T_F1;
			}

"akey"			{
				return T_AKEY;
			}

"skey"			{
				return T_SKEY;
			}

"dkey"			{
				return T_DKEY;
			}

"fkey"			{
				return T_FKEY;
			}

"hkey"			{
				return T_HKEY;
			}

"jkey"			{
				return T_JKEY;
			}

"kkey"			{
				return T_KKEY;
			}

"lkey"			{
				return T_LKEY;
			}

"wkey"			{
				return T_WKEY;
			}

	/*************************************
		GPL Language Constructs
	*************************************/
"animation"	{
			return T_ANIMATION;
		}

"if"		{
			return T_IF;
		}

"for"		{
			return T_FOR;
		}

"else"		{
			return T_ELSE;
		}

"//".*		{
			// This is a comment. Ignore it.	
		}

	/**************************************
		GPL Built-in Functions
	**************************************/
"print"		{
			/** The print statement makes use of the line # **/
			yylval.union_int = line_count;
			return T_PRINT;
		}

"exit"		{
			/** The exit statement makes use of the line # **/
			yylval.union_int = line_count;
			return T_EXIT;
		}

"sin"		{
			return T_SIN;
		}

"cos"		{
			return T_COS;
		}

"tan"		{
			return T_TAN;
		}

"asin"		{
			return T_ASIN;
		}

"acos"		{
			return T_ACOS;
		}

"atan"		{
			return T_ATAN;
		}

"sqrt"		{
			return T_SQRT;
		}

"floor"		{
			return T_FLOOR;
		}

"abs"		{
			return T_ABS;
		}

"random"	{
			return T_RANDOM;
		}

	/***************************************
		GPL Operators
	***************************************/
"touches"	{
			return T_TOUCHES;
		}

"near"		{
			return T_NEAR;
		}

"="		{
			return T_ASSIGN;
		}

"+="		{
			return T_PLUS_ASSIGN;
		}

"-="		{
			return T_MINUS_ASSIGN;
		}

"*"		{
			return T_ASTERISK;
		}

"/"		{
			return T_DIVIDE;
		}

"%"		{
			return T_MOD;
		}

"+"		{
			return T_PLUS;
		}

"-"		{
			return T_MINUS;
		}

"<"		{
			return T_LESS;
		}

">"		{
			return T_GREATER;
		}

"<="		{
			return T_LESS_EQUAL;
		}

">="		{
			return T_GREATER_EQUAL;
		}

"=="		{
			return T_EQUAL;
		}

"!="		{
			return T_NOT_EQUAL;
		}

"&&"		{
			return T_AND;
		}

"||"		{
			return T_OR;
		}

"!"		{
			return T_NOT;
		}

	/***************************************
		Basic Syntax Tokens
	***************************************/
"("		{
			return T_LPAREN;
		}

")"		{
			return T_RPAREN;
		}

"{"		{
			return T_LBRACE;
		}

"}"		{
			return T_RBRACE;
		}

"["		{
			return T_LBRACKET;
		}

"]"		{
			return T_RBRACKET;
		}

";"		{
			return T_SEMIC;
		}

","		{
			return T_COMMA;
		}

"."		{
			return T_PERIOD;
		}

	/*************************************
		GPL Value Parsing
	*************************************/
"true"			{
				return T_TRUE;
			}

"false"			{
				return T_FALSE;
			}

[a-zA-Z_]+[a-zA-Z0-9_]*	{
				capture_token();
				return T_ID;
			}

[0-9]+			{
				capture_int_const();
				return T_INT_CONSTANT;
			}

[0-9]+\.[0-9]*		{
				capture_double_const();
				return T_DOUBLE_CONSTANT;
			}

\.[0-9]+		{
				capture_double_const();
				return T_DOUBLE_CONSTANT;
			}

\"([^\"]|\\\")*\"	{
				capture_string_const();
				return T_STRING_CONSTANT;
			}

	/*************************************
		If we couldn't match it to 
		one of the rules above,
		the input must not be valid
		GPL. Error.
	*************************************/
.		{
			//TO DO: Error Handling
			handle_illegal_token();
			return T_ERROR;
		}
%%
/*********************************************
	O T H E R  C++  C O D E
*********************************************/
