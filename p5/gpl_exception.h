#ifndef GPL_EXCEPTION_H
#define GPL_EXCEPTION_H
#include <stdexcept>

#include "gpl_type.h"
#include "error.h"

class gpl_exception : public std::exception
{
public:
	virtual ~gpl_exception();

	Error::Error_type get_error() const;
	std::string get_argument(int i) const;
	int get_line() const;

protected:
	gpl_exception(Error::Error_type err_type, 
			std::string arg1 = "",
			std::string arg2 = "",
			std::string arg3 = "");	

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
protected:
	std::string _varName;
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
	object_operand_expected();
	virtual ~object_operand_expected();
};

class invalid_array_size : public gpl_exception
{
public:
	invalid_array_size();
	virtual ~invalid_array_size();
};

class index_out_of_bounds : public gpl_exception
{
public:
	index_out_of_bounds();
	virtual ~index_out_of_bounds();
};

class invalid_index_type : public gpl_exception
{
public:
	invalid_index_type();
	virtual ~invalid_index_type();
};

class not_an_array : public gpl_exception
{
public:
	not_an_array(std::string var);
	virtual ~not_an_array();
	std::string get_variable_name();
};

#endif
