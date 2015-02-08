#include "symbol.h"
#include "value.h"
#include "parser.h"

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
	_type = type;
}

Symbol::Symbol(const std::string& name, Gpl_type type, std::shared_ptr<IValue> val)
{
	if(!val)
	{
		throw std::invalid_argument("Symbol::Symbol - val is NULL");
	}

	_name = name;
	_type = type;
	_pval = val;

	TRACE_VERBOSE("Constructed Symbol '" << name.c_str()
			<< "' (" << gpl_type_to_string(_type) << "): "
			<< val->get_string())
}

const std::string& Symbol::get_name() const
{
	return _name;
}

Gpl_type Symbol::get_type() const
{
	return _type;
}

std::string Symbol::to_string() const
{
	std::string ret = gpl_type_to_string(_type);
	ret += " " + _name;

	if(_type == STRING)
	{
		// Put quotes around string literals
		ret += " \"" + _pval->get_string() + "\"";
	}
	else if(_type & (GAME_OBJECT | ANIMATION_BLOCK))
	{
		ret += "\n";
		indent++;
		ret += _pval->get_string();
		indent--;
	}
	else
	{
		ret += " " + _pval->get_string();
	}

	return ret;
}

const std::shared_ptr<IValue>& Symbol::get_value()
{
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
	switch(_type)
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
