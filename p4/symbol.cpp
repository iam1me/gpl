#include "symbol.h"

Symbol::~Symbol()
{
	if(_pvar) delete _pvar;
}

const std::string& Symbol::get_name() const
{
	return _name;
}

Gpl_type Symbol::get_type() const
{
	return _pvar->get_type();
}

std::string Symbol::to_string() const
{
	std::string ret = gpl_type_to_string(_pvar->get_type());
	ret += " " + _name;

	switch(_pvar->get_type())
	{
		case INT:
			ret += _pvar->get_value<int>();
			//sprintf(ret, "%s %d", ret, (_var.get_value<int>()));
			break;

		case DOUBLE:
			ret += _pvar->get_value<double>();
			//rintf(ret, "%s %f", ret, _var.get_value<double>());
			break;

		case STRING:
			ret += _pvar->get_value<std::string>();
			break;

		default:
			throw std::runtime_error("Symbol::to_string - Unrecognized Type");
	}

	return ret;
}

