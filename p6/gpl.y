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

#include <iostream>
#include <string>
#include "error.h"      // class for printing errors (used by gpl)
#include "parser.h"
#include "gpl_type.h"
#include "gpl_exception.h"
#include "symbol.h"
#include "symbol_table.h"
#include "expression.h"
#include "value.h"
#include "triangle.h"
#include "rectangle.h"
#include "circle.h"
#include "pixmap.h"
#include "textbox.h"
%}

%union 
{
	char           		union_char;
	int            		union_int;
	double         		union_double;
	IExpression*   		union_expression;

	// Use the new shared_ptr class instead!
	std::string    		*union_string;  // MUST be a pointer to a string (this sucks!)

	Gpl_type		union_type;
	Game_object_type	union_object_type;
	Operator_type		union_operator;

	Parameter*		union_parameter;
	ParameterList*		union_parameter_list;
}

%{
Error error_handler;

std::shared_ptr<Symbol> InsertSymbol (std::string name, Gpl_type type,
			const std::shared_ptr<IValue>& pval)
{
	TRACE_VERBOSE("InsertSymbol()...")
	TRACE_VERBOSE("InsertSymbol('" << name << "', " << gpl_type_to_string(type) << ", " << pval->to_string())

	std::shared_ptr<Symbol> pSymbol(new Symbol(name, type, pval));
	if(!pSymbol)
	{
		TRACE_ERROR("Failed to construct Symbol")
		throw std::runtime_error("Failed to construct Symbol '" + name + "' of type: " + gpl_type_to_string(type));
	}

	TRACE_VERBOSE("Calling Symbol_table::insert_symbol...")
	bool result = Symbol_table::instance()->insert_symbol(pSymbol);
	if(!result)
	{
		throw previously_declared_variable(name);
	}

	return pSymbol;
}

// Declares a variable. Default value left up to the Symbol class
std::shared_ptr<Symbol> InsertSymbol(std::string name, Gpl_type type)
{
	TRACE_VERBOSE("InsertSymbol('" << name << "', " << gpl_type_to_string(type) << ")")

	std::shared_ptr<Symbol> pSymbol(new Symbol(name, type));
	bool result = Symbol_table::instance()->insert_symbol(pSymbol);
	if(!result)
	{
		throw previously_declared_variable(name);
	}

	return pSymbol;
}


bool is_symbol_defined(std::string name, bool* bIsArray)
{
	*bIsArray = false;
	std::shared_ptr<Symbol> pSymbol = Symbol_table::instance()->find_symbol(name);
	if(pSymbol) return true;

	// Check for the array version of this symbol
	*bIsArray = true;
	name += "[0]";
	pSymbol = Symbol_table::instance()->find_symbol(name);
	return !!pSymbol;
}

inline std::shared_ptr<Symbol> get_symbol(std::string name, bool* bIsArray)
{
	*bIsArray = false;
	std::shared_ptr<Symbol> pSymbol = Symbol_table::instance()->find_symbol(name);
	if(!!pSymbol) return pSymbol;

	*bIsArray = true;
	pSymbol = Symbol_table::instance()->find_symbol(name + "[0]");
	return pSymbol;
}

#define GPL_BEGIN_EXPR_BLOCK(block_name)\
	std::string __gpl_block_name = block_name;\
	TRACE_VERBOSE("GPL_BEGIN_EXPR_BLOCK '" << __gpl_block_name << "'")\
	try{

#define GPL_END_EXPR_BLOCK(ret_param)\
	} catch(const gpl_exception& ex) { \
		ex.write_exception();\
		std::shared_ptr<IValue> pval(new GPLVariant(0));\
		ret_param = new ValueExpression(pval);\
	} catch (const std::exception& ex) { \
		TRACE_ERROR("std::exception in '" << __gpl_block_name << "': "  << ex.what()) \
		error_handler.error(Error::UNDEFINED_ERROR, ex.what()); \
		YYABORT;\
	} catch (...) { \
		TRACE_ERROR("Unknown Exception in '" << __gpl_block_name << "'")\
		error_handler.error(Error::UNDEFINED_ERROR, "Unknown Exception");\
		YYABORT;\
	}\
	TRACE_VERBOSE("GPL_END_EXPR_BLOCK '" << __gpl_block_name << "' (Total Error Count: " \
		<< Error::num_errors() << ")")
	

#define GPL_BEGIN_DECL_BLOCK(block_name)\
	std::string __gpl_block_name = block_name;\
	TRACE_VERBOSE("GPL_BEGIN_DECL_BLOCK '" << __gpl_block_name << "'")\
	try{


#define GPL_END_DECL_BLOCK()\
	}catch(const gpl_exception& ex){\
		ex.write_exception();\
	}catch(const std::exception& ex){\
		TRACE_ERROR("what():" << ex.what())\
		YYABORT;\
	}catch(...){\
		TRACE_ERROR("Unknown Exception Encountered")\
		YYABORT;\
	}\
	TRACE_VERBOSE("GPL_END_DECL_BLOCK '" << __gpl_block_name << "' (Total Error Count: " \
		<< Error::num_errors() << ")")


// Classes used for encapsulating parse information

%} 

