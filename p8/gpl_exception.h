#ifndef GPL_EXCEPTION_H
#define GPL_EXCEPTION_H
#include <stdexcept>

#include "gpl_type.h"
#include "error.h"

/*enum ErrorSeverity
{
	FATAL,
	DO_NOT_RUN, // continue parsing
	CONTINUE, // report and move on
	IGNORE // copmletely silence the error and ignore
};*/

class gpl_exception : public std::exception
{
public:
	virtual ~gpl_exception();

	Error::Error_type get_error() const;
	std::string get_argument(int i) const;
	int get_line() const;

	/*virtual ErrorSeverity get_severity() const
		{ return DO_NOT_RUN; };*/

	void write_exception() const;

protected:
	gpl_exception(Error::Error_type err_type, 
			std::string arg1 = "",
			std::string arg2 = "",
			std::string arg3 = "");	

	void set_argument(int i, std::string arg);
	void set_line(int line);

private:
	int _line;
	Error::Error_type _err;
	std::string _args[3];		
};

class undefined_error : public gpl_exception
{
public:
	undefined_error() 
		: gpl_exception(Error::UNDEFINED_ERROR) {};
	virtual ~undefined_error() {};
};

class divide_by_zero : public gpl_exception
{
public:
	divide_by_zero()
		: gpl_exception(Error::DIVIDE_BY_ZERO_AT_PARSE_TIME) {};
	virtual ~divide_by_zero() {};
	//ErrorSeverity get_severity() const { return CONTINUE; };
};

class mod_by_zero : public gpl_exception
{
public:
	mod_by_zero()
		: gpl_exception(Error::MOD_BY_ZERO_AT_PARSE_TIME) {};
	virtual ~mod_by_zero() {};
};

class undeclared_variable : public gpl_exception
{
public:
	undeclared_variable(std::string var_name);
	virtual ~undeclared_variable();
	std::string get_variable_name() const;
	//ErrorSeverity get_severity() const { return FATAL; };
protected:
	std::string _varName;
};

class bad_initial_value : public gpl_exception
{
public:
	bad_initial_value(std::string var_name)
		: gpl_exception(Error::INVALID_TYPE_FOR_INITIAL_VALUE, var_name) {};
	virtual ~bad_initial_value() {};
	//ErrorSeverity get_severity() const { return CONTINUE; };
};

class previously_declared_variable : public gpl_exception
{
public:
	previously_declared_variable(std::string var_name);
	~previously_declared_variable();
	std::string get_variable_name() const;

protected:
	std::string _varName;
};

class invalid_operand_type : public gpl_exception
{
public:
	invalid_operand_type(Operator_type op_type, bool bLHS);
	virtual ~invalid_operand_type();

	Operator_type get_operator() const;
	bool isLHS() const;
	bool isRHS() const;

private:
	Operator_type _operator;
	bool _bLHS;
};

class object_operand_expected : public gpl_exception
{
public:
	object_operand_expected(std::string oper_name);
	virtual ~object_operand_expected();
};

class invalid_array_size : public gpl_exception
{
public:
	invalid_array_size(std::string array_name, std::string invalid_size);
	virtual ~invalid_array_size() {};
	//ErrorSeverity get_severity() const { return FATAL; };
};

class invalid_index_type : public gpl_exception
{
public:
	invalid_index_type(std::string array_name, Gpl_type invalid_type);
	virtual ~invalid_index_type() {};
	std::string get_array_name() const;
	Gpl_type get_invalid_type() const;
private:
	std::string _name;
	Gpl_type _type;
};


class index_out_of_bounds : public gpl_exception
{
public:
	index_out_of_bounds(std::string array_name, int ndx);
	virtual ~index_out_of_bounds() {};
};

class not_an_array : public gpl_exception
{
public:
	not_an_array(std::string var);
	virtual ~not_an_array();
	std::string get_variable_name();
};

class unknown_parameter : public gpl_exception
{
public:
	unknown_parameter(std::string class_name, std::string property_name);
	virtual ~unknown_parameter() {};
};

class undeclared_member : public gpl_exception
{
public:
	undeclared_member(std::string object_name, std::string member_name);
	virtual ~undeclared_member() {};
};

class invalid_parameter_type : public gpl_exception
{
public:
	invalid_parameter_type(std::string object_name, std::string member_name);
	virtual ~invalid_parameter_type() {};
};

