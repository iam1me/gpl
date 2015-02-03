#include "symbol.h"
#include "value.h"

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

std::shared_ptr<GPLVariant> Symbol::get_variant()
{
	return _pvar;
}

std::shared_ptr<IValue> Symbol::get_value()
{
	std::shared_ptr<IValue> pVal(
		new ReferenceValue(std::shared_ptr<Symbol>(this)));
	return pVal;
}


void Symbol::set_value(std::shared_ptr<IValue> pval)
{
	switch(_pvar->get_type())
	{
		case INT:
			_pvar->set_value<int>(pval->get_int());
			break;

		case DOUBLE:
			_pvar->set_value<double>(pval->get_double());
			break;

		case STRING:
			_pvar->set_value<std::string>(pval->get_string());
			break;

		default:
			throw std::logic_error("Symbol::set_value - Unsupported Type");		
	}	
}