/*********************************************
	T O K E N  D E C L A R A T I O N S
*********************************************/
%error-verbose

%token <union_type> T_INT                 "int"
%token <union_type> T_DOUBLE              "double"
%token <union_type> T_STRING              "string"
%token T_TRIANGLE            "Triangle"
%token T_PIXMAP              "Pixmap"
%token T_CIRCLE              "Circle"
%token T_RECTANGLE           "Rectangle"
%token T_TEXTBOX             "Textbox"
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
%token T_ASSIGN              "="
%token T_PLUS_ASSIGN         "+="
%token T_MINUS_ASSIGN        "-="
%right T_ASSIGN T_PLUS_ASSIGN T_MINUS_ASSIGN

/********************************************************
	Logic Operators
	NOTE: And has higher precedence than Or
********************************************************/
%token T_OR "||"
%left T_OR 

%token T_AND "&&"
%left T_AND

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
%token T_EQUAL               "=="
%token T_NOT_EQUAL           "!="
%left  T_EQUAL T_NOT_EQUAL

%token T_LESS                "<"
%token T_GREATER             ">"
%token T_LESS_EQUAL          "<="
%token T_GREATER_EQUAL       ">="
%left T_LESS T_GREATER T_LESS_EQUAL T_GREATER_EQUAL


/********************************************************
	Simple Mathematical Operators
	Precedence Level 5. Group to the left.
	Example "a + b - c" ==> "(a + b) - c"
********************************************************/
%token T_PLUS  "+"
%token T_MINUS "-"
%left T_PLUS T_MINUS

/********************************************************
	Complex Mathematical Operators
	Precedence Level 6. Group to the left.
	Example: "a * b / c" ==> "(a * b) / c"
********************************************************/
%token T_ASTERISK            "*"
%token T_DIVIDE              "/"
%token T_MOD                 "mod"
%left T_ASTERISK T_DIVIDE T_MOD


/********************************************************
	Unary Operators
	Precedence Level 6.
********************************************************/
%token T_PLUS_PLUS           "++"
%token T_MINUS_MINUS         "--"
%token T_NOT                 "!"
%nonassoc T_PLUS_PLUS T_MINUS_MINUS T_NOT
%precedence UNARY_OPS

