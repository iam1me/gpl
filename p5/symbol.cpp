#include "symbol.h"

Symbol::~Symbol()
{
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
	ret += " " + _name + " ";

	if(_pvar->get_type() == STRING)
	{
		// Put quotes around string literals
		ret += "\"" + _pvar->to_string() + "\"";
	}
	else
	{
		ret += _pvar->to_string();
	}

	return ret;
}

