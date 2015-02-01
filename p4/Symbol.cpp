#include "Symbol.h"

template<class T>
Symbol::Symbol(const std::string& name, const Gpl_type& type, const T& val)
{
	_name = name;
	_var.set_type(type);
	_var.set_value<T>(val);
}

Symbol::~Symbol()
{}

const std::string& Symbol::get_name() const
{
	return _name;
}

Gpl_type Symbol::get_type() const
{
	return _var.get_type();
}

template<class T>
const T& Symbol::get_value() const
{
	return _var.get_value<T>();
}

std::string Symbol::to_string() const
{
	std::string ret = gpl_type_to_string(_var_val.get_type());
	ret += _name;

	switch(_var_val.get_type())
	{
		case INT:
			ret += itoa(_var_val.get_value<int>());
			break;

		case DOUBLE:
			ret += ftoa(_var_val.get_value<double>());
			break;

		case STRING:
			ret += _var_val.get_value<std::string>();
			break;
	}
}
