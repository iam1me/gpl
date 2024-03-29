/** gpl.y
 ** A bison script for producing a parser that can
 ** process a program written in the Game Programming
 ** Language (GPL). 
 **/

%{  // bison syntax to indicate the start of the header
    // the header is copied directly into y.tab.c (the generated parser)

extern int yylex();               // this lexer function returns next token
extern int yyerror(const char *); // used to print errors
extern int line_count;            // current line in the input; from record.l

#include "error.h"      // class for printing errors (used by gpl)
#include "parser.h"
#include "gpl_type.h"
#include "symbol.h"
#include "symbol_table.h"
#include <iostream>
#include <string>

Error error_handler;

#define INSERT_SYMBOL(type, name)\
	Symbol* pSymbol = NULL;\
	switch(type)\
	{\
		case INT:\
			pSymbol = new Symbol(name, 42);\
			break;\
		\
		case DOUBLE:\
			pSymbol = new Symbol(name, 3.14159);\
			break;\
		\
		case STRING:\
		{\
			pSymbol = new Symbol(name, std::string("Hello world"));\
			break;\
		}\
		default:\
			std::string err = "Failed to Insert Symbol. Unrecognized Type: ";\
			err += gpl_type_to_string(type);\
			throw std::runtime_error(err);\
			break;\
	}\
	\
	bool result = Symbol_table::instance()->insert_symbol(pSymbol);\
	if(!result)\
	{\
		error_handler.error(Error::PREVIOUSLY_DECLARED_VARIABLE, name);\
	}\


bool is_symbol_defined(std::string name)
{
	const Symbol* pSymbol = Symbol_table::instance()->find_symbol(name);
	if(pSymbol) return true;

	// Check for the array version of this symbol
	name += "[0]";
	pSymbol = Symbol_table::instance()->find_symbol(name);
	return !!pSymbol;
}

	
%} 

/*********************************************
	T O K E N  D E C L A R A T I O N S
*********************************************/
%error-verbose

%union 
{
	char           union_char;
	int            union_int;
	double         union_double;

	// Use the new shared_ptr class instead!
	std::string    *union_string;  // MUST be a pointer to a string (this sucks!)

	Gpl_type	union_type;
}

%token <union_type> T_INT                 "int"
%token <union_type> T_DOUBLE              "double"
%token <union_type> T_STRING              "string"
%token T_TRIANGLE            "triangle"
%token T_PIXMAP              "pixmap"
%token T_CIRCLE              "circle"
%token T_RECTANGLE           "rectangle"
%token T_TEXTBOX             "textbox"
%token <union_int> T_FORWARD "forward" // value is line number
%token T_INITIALIZATION      "initialization" 

%token <union_string>	T_ID			"identifier"
%token <union_int> 	T_INT_CONSTANT		"int constant"
%token <union_double>	T_DOUBLE_CONSTANT	"double constant"
%token <union_string>	T_STRING_CONSTANT	"string constant"

%token T_TRUE                "true"
%token T_FALSE               "false"

%token T_ON                  "on"
%token T_SPACE               "space"
%token T_LEFTARROW           "leftarrow"
%token T_RIGHTARROW          "rightarrow"
%token T_UPARROW             "uparrow"
%token T_DOWNARROW           "downarrow"
%token T_LEFTMOUSE_DOWN      "leftmouse_down"
%token T_MIDDLEMOUSE_DOWN    "middlemouse_down"
%token T_RIGHTMOUSE_DOWN     "rightmouse_down"
%token T_LEFTMOUSE_UP        "leftmouse_up"
%token T_MIDDLEMOUSE_UP      "middlemouse_up"
%token T_RIGHTMOUSE_UP       "rightmouse_up"
%token T_MOUSE_MOVE          "mouse_move"
%token T_MOUSE_DRAG          "mouse_drag"
%token T_F1                  "f1"
%token T_AKEY                "akey"
%token T_SKEY                "skey"
%token T_DKEY                "dkey"
%token T_FKEY                "fkey"
%token T_HKEY                "hkey"
%token T_JKEY                "jkey"
%token T_KKEY                "kkey"
%token T_LKEY                "lkey"
%token T_WKEY                "wkey"


%token T_ANIMATION           "animation"


%token T_LBRACE              "{"
%token T_RBRACE              "}"
%token T_SEMIC               ";"
%token T_COMMA               ","

