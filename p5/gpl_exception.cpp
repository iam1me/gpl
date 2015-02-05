#include "gpl_exception.h"

extern int line_count;

gpl_exception::gpl_exception(Error::Error_type err_type, 
	std::string arg1, std::string arg2, std::string arg3)
{
	_err = err_type;
	_args[0] = arg1;
	_args[1] = arg2;
	_args[2] = arg3;
	_line = line_count;
}

gpl_exception::~gpl_exception()
{}

Error::Error_type gpl_exception::get_error() const
{
	return _err;
}

std::string gpl_exception::get_argument(int i) const
{
	if(i < 0 || i > 2)
	{
		throw std::invalid_argument("Array Index Out of Bounds");
	}

	return _args[i];
}

int gpl_exception::get_line() const
{
	return _line;
}

previously_declared_variable::previously_declared_variable(std::string var_name)
	: gpl_exception(Error::PREVIOUSLY_DECLARED_VARIABLE, var_name)
{
	_varName = var_name;
}

previously_declared_variable::~previously_declared_variable() 
{}

std::string previously_declared_variable::get_variable_name() const
{
	return _varName;
}


invalid_operand_type::invalid_operand_type(Operator_type op_type, bool bLHS)
	: gpl_exception((bLHS)? Error::INVALID_LEFT_OPERAND_TYPE : 
		Error::INVALID_RIGHT_OPERAND_TYPE, 
		operator_to_string(op_type) )
{
	_operator = op_type;
	_bLHS = bLHS;
}

invalid_operand_type::~invalid_operand_type() 
{}

Operator_type invalid_operand_type::get_operator() const
{
	return _operator;
}

bool invalid_operand_type::isLHS() const
{
	return _bLHS;
}

bool invalid_operand_type::isRHS() const
{
	return !_bLHS;
}

object_operand_expected::object_operand_expected()
	: gpl_exception(Error::OPERAND_MUST_BE_A_GAME_OBJECT)
{
}

object_operand_expected::~object_operand_expected()
{}

invalid_array_size::invalid_array_size()
	: gpl_exception(Error::INVALID_ARRAY_SIZE)
{}

invalid_array_size::~invalid_array_size()
{}

index_out_of_bounds::index_out_of_bounds()
	: gpl_exception(Error::ARRAY_INDEX_OUT_OF_BOUNDS)
{}

index_out_of_bounds::~index_out_of_bounds()
{}

invalid_index_type::invalid_index_type()
	: gpl_exception(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER)
{}

invalid_index_type::~invalid_index_type()
{}

undeclared_variable::undeclared_variable(std::string var_name)
	: gpl_exception(Error::UNDECLARED_VARIABLE, var_name)
{
	_varName = var_name;
}

undeclared_variable::~undeclared_variable()
{}

std::string undeclared_variable::get_variable_name() const
{
	return _varName;
}

not_an_array::not_an_array(std::string varName)
	: gpl_exception(Error::VARIABLE_NOT_AN_ARRAY, varName)
{}

not_an_array::~not_an_array()
{}

std::string not_an_array::get_variable_name()
{
	return get_argument(0);
}
