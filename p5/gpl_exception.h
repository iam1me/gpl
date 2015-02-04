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


#endif