%nonassoc T_SIN                 "sin"
%nonassoc T_COS                 "cos"
%nonassoc T_TAN                 "tan"
%nonassoc T_ASIN                "asin"
%nonassoc T_ACOS                "acos"
%nonassoc T_ATAN                "atan"
%nonassoc T_SQRT                "sqrt"
%nonassoc T_FLOOR               "floor"
%nonassoc T_ABS                 "abs"
%nonassoc T_RANDOM              "random"
%nonassoc <union_int> T_PRINT           "print" // value is line number
%nonassoc <union_int> T_EXIT            "exit" // value is line number

/********************************************************
	Assignment Operators
	Precedence Level 1. Group to the right.
	Example: "a = b = c" should be evaluated as
		 "a = (b = c)"
********************************************************/
%precedence ASSIGN_OPS
%right T_ASSIGN              "="
%right T_PLUS_ASSIGN         "+="
%right T_MINUS_ASSIGN        "-="

/********************************************************
	Logic Operators
	Precedence Level 2. Group to the left.
	Example: "a && b || c" ==> "(a && b) || c"
********************************************************/
%precedence LOGIC_COMPARE_OPS
%left T_AND                 "&&"
%left T_OR                  "||"

/********************************************************
	GPL Object Comparison Functions
	Precedence Level 3. Non-Associative.
	These operators cannot be chained.
	Example: "objA touches objB" is OK
		but "objA touches objB near objC" is
		not supported.
********************************************************/
%precedence OBJECT_COMPARE_OPS
%nonassoc T_TOUCHES             "touches"
%nonassoc T_NEAR                "near"

/********************************************************
	Comparison Operators
	Precedence Level 4. Group to the left.
	Example: "a > b <= c" ==> "(a > b) <= c"
********************************************************/
%precedence MATH_COMPARE_OPS
%token T_LESS                "<"
%token T_GREATER             ">"
%token T_LESS_EQUAL          "<="
%token T_GREATER_EQUAL       ">="
%token T_EQUAL               "=="
%token T_NOT_EQUAL           "!="
%left T_LESS T_GREATER T_LESS_EQUAL T_GREATER_EQUAL T_EQUAL T_NOT_EQUAL

/********************************************************
	Simple Mathematical Operators
	Precedence Level 5. Group to the left.
	Example "a + b - c" ==> "(a + b) - c"
********************************************************/
%left MATH_SIMPLE_OPS
%token T_PLUS  "+"
%token T_MINUS "-"
%left T_PLUS T_MINUS

/********************************************************
	Complex Mathematical Operators
	Precedence Level 6. Group to the left.
	Example: "a * b / c" ==> "(a * b) / c"
********************************************************/
%precedence MATH_COMPLEX_OPS
%token T_ASTERISK            "*"
%token T_DIVIDE              "/"
%token T_MOD                 "%"
%left T_ASTERISK T_DIVIDE T_MOD


/********************************************************
	Unary Operators
	Precedence Level 6.
********************************************************/
%precedence UNARY_OPS
%token T_PLUS_PLUS           "++"
%token T_MINUS_MINUS         "--"
%token T_NOT                 "!"
%nonassoc T_PLUS_PLUS T_MINUS_MINUS T_NOT

/********************************************************
	Scope Operators
	Precedence Level 7
********************************************************/
%precedence SCOPE_OPS
%token T_PERIOD "."

/********************************************************
	Parentheses, Function Calls, Array Indeces
	Precendence Level 8.
********************************************************/
%precedence SUB_EXPR_OPS
%token T_LPAREN              "("
%token T_RPAREN              ")"
%token T_LBRACKET            "["
%token T_RBRACKET            "]"

/*******************************************************
	We need to setup the associativity for
	if.. then .. else statements. These can be 
	nested. 

	A if statement with an else has higher precedence
	than an if statement without an else
*******************************************************/
%precedence IF_NO_ELSE		"if-then"
%precedence IF_ELSE		"if-then-else"
%token T_IF                  "if"
%token T_FOR                 "for"
%token T_ELSE                "else"
%nonassoc T_IF T_FOR
%nonassoc T_ELSE

// special token that does not match any production
// used for characters that are not part of the language
%token T_ERROR               "error"

%type <union_type> simple_type

%%
/*********************************************
	G R A M M A R  R U L E S
*********************************************/

//---------------------------------------------------------------------
program:
    declaration_list block_list
    ;

//---------------------------------------------------------------------
declaration_list:
    declaration_list declaration
    | empty
    ;

//---------------------------------------------------------------------
declaration:
    variable_declaration T_SEMIC
    | object_declaration T_SEMIC
    | forward_declaration T_SEMIC
    ;

