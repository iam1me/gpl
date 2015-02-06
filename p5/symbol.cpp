#include "symbol.h"
#include "value.h"

Symbol::~Symbol()
{
}

Symbol::Symbol(const std::string& name, Gpl_type type)
{
	_name = name;

	switch(type)
	{
		case INT:
			_pval.reset(new VariableValue(0));
			_bInitialized = false;
			break;
		case DOUBLE:
			_pval.reset(new VariableValue(.0));
			_bInitialized = false;
			break;
		case STRING:
			_pval.reset(new VariableValue(""));
			_bInitialized = false;
			break;
		default:
			throw std::invalid_argument("Symbol::Symbol - Invalid Type: " + gpl_type_to_string(type));
	}
}

Symbol::Symbol(const std::string& name, std::shared_ptr<IValue> val)
{
	if(!val)
	{
		throw std::invalid_argument("Symbol::Symbol - val is NULL");
	}

	_name = name;
	_pval = val;
}

const std::string& Symbol::get_name() const
{
	return _name;
}

Gpl_type Symbol::get_type() const
{
	return _pval->get_type();
}

std::string Symbol::to_string() const
{
	std::string ret = gpl_type_to_string(_pval->get_type());
	ret += " " + _name + " ";

	if(_pval->get_type() == STRING)
	{
		// Put quotes around string literals
		ret += "\"" + _pval->get_string() + "\"";
	}
	else
	{
		ret += _pval->get_string();
	}

	return ret;
}

/*std::shared_ptr<GPLVariant> Symbol::get_variant()
{
	return _pvar;
}*/

const std::shared_ptr<IValue>& Symbol::get_value()
{
	/*std::shared_ptr<IValue> pVal(
		new ReferenceValue(std::shared_ptr<Symbol>(this)));
	return pVal;*/
	return _pval;
}


void Symbol::set_value(const std::shared_ptr<IValue>& val)
{
	if(!val)
	{
		throw std::invalid_argument("Symbol::set_value - val is NULL");
	}

	if(_pval->is_constant())
	{
		throw std::logic_error("Symbol::set_value - the Symbol holds a constant value");
	}

	IVariable* pTemp = (IVariable*)_pval.get();
	switch(_pval->get_type())
	{
		case INT:
			pTemp->set_value(val->get_int());
			break;

		case DOUBLE:
			pTemp->set_value(val->get_double());
			break;

		case STRING:
			pTemp->set_value(val->get_string());
			break;

		default:
			throw std::logic_error("Symbol::set_value - Unsupported Type");		
	}
	_bInitialized = true;	
}
