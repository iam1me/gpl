#include "parameter.h"

Parameter::Parameter(std::string param_name, std::shared_ptr<IExpression> init_expr)
{
	_name = param_name;
	_expr = init_expr;
}

Parameter::~Parameter()
{}

const std::string& Parameter::get_name() const
{
	return _name;
}

const std::shared_ptr<IExpression>& Parameter::get_expr() const
{
	return _expr;
}