//---------------------------------------------------------------------
variable_declaration:
    simple_type  T_ID  optional_initializer
	{
		if(is_symbol_defined(*$2))
		{
			error_handler.error(Error::PREVIOUSLY_DECLARED_VARIABLE, *$2);
		}
		else
		{		
			INSERT_SYMBOL($1, *$2);
		}
		delete $2; // free ID
	}

/* Temporarily simplifying this one to only accept an integer for p4.
    | simple_type  T_ID  T_LBRACKET expression T_RBRACKET
*/

    | simple_type T_ID T_LBRACKET T_INT_CONSTANT T_RBRACKET
	{
		if(is_symbol_defined(*$2))
		{
			error_handler.error(Error::PREVIOUSLY_DECLARED_VARIABLE, *$2);
		}
		else
		{
			for(int i = 0; i < $4; i++)
			{
				std::string name = *$2 + "[";
				name += std::to_string(i) + "]";

				INSERT_SYMBOL($1, name);
			}
		}
		delete $2; // free ID
	}
    ;

//---------------------------------------------------------------------
simple_type:
    T_INT
	{
		$$ = INT;
	}
    | T_DOUBLE
	{
		$$ = DOUBLE;
	}
    | T_STRING
	{
		$$ = STRING;
	}
    ;

//---------------------------------------------------------------------
optional_initializer:
    T_ASSIGN expression
    | empty
    ;

//---------------------------------------------------------------------
object_declaration:
    object_type T_ID T_LPAREN parameter_list_or_empty T_RPAREN
    | object_type T_ID T_LBRACKET expression T_RBRACKET
    ;

//---------------------------------------------------------------------
object_type:
    T_TRIANGLE
    | T_PIXMAP
    | T_CIRCLE
    | T_RECTANGLE
    | T_TEXTBOX
    ;

//---------------------------------------------------------------------
parameter_list_or_empty :
    parameter_list
    | empty
    ;

//---------------------------------------------------------------------
parameter_list :
    parameter_list T_COMMA parameter
    | parameter
    ;

//---------------------------------------------------------------------
parameter:
    T_ID T_ASSIGN expression
	{
		//free the ID
		delete $1;
	}
    ;

//---------------------------------------------------------------------
forward_declaration:
    T_FORWARD T_ANIMATION T_ID T_LPAREN animation_parameter T_RPAREN
	{
		//free the ID
		delete $3;
	}
    ;

//---------------------------------------------------------------------
block_list:
    block_list block
    | empty
    ;

//---------------------------------------------------------------------
block:
    initialization_block
    | animation_block
    | on_block
    ;

//---------------------------------------------------------------------
initialization_block:
    T_INITIALIZATION statement_block
    ;

//---------------------------------------------------------------------
animation_block:
    T_ANIMATION T_ID T_LPAREN check_animation_parameter T_RPAREN T_LBRACE { } statement_list T_RBRACE end_of_statement_block
	{
		//free the ID
		delete $2;
	}
    ;

//---------------------------------------------------------------------
animation_parameter:
    object_type T_ID
	{
		//free the ID
		delete $2;
	}
    ;

//---------------------------------------------------------------------
check_animation_parameter:
    T_TRIANGLE T_ID
	{
		delete $2; // free the ID
	}
    | T_PIXMAP T_ID
	{
		delete $2; // free the ID
	}
    | T_CIRCLE T_ID
	{
		delete $2; // free the ID
	}
    | T_RECTANGLE T_ID
	{
		delete $2; // free the ID
	}
    | T_TEXTBOX T_ID
	{
		delete $2; // free the ID
	}
    ;

//---------------------------------------------------------------------
on_block:
    T_ON keystroke statement_block
    ;

//---------------------------------------------------------------------
keystroke:
    T_SPACE
    | T_UPARROW
    | T_DOWNARROW
    | T_LEFTARROW
    | T_RIGHTARROW
    | T_LEFTMOUSE_DOWN
    | T_MIDDLEMOUSE_DOWN
    | T_RIGHTMOUSE_DOWN
    | T_LEFTMOUSE_UP
    | T_MIDDLEMOUSE_UP
    | T_RIGHTMOUSE_UP
    | T_MOUSE_MOVE
    | T_MOUSE_DRAG
    | T_AKEY 
    | T_SKEY 
    | T_DKEY 
    | T_FKEY 
    | T_HKEY 
    | T_JKEY 
    | T_KKEY 
    | T_LKEY 
    | T_WKEY 
    | T_F1
    ;

//---------------------------------------------------------------------
if_block:
    statement_block_creator statement end_of_statement_block
    | statement_block
    ;

