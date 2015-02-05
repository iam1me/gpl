#include "value.h"

IValue::~IValue()
{

}

int IValue::get_int() const
{
	const std::shared_ptr<GPLVariant>& var = get_variant();
	if(!var)
	{
		throw std::runtime_error("IValue::get_int - variant is NULL");
	}

	switch(var->get_type())
	{
		case INT:
			return var->get_value<int>();

		default:
			throw std::logic_error("IValue::get_int - invalid cast");
	}
}

double IValue::get_double() const
{
	const std::shared_ptr<GPLVariant>& var = get_variant();
	if(!var)
	{
		throw std::runtime_error("IValue::get_double - variant is NULL");
	}

	switch(var->get_type())
	{
		case INT:
			return (double) var->get_value<int>();

		case DOUBLE:
			return var->get_value<double>();

		default:
			throw std::logic_error("IValue::get_double - invalid cast");
	}
}

std::string IValue::get_string() const
{
	const std::shared_ptr<GPLVariant>& var = get_variant();
	if(!var)
	{
		throw std::runtime_error("IValue::get_string - variant is NULL");
	}

	/*switch(var->get_type())
	{
		case INT:
		{
			int val = var->get_value<int>();
			return std::to_string(val);
		}
		case DOUBLE:
		{
			double val = var->get_value<double>();
			char buff[256];
			sprintf(buff, "%g", val);
			return std::string(buff);
		}
		case STRING:
		{
			return var->get_value<std::string>();
		}			
		default:
		{
			throw std::logic_error("IValue::get_string - invalid cast");
		}
	}*/

	return var->to_string();
}

ConstantValue::ConstantValue(std::shared_ptr<GPLVariant> pVar)
{
	if(!pVar)
	{
		throw std::invalid_argument("var is NULL");
	}

	_pVar = pVar;
}

ConstantValue::ConstantValue(int val)
{
	_pVar.reset(new GPLVariant(val));
}

ConstantValue::ConstantValue(double val)
{
	_pVar.reset(new GPLVariant(val));
}

ConstantValue::ConstantValue(std::string val)
{
	_pVar.reset(new GPLVariant(val));
}

ConstantValue::~ConstantValue()
{}
	
Gpl_type ConstantValue::get_type() const
{
	return _pVar->get_type();
}

bool ConstantValue::is_reference() const
{
	return false;
}

bool ConstantValue::is_constant() const
{
	return true;
}

std::shared_ptr<Symbol> ConstantValue::get_reference() const
{
	return nullptr;
}

std::shared_ptr<GPLVariant> ConstantValue::get_variant() const
{
	return _pVar;
}

ReferenceValue::ReferenceValue(std::shared_ptr<Symbol> pSymbol)
{
	if(!pSymbol)
	{
		throw std::invalid_argument("pSymbol is NULL");	
	}

	_pSymbol = pSymbol;
}

ReferenceValue::~ReferenceValue()
{}
	
Gpl_type ReferenceValue::get_type() const
{
	return _pSymbol->get_type();
}

bool ReferenceValue::is_reference() const
{
	return true;
}

bool ReferenceValue::is_constant() const
{
	return false;
}

std::shared_ptr<Symbol> ReferenceValue::get_reference() const
{
	return _pSymbol;
}

std::shared_ptr<GPLVariant> ReferenceValue::get_variant() const
{
	return _pSymbol->get_variant();
}