/********************************************************
	Scope Operators
	Precedence Level 7
********************************************************/
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
%type <union_object_type> object_type
%type <union_operator> geometric_operator
%type <union_operator> math_operator
%type <union_expression> variable
%type <union_expression> expression
%type <union_expression> primary_expression
%type <union_expression> optional_initializer
%type <union_parameter> parameter
%type <union_parameter_list> parameter_list
%type <union_parameter_list> parameter_list_or_empty
%type <union_string> animation_parameter

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
		GPL_BEGIN_DECL_BLOCK("variable_declaration[0]")
		std::string var_name(*$2);
		delete $2;

		TRACE_VERBOSE("Checking to see if the variable is already declared...")
		bool bIsArray;
		if(is_symbol_defined(var_name, &bIsArray))
		{
			throw previously_declared_variable(var_name);
		}

		TRACE_VERBOSE("Attempting to declare variable '" << var_name << "' of type '" << $1 << "'")
		if($3 != NULL)
		{
			TRACE_VERBOSE("Optional Initializer Found...")
			std::shared_ptr<IExpression> init_expr((IExpression*)$3);		

			TRACE_VERBOSE("Evaluating the Intializer Expression...")
			std::shared_ptr<IValue> init_val = init_expr->eval();
			Gpl_type init_type = init_val->get_type();

			TRACE_VERBOSE("Checking the Initial Value (Type: " + gpl_type_to_string(init_type) + ")...")
			bool bBadInitialValue = false;
			switch($1)
			{
				case INT:
					if(init_type != INT)
					{
						TRACE_ERROR("Mismatched Types. Defaulting to 0")
						bad_initial_value(var_name).write_exception();
						bBadInitialValue = true;
						init_val.reset(new GPLVariant(0));
					}
					break;

				case DOUBLE:
					if(!(init_type & (INT|DOUBLE)))
					{
						TRACE_ERROR("Mismatched Types. Defaulting to 0.0")
						bad_initial_value(var_name).write_exception();
						bBadInitialValue = true;
						init_val.reset(new GPLVariant(0.0));
					}
					break;

				case STRING:
					if(!(init_type & (INT|DOUBLE|STRING)))
					{
						TRACE_ERROR("Mismatched Types. Defaulting to \"\"")
						bad_initial_value(var_name).write_exception();
						bBadInitialValue = true;
						init_val.reset(new GPLVariant(""));
					}
					break;

				default:
					TRACE_ERROR("Unhandled Type!");
					throw std::logic_error("Unhandled type: " + gpl_type_to_string(init_type));
			}

			TRACE_VERBOSE("Inserting the Symbol...")
			InsertSymbol(var_name, $1, init_val);
		}
		else
		{
			TRACE_VERBOSE("No Initialization Expression Found")
			TRACE_VERBOSE("Registering Symbol: " << var_name << "(no initial value)")
			InsertSymbol(var_name, $1);
		}
		GPL_END_DECL_BLOCK()
	}

    | simple_type  T_ID  T_LBRACKET expression T_RBRACKET
	{
		GPL_BEGIN_DECL_BLOCK("variable_declaration[1]")

		std::string var_name(*$2);
		TRACE_VERBOSE("Checking to see if the symbol '" << var_name << "' is already defined")
		delete $2;

		bool bIsArray;
		if(is_symbol_defined(var_name, &bIsArray))
		{
			//error_handler.error(Error::PREVIOUSLY_DECLARED_VARIABLE, *$2);
			throw previously_declared_variable(var_name);
		}
		
		TRACE_VERBOSE("Checking to see if the Index is of the appropriate type...")
		std::shared_ptr<IExpression> ndx_expr($4);
		std::shared_ptr<IValue> ndx_val = ndx_expr->eval();

		int array_size;
		if(ndx_val->get_int(array_size) == CONVERSION_ERROR)
		{
			std::string str_val;
			ndx_val->get_string(str_val);
			throw invalid_array_size(var_name,str_val);
		}			
		else if(array_size <= 0)
		{
			throw invalid_array_size(var_name, std::to_string(array_size));
		}

		TRACE_VERBOSE("Creating the set of variables")
		for(int i = 0; i < array_size; i++)
		{
			std::string name = var_name + "[";
			name += std::to_string(i) + "]";

			std::shared_ptr<IValue> init_val(new GPLVariant(0));
			InsertSymbol(name, $1, init_val);
		}
		
		GPL_END_DECL_BLOCK()
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
	{
		$$ = $2;
	}
    | empty
	{
		$$ = NULL;
	}
    ;

//---------------------------------------------------------------------
object_declaration:
    object_type T_ID T_LPAREN parameter_list_or_empty T_RPAREN
	{
		GPL_BEGIN_DECL_BLOCK("object_declaration[0]")

		std::string var_name(*$2);	
		TRACE_VERBOSE("Object Name: " << var_name);
		delete $2;

		// Create the object 
		TRACE_VERBOSE("Creating the GameObject");
		std::shared_ptr<Game_object> pobj;
		switch($1)
		{
			case TRIANGLE:
				pobj.reset(new Triangle());
				break;
			case RECTANGLE:
				pobj.reset(new Rectangle());
				break;
			case CIRCLE:
				pobj.reset(new Circle());
				break;
			case PIXMAP:
				pobj.reset(new Pixmap());
				break;
			case TEXTBOX:
				pobj.reset(new Textbox());
				break;
			default:
				throw std::invalid_argument("Unrecognized Game Object Type");
		}

		// Process any Parameters
		ParameterList* pParams = ($4);
		if(pParams != NULL)
		{
			Gpl_type member_type;
			Status result;
			ConversionStatus conv_status;
			std::shared_ptr<Parameter> pcur;
			std::shared_ptr<IExpression> pexpr;
			for(ParameterList::iterator it = pParams->begin(); it != pParams->end(); it++)
			{
				pcur = *it;
				pexpr = pcur->get_expr();

				// Determine the Member Type
				result = pobj->get_member_variable_type(pcur->get_name(), member_type);
				if(result != OK)
				{
					TRACE_VERBOSE("Failed to retrieve type of member '" + pcur->get_name()
						+ "': " + status_to_string(result))
					
					unknown_parameter(game_object_type_to_string($1), pcur->get_name()).write_exception();
				}				
		
				std::shared_ptr<IValue> pval = pexpr->eval();
				switch(member_type)
				{
					case INT:
					{
						int temp;
						conv_status = pval->get_int(temp);
						if(conv_status == CONVERSION_ERROR) break;

						result = pobj->set_member_variable(pcur->get_name(),temp);
						break;
					}

					case DOUBLE:
					{
						double temp;
						conv_status = pval->get_double(temp);
						if(conv_status == CONVERSION_ERROR) break;

						result = pobj->set_member_variable(pcur->get_name(), temp);
						break;						
					}

					case STRING:
					{
						std::string temp;
						conv_status = pval->get_string(temp);
						if(conv_status == CONVERSION_ERROR) break;

						result = pobj->set_member_variable(pcur->get_name(), temp);
						break;	
					}

					case ANIMATION_BLOCK:
					{
						std::shared_ptr<Animation_block> temp;
						conv_status = pval->get_animation_block(temp);
						if(conv_status == CONVERSION_ERROR) break;

						result = pobj->set_member_variable(pcur->get_name(), temp);
						break;
					}

					default:
						TRACE_ERROR("Unsupported Game Object Type: " + gpl_type_to_string(member_type))
						result = MEMBER_NOT_OF_GIVEN_TYPE;
						break;
				}

				if(conv_status == CONVERSION_ERROR || result == MEMBER_NOT_OF_GIVEN_TYPE)
				{
					invalid_parameter_type(var_name, pcur->get_name()).write_exception();
				}
			}	

			delete pParams;
		}

		// Register the Symbol
		TRACE_VERBOSE("Creating the Value... ")
		std::shared_ptr<IValue> pval(new GPLVariant(pobj));

		TRACE_VERBOSE("Registering the Symbol...")
		InsertSymbol(var_name, GAME_OBJECT, pval);
		
		TRACE_VERBOSE("Registered the Symbol Successfully")
		GPL_END_DECL_BLOCK()
	}
    | object_type T_ID T_LBRACKET expression T_RBRACKET
	{
		GPL_BEGIN_DECL_BLOCK("object_declaration[1]")

		std::string var_name(*$2);
		delete $2;

		std::shared_ptr<IExpression> ndx_expr($4);
		std::shared_ptr<IValue> ndx_val = ndx_expr->eval();

		int size = 0;
		if(ndx_val->get_int(size) == CONVERSION_ERROR)
		{
			throw invalid_index_type(var_name, ndx_val->get_type());
		}
		else if(size <= 0)
		{
			throw invalid_array_size(var_name, ndx_val->to_string());
		}

		bool bIsArray;
		if(is_symbol_defined(var_name, &bIsArray))
		{
			throw previously_declared_variable(var_name);
		}

		std::shared_ptr<Game_object> pobj;
		std::shared_ptr<IValue> pval;
		for(int i = 0; i < size; i++)
		{
			switch($1)
			{
				case TRIANGLE:
					pobj.reset(new Triangle());
					break;
				case RECTANGLE:
					pobj.reset(new Rectangle());
					break;
				case CIRCLE:
					pobj.reset(new Circle());
					break;
				case PIXMAP:
					pobj.reset(new Pixmap());
					break;
				case TEXTBOX:
					pobj.reset(new Textbox());
					break;
			}
			pval.reset(new GPLVariant(pobj));

			std::string symbol_name = var_name + "[" + std::to_string(i) + "]";
			InsertSymbol(symbol_name, GAME_OBJECT, pval);
		}

		GPL_END_DECL_BLOCK()
	}
    ;

//---------------------------------------------------------------------
object_type:
    T_TRIANGLE
	{
		$$ = TRIANGLE;
	}
    | T_PIXMAP
	{
		$$ = PIXMAP;
	}
    | T_CIRCLE
	{
		$$ = CIRCLE;
	}
    | T_RECTANGLE
	{
		$$ = RECTANGLE;
	}
    | T_TEXTBOX
	{
		$$ = TEXTBOX;
	}
    ;

//---------------------------------------------------------------------
parameter_list_or_empty :
    parameter_list
	{
		$$ = $1;
	}
    | empty
	{
		$$ = NULL;
	}
    ;

//---------------------------------------------------------------------
parameter_list :
    parameter_list T_COMMA parameter
	{
		$$ = $1;
		$$->push_back(std::shared_ptr<Parameter>($3));
	}
    | parameter
	{
		$$ = new ParameterList();
		$$->push_back(std::shared_ptr<Parameter>($1));
	}
    ;

//---------------------------------------------------------------------
parameter:
    T_ID T_ASSIGN expression
	{
		std::string param_name(*$1);
		delete $1;

		std::shared_ptr<IExpression> init_expr($3);

		$$ = new Parameter(param_name, init_expr);
	}
    ;

//---------------------------------------------------------------------
forward_declaration:
    T_FORWARD T_ANIMATION T_ID T_LPAREN animation_parameter T_RPAREN
	{
		try
		{
			std::string anim_name(*$3);
			delete $3;

			std::string param_name(*$5);
			delete $5;

			std::shared_ptr<Symbol> pObjSymbol = Symbol_table::instance()->find_symbol(param_name);
			if(!pObjSymbol || (pObjSymbol->get_type() != GAME_OBJECT))
			{
				throw std::logic_error("Expected Game Object as Animation Block Parameter");
			}

			bool bIsArray;
			if(is_symbol_defined(anim_name, &bIsArray))
			{
				throw previously_declared_variable(anim_name);
			}
				
			std::shared_ptr<Animation_block> pAnim(new Animation_block(pObjSymbol, anim_name));	
			std::shared_ptr<IValue>pVal (new GPLVariant(pAnim));
			InsertSymbol(anim_name, ANIMATION_BLOCK, pVal);
		}
		catch(const gpl_exception& ex)
		{
			ex.write_exception();
		}
		catch(const std::exception& ex)
		{
			TRACE_ERROR("std::exception what(): " << ex.what())
			undefined_error().write_exception();
		}
		catch(...)
		{
			undefined_error().write_exception();
		}
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
		std::string param_name(*$2);
		delete $2;

		// Check to see if the Symbol exists
		bool bIsArray;
		if(is_symbol_defined(param_name, &bIsArray))
		{
			throw previously_declared_variable(param_name);
		}

		std::shared_ptr<Game_object> pObj;
		switch($1)
		{
			case TRIANGLE:
				pObj.reset(new Triangle());
				break;
			case RECTANGLE:
				pObj.reset(new Rectangle());
				break;
			case CIRCLE:
				pObj.reset(new Circle());
				break;
			case PIXMAP:
				pObj.reset(new Pixmap());
				break;
			case TEXTBOX:
				pObj.reset(new Textbox());
				break;
			default:
				throw std::invalid_argument("Unsupported Object Type: " 
					+ game_object_type_to_string($1));
		}

		pObj->never_animate();
		pObj->never_draw();

		std::shared_ptr<IValue> pval(new GPLVariant(pObj));
		InsertSymbol(param_name, GAME_OBJECT, pval);
		$$ = new std::string(param_name);
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
    variable T_ASSIGN expression 
    | variable T_PLUS_ASSIGN expression
    | variable T_MINUS_ASSIGN expression
    ;

//---------------------------------------------------------------------
variable:
    T_ID
	{
		GPL_BEGIN_EXPR_BLOCK("variable[0]")
		$$ = NULL;
		
		std::string var_name(*$1);
		delete $1;

		bool bIsArray;
		std::shared_ptr<Symbol> pSymbol = get_symbol(var_name, &bIsArray);
		if(!pSymbol)
		{
			throw undeclared_variable(var_name);
		}
		else if(bIsArray)
		{
			throw not_an_array(var_name);
		}
		else
		{
			std::shared_ptr<IValue> pVal(new Reference(pSymbol));
			$$ = new ValueExpression(pVal);
		}
		GPL_END_EXPR_BLOCK($$)
	}
    | T_ID T_LBRACKET expression T_RBRACKET
	{
		GPL_BEGIN_EXPR_BLOCK("variable[1]")
		$$ = NULL;

		if(!$1 || !$3) YYABORT;

		std::string var_name(*$1);
		TRACE_VERBOSE("Attempting to index into the array '" << var_name 
			<< "'. Checking to see if the array exists...")
		delete $1;

		bool bIsArray;
		std::shared_ptr<Symbol> pSymbol = get_symbol(var_name, &bIsArray);
		if(!pSymbol)
		{
			throw undeclared_variable(var_name);
		}
		else if(!bIsArray)
		{
			//error_handler.error(Error::VARIABLE_NOT_AN_ARRAY, *$1);
			throw not_an_array(var_name);
		}
		else 
		{
			std::shared_ptr<IExpression> ndx_expr($3);
			$$ = new ArrayReferenceExpression(var_name, ndx_expr);
		}

		GPL_END_EXPR_BLOCK($$)
	}
    | T_ID T_PERIOD T_ID
	{
		GPL_BEGIN_EXPR_BLOCK("variable[2]")
		std::string obj_name(*$1);
		std::string member_name(*$3);
		delete $1; // free the ID
		delete $3; // free the ID

		TRACE_VERBOSE("Member Variable Reference. Object: '" + obj_name 
				+ "', Member: '" + member_name + "'")

		std::shared_ptr<Symbol> pSymbol = Symbol_table::instance()->find_symbol(obj_name);
		if(!pSymbol)
		{
			throw undeclared_variable(obj_name);
		}

		std::shared_ptr<IValue> pval(new MemberReference(pSymbol, member_name));
		$$ = new ValueExpression(pval);		

		GPL_END_EXPR_BLOCK($$)
	}
    | T_ID T_LBRACKET expression T_RBRACKET T_PERIOD T_ID
	{
		GPL_BEGIN_EXPR_BLOCK("variable[3]")
		std::string obj_name(*$1);
		std::string member_name(*$6);
		delete $1; // free the ID
		delete $6; // free the ID

		std::shared_ptr<IExpression> ndx_expr($3);

		bool bIsArray;
		if(!is_symbol_defined(obj_name, &bIsArray)) 
			throw undeclared_variable(obj_name);
		else if(!bIsArray) 
			throw not_an_array(obj_name);

		$$ = new ArrayMemberReferenceExpression(obj_name, member_name, ndx_expr);

		GPL_END_EXPR_BLOCK($$)
	}
    ;


//---------------------------------------------------------------------
expression:
    primary_expression
    | expression T_OR expression
	{
		GPL_BEGIN_EXPR_BLOCK("expression[0]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new OrExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_AND expression
	{
		GPL_BEGIN_EXPR_BLOCK("expression[1]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new AndExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_LESS_EQUAL expression 
	{
		GPL_BEGIN_EXPR_BLOCK("expression[2]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new LessThanEqualExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_GREATER_EQUAL  expression 
	{
		GPL_BEGIN_EXPR_BLOCK("expression[3]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new GreaterThanEqualExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_LESS expression 
	{
		GPL_BEGIN_EXPR_BLOCK("expression[4]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new LessThanExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_GREATER  expression 
	{
		GPL_BEGIN_EXPR_BLOCK("expression[5]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new GreaterThanExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_EQUAL expression 
	{
		GPL_BEGIN_EXPR_BLOCK("expression[6]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new EqualExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_NOT_EQUAL expression 
	{
		GPL_BEGIN_EXPR_BLOCK("expression[7]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new NotEqualExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_PLUS expression 
	{
		GPL_BEGIN_EXPR_BLOCK("expression[8]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new AddExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_MINUS expression 
	{
		GPL_BEGIN_EXPR_BLOCK("expression[9]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new MinusExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_ASTERISK expression 
	{
		GPL_BEGIN_EXPR_BLOCK("expression[10]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new MultiplyExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_DIVIDE expression
	{
		GPL_BEGIN_EXPR_BLOCK("expression[11]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new DivideExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | expression T_MOD expression
	{
		GPL_BEGIN_EXPR_BLOCK("expression[11]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		$$ = new ModExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | T_MINUS  expression %prec UNARY_OPS
	{
		GPL_BEGIN_EXPR_BLOCK("expression[12]")
		std::shared_ptr<IValue> pVal(new GPLVariant(-1));
		std::shared_ptr<IExpression> pLHS(new ValueExpression(pVal));
		std::shared_ptr<IExpression> pRHS($2);
		$$ = new MultiplyExpression(pLHS, pRHS);
		GPL_END_EXPR_BLOCK($$)
	}
    | T_NOT  expression 
	{
		GPL_BEGIN_EXPR_BLOCK("expression[13]")
		std::shared_ptr<IExpression> pOperand($2);
		$$ = new NotExpression(pOperand);
		GPL_END_EXPR_BLOCK($$)
	}
    | math_operator T_LPAREN expression T_RPAREN %prec SUB_EXPR_OPS
	{
		GPL_BEGIN_EXPR_BLOCK("expression[14]")
		std::shared_ptr<IExpression> pArg($3);
		switch($1)
		{
			case SIN:
				$$ = new SinExpression(pArg);
				break;

			case COS:
				$$ = new CosExpression(pArg);
				break;

			case TAN:
				$$ = new TanExpression(pArg);
				break;

			case ASIN:
				$$ = new AsinExpression(pArg);
				break;
	
			case ACOS:
				$$ = new AcosExpression(pArg);
				break;
			
			case ATAN:
				$$ = new AtanExpression(pArg);
				break;

			case FLOOR:
				$$ = new FloorExpression(pArg);
				break;

			case SQRT:
				$$ = new SqrtExpression(pArg);
				break;

			case ABS:
				$$ = new AbsoluteExpression(pArg);
				break;

			case RANDOM:
				$$ = new RandomExpression(pArg);
				break;

			default:
				throw std::runtime_error("That math_operator isn't implemented yet");
		}
		GPL_END_EXPR_BLOCK($$)
	}
    | variable geometric_operator variable %prec OBJECT_COMPARE_OPS
	{
		GPL_BEGIN_EXPR_BLOCK("expression[15]")
		std::shared_ptr<IExpression> pLHS($1);
		std::shared_ptr<IExpression> pRHS($3);
		
		throw std::runtime_error("That geometric_operator isn't implemented yet");
		GPL_END_EXPR_BLOCK($$)
	}
    ;

//---------------------------------------------------------------------
primary_expression:
    T_LPAREN  expression T_RPAREN
	{
		GPL_BEGIN_EXPR_BLOCK("primary_expression[0]")
		$$ = $2;
		GPL_END_EXPR_BLOCK($$)
	}
    | variable
	{
		GPL_BEGIN_EXPR_BLOCK("primary_expression[1]")
		$$ = $1;
		GPL_END_EXPR_BLOCK($$)
	}
    | T_INT_CONSTANT
	{
		GPL_BEGIN_EXPR_BLOCK("primary_expression[2]")
		std::shared_ptr<IValue> pval(new GPLVariant($1));
		$$ = new ValueExpression(pval);
		GPL_END_EXPR_BLOCK($$)
	}
    | T_TRUE
	{
		GPL_BEGIN_EXPR_BLOCK("primary_expression[3]")
		std::shared_ptr<IValue> pval(new GPLVariant(1));
		$$ = new ValueExpression(pval);
		GPL_END_EXPR_BLOCK($$)
	}
    | T_FALSE
	{
		GPL_BEGIN_EXPR_BLOCK("primary_expression[4]")
		std::shared_ptr<IValue> pval(new GPLVariant(0));
		$$ = new ValueExpression(pval);
		GPL_END_EXPR_BLOCK($$)
	}
    | T_DOUBLE_CONSTANT
	{
		GPL_BEGIN_EXPR_BLOCK("primary_expression[5]")
		std::shared_ptr<IValue> pval(new GPLVariant((double)$1));
		$$ = new ValueExpression(pval);
		TRACE_VERBOSE("DOUBLE: " << $1)
		GPL_END_EXPR_BLOCK($$)
	}
    | T_STRING_CONSTANT
	{
		GPL_BEGIN_EXPR_BLOCK("primary_expresion[6]")
		std::shared_ptr<IValue> pval(new GPLVariant(*$1));
		delete $1;
		$$ = new ValueExpression(pval);
		GPL_END_EXPR_BLOCK($$)
	}
    ;

//---------------------------------------------------------------------
geometric_operator:
    T_TOUCHES
	{
		$$ = TOUCHES;
	}
    | T_NEAR
	{
		$$ = NEAR;
	}
    ;

//---------------------------------------------------------------------
math_operator:
    T_SIN
	{
		$$ = SIN;
	}
    | T_COS
	{
		$$ = COS;
	}
    | T_TAN
	{
		$$ = TAN;
	}
    | T_ASIN
	{
		$$ = ASIN;
	}
    | T_ACOS
	{
		$$ = ACOS;
	}
    | T_ATAN
	{
		$$ = ATAN;
	}
    | T_SQRT
	{
		$$ = SQRT;
	}
    | T_ABS
	{
		$$ = ABS;
	}
    | T_FLOOR
	{
		$$ = FLOOR;
	}
    | T_RANDOM
	{
		$$ = RANDOM;
	}
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