//---------------------------------------------------------------------
statement_block:
    T_LBRACE statement_block_creator statement_list T_RBRACE end_of_statement_block
    ;

//---------------------------------------------------------------------
statement_block_creator:
    // this goes to nothing so that you can put an action here in p7
    ;

//---------------------------------------------------------------------
end_of_statement_block:
    // this goes to nothing so that you can put an action here in p7
    ;

//---------------------------------------------------------------------
statement_list:
    statement_list statement
    | empty
    ;

//---------------------------------------------------------------------
statement:
    if_statement
    | for_statement
    | assign_statement T_SEMIC
    | print_statement T_SEMIC
    | exit_statement T_SEMIC
    ;

//---------------------------------------------------------------------
if_statement:
    T_IF T_LPAREN expression T_RPAREN if_block %prec IF_NO_ELSE
    | T_IF T_LPAREN expression T_RPAREN if_block T_ELSE if_block %prec IF_ELSE
    ;

//---------------------------------------------------------------------
for_statement:
    T_FOR T_LPAREN statement_block_creator assign_statement end_of_statement_block T_SEMIC expression T_SEMIC statement_block_creator assign_statement end_of_statement_block T_RPAREN statement_block
    ;

//---------------------------------------------------------------------
print_statement:
    T_PRINT T_LPAREN expression T_RPAREN %prec SUB_EXPR_OPS
    ;

//---------------------------------------------------------------------
exit_statement:
    T_EXIT T_LPAREN expression T_RPAREN %prec SUB_EXPR_OPS
    ;

//---------------------------------------------------------------------
assign_statement:
    variable T_ASSIGN expression %prec ASSIGN_OPS
    | variable T_PLUS_ASSIGN expression %prec ASSIGN_OPS
    | variable T_MINUS_ASSIGN expression %prec ASSIGN_OPS
    ;

//---------------------------------------------------------------------
variable:
    T_ID
    | T_ID T_LBRACKET expression T_RBRACKET
	{
		delete $1; // free the ID
	}
    | T_ID T_PERIOD T_ID
	{
		delete $1; // free the ID
		delete $3; // free the ID
	}
    | T_ID T_LBRACKET expression T_RBRACKET T_PERIOD T_ID
	{
		delete $1; // free the ID
		delete $6; // free the ID
	}
    ;


//---------------------------------------------------------------------
expression:
    primary_expression
    | expression T_OR expression %prec LOGIC_COMPARE_OPS
    | expression T_AND expression %prec LOGIC_COMPARE_OPS
    | expression T_LESS_EQUAL expression %prec MATH_COMPARE_OPS
    | expression T_GREATER_EQUAL  expression %prec MATH_COMPARE_OPS
    | expression T_LESS expression  %prec MATH_COMPARE_OPS
    | expression T_GREATER  expression %prec MATH_COMPARE_OPS
    | expression T_EQUAL expression %prec MATH_COMPARE_OPS
    | expression T_NOT_EQUAL expression %prec MATH_COMPARE_OPS
    | expression T_PLUS expression  %prec MATH_SIMPLE_OPS
    | expression T_MINUS expression %prec MATH_SIMPLE_OPS
    | expression T_ASTERISK expression %prec MATH_COMPLEX_OPS
    | expression T_DIVIDE expression %prec MATH_COMPLEX_OPS
    | expression T_MOD expression %prec MATH_COMPLEX_OPS
    | T_MINUS  expression %prec UNARY_OPS
    | T_NOT  expression %prec UNARY_OPS
    | math_operator T_LPAREN expression T_RPAREN %prec SUB_EXPR_OPS
    | variable geometric_operator variable %prec OBJECT_COMPARE_OPS
    ;

//---------------------------------------------------------------------
primary_expression:
    T_LPAREN  expression T_RPAREN
    | variable
    | T_INT_CONSTANT
    | T_TRUE
    | T_FALSE
    | T_DOUBLE_CONSTANT
    | T_STRING_CONSTANT
	{
		delete $1;
	}
    ;

//---------------------------------------------------------------------
geometric_operator:
    T_TOUCHES
    | T_NEAR
    ;

//---------------------------------------------------------------------
math_operator:
    T_SIN
    | T_COS
    | T_TAN
    | T_ASIN
    | T_ACOS
    | T_ATAN
    | T_SQRT
    | T_ABS
    | T_FLOOR
    | T_RANDOM
    ;

//---------------------------------------------------------------------
empty:
    // empty goes to nothing so that you can use empty in productions
    // when you want a production to go to nothing
    ;

%%
/*********************************************
	C++  I M P L E M E N T A T I O N
*********************************************/


