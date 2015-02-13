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
	
void gpl_exception::set_argument(int i, std::string arg)
{
	if(i < 0 || i > 2)
	{
		throw std::invalid_argument("Array Index Out of Bounds");
	}

	_args[i] = arg;
}

int gpl_exception::get_line() const
{
	return _line;
}

void gpl_exception::set_line(int l)
{
	_line = l;
}

void gpl_exception::write_exception() const		
{ 
	Error::error(_err, _args[0], _args[1], _args[2], _line); 
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

	/*switch(_operator)
	{
		case MOD:
			set_argument(0, "mod");
			break;
		default:
			break;
	}*/
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

object_operand_expected::object_operand_expected(std::string oper_name)
	: gpl_exception(Error::OPERAND_MUST_BE_A_GAME_OBJECT, oper_name)
{
}

object_operand_expected::~object_operand_expected()
{}

invalid_array_size::invalid_array_size(std::string array_name, std::string illegal_size)
	: gpl_exception(Error::INVALID_ARRAY_SIZE, array_name, illegal_size)
{}

index_out_of_bounds::index_out_of_bounds(std::string array_name, int ndx)
	: gpl_exception(Error::ARRAY_INDEX_OUT_OF_BOUNDS, array_name, std::to_string(ndx))
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
	
invalid_index_type::invalid_index_type(std::string array_name, Gpl_type invalid_type)
	: gpl_exception(Error::ARRAY_INDEX_MUST_BE_AN_INTEGER, array_name)
{
	_type = invalid_type;
	switch(_type)
	{
		case INT:
			throw std::logic_error("INT is, actually, a valid index type. The only one. fix this.");

		case DOUBLE:
			set_argument(1, "A double expression");
			break;

		case STRING:
			set_argument(1, "A string expression");
			break;

		case ANIMATION_BLOCK:
			set_argument(1, "A animation_block expression");
			break;

		default:
			throw std::runtime_error("An unaccounted type: " + gpl_type_to_string(_type));
	}
}

std::string invalid_index_type::get_array_name() const
{
	return get_argument(0);
}

Gpl_type invalid_index_type::get_invalid_type() const
{
	return _type;
}
	
unknown_parameter::unknown_parameter(std::string class_name, std::string property_name)
	: gpl_exception(Error::UNKNOWN_CONSTRUCTOR_PARAMETER, class_name, property_name)
{}

invalid_parameter_type::invalid_parameter_type(std::string object_name, std::string param_name)
	: gpl_exception(Error::INCORRECT_CONSTRUCTOR_PARAMETER_TYPE, object_name, param_name)
{}

undeclared_member::undeclared_member(std::string object_name, std::string member_name)
	: gpl_exception(Error::UNDECLARED_MEMBER, object_name, member_name)
{}