class object_expected_lhs : public gpl_exception
{
public:
	object_expected_lhs(std::string symbol_name)
		: gpl_exception(Error::LHS_OF_PERIOD_MUST_BE_OBJECT, symbol_name) {};
	virtual ~object_expected_lhs() {};
};
          
class object_type_mismatch : public gpl_exception
{
public:
	object_type_mismatch(std::string object_name, std::string anim_name)
		: gpl_exception(Error::TYPE_MISMATCH_BETWEEN_ANIMATION_BLOCK_AND_OBJECT, 
		object_name, anim_name) {};
	virtual ~object_type_mismatch() {};
};


class invalid_if_expression : public gpl_exception
{
public:
	invalid_if_expression()
		: gpl_exception(Error::INVALID_TYPE_FOR_IF_STMT_EXPRESSION) 
		{};
	virtual ~invalid_if_expression() {};
};

class invalid_for_expression : public gpl_exception
{
public:
	invalid_for_expression()
		: gpl_exception(Error::INVALID_TYPE_FOR_FOR_STMT_EXPRESSION)
		{};
	virtual ~invalid_for_expression() {};
};

class invalid_print_expression : public gpl_exception
{
public:
	invalid_print_expression()
		: gpl_exception(Error::INVALID_TYPE_FOR_PRINT_STMT_EXPRESSION)
		{};
	virtual ~invalid_print_expression() {};
};

class invalid_exit_expression : public gpl_exception
{
public:
	invalid_exit_expression(Gpl_type type)
		: gpl_exception(Error::EXIT_STATUS_MUST_BE_AN_INTEGER,
		  gpl_type_to_string(type)) {};
	virtual ~invalid_exit_expression() {};
};

class animation_body_expected : public gpl_exception
{
public:
	animation_body_expected(std::string anim_name)
		: gpl_exception(Error::NO_BODY_PROVIDED_FOR_FORWARD, anim_name) 
		{};

	virtual ~animation_body_expected() {};
};

class animation_forward_expected : public gpl_exception
{
public:
	animation_forward_expected(std::string anim_name, int line)
		: gpl_exception(Error::NO_FORWARD_FOR_ANIMATION_BLOCK, anim_name) 
		{ set_line(line); };
	virtual ~animation_forward_expected() {};
};

class animation_parameter_not_unique : public gpl_exception
{
public:
	animation_parameter_not_unique(std::string param_name)
		: gpl_exception(Error::ANIMATION_PARAMETER_NAME_NOT_UNIQUE, param_name) {};
	virtual ~animation_parameter_not_unique() {};
};

class animation_parameter_invalid : public gpl_exception
{
public:
	animation_parameter_invalid(int line)
		: gpl_exception(Error::ANIMATION_PARAM_DOES_NOT_MATCH_FORWARD)
		 { set_line(line); };

	virtual ~animation_parameter_invalid(){};
};

class animation_previously_declared : public gpl_exception
{
public:
	animation_previously_declared(std::string anim_name, int line)
		: gpl_exception(Error::PREVIOUSLY_DEFINED_ANIMATION_BLOCK, anim_name)
		{ set_line(line); }
	virtual ~animation_previously_declared() {};
};

class invalid_assign_lhs : public gpl_exception
{
public:
	invalid_assign_lhs(Assignment_type oper, std::string var_name, Gpl_type var_type)
		: gpl_exception((oper == ASSIGN)? Error::INVALID_LHS_OF_ASSIGNMENT :
				(oper == ADD_ASSIGN)? Error:: INVALID_LHS_OF_PLUS_ASSIGNMENT :
				Error::INVALID_LHS_OF_MINUS_ASSIGNMENT, 
				var_name, gpl_type_to_string(var_type))
		{};

	virtual ~invalid_assign_lhs() {};
};

class invalid_assign_rhs : public gpl_exception
{
public:
	invalid_assign_rhs(Assignment_type oper, Gpl_type lhs_type, Gpl_type rhs_type)
		: gpl_exception((oper == ASSIGN)? Error::ASSIGNMENT_TYPE_ERROR :
				(oper == ADD_ASSIGN)? Error::PLUS_ASSIGNMENT_TYPE_ERROR :
				Error::MINUS_ASSIGNMENT_TYPE_ERROR,
				gpl_type_to_string(lhs_type), gpl_type_to_string(rhs_type))
		{};

	virtual ~invalid_assign_rhs() {};
};

#endif
