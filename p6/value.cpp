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

	return var->to_string();
}
	
std::shared_ptr<Game_object> IValue::get_game_object() const
{
	const std::shared_ptr<GPLVariant>& var = get_variant();
	if(!var)
	{
		throw std::runtime_error("IValue::get_game_object - variant is NULL");
	}
	return var->get_value<std::shared_ptr<Game_object>>();
}

std::shared_ptr<Animation_block> IValue::get_animation_block() const
{
	const std::shared_ptr<GPLVariant>& var = get_variant();
	if(!var)
	{
		throw std::runtime_error("IValue::get_animation_block - variant is NULL");
	}
	return var->get_value<std::shared_ptr<Animation_block>>();
}

IVariable::IVariable(Gpl_type type)
{
	_type = type;
}

IVariable::~IVariable()
{}

bool IVariable::is_constant() const
{ return false; }

Gpl_type IVariable::get_type() const
{ return _type; }
	
bool IVariable::is_reference() const
{ return false; }

std::shared_ptr<Symbol> IVariable::get_reference() const
{ return nullptr; }

void IVariable::set_value(int val)
{
	switch(_type)
	{
		case INT:
			get_variant()->set_value<int>(val);
			break;

		case DOUBLE:
			get_variant()->set_value<double>((double)val);
			break;

		case STRING:
			get_variant()->set_value<std::string>(std::to_string(val));
			break;

		default:
			throw std::logic_error("IVariable::set_value(int)  - Invalid Type");
	}
}

void IVariable::set_value(double val)
{
	switch(_type)
	{
		case INT:
			get_variant()->set_value<int>((int)val);
			break;

		case DOUBLE:
			get_variant()->set_value<double>(val);
			break;

		case STRING:
			char buf[256];
			sprintf(buf, "%g", val);
			get_variant()->set_value<std::string>(std::string(buf));
			break;

		default:
			throw std::logic_error("IVariable::set_value(double) - Invalid Type");
	}
}

void IVariable::set_value(std::string val)
{
	switch(_type)
	{
		case STRING:
			get_variant()->set_value<std::string>(val);
			break;

		default:
			throw std::logic_error("IVariable::set_value(string) - Invalid Type");
	}
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

ConstantValue::ConstantValue(const std::shared_ptr<Game_object>& pobj)
{
	if(!pobj)
	{
		throw std::invalid_argument("ConstantValue::ConstantValue - Game_object is NULL");
	}
	_pVar.reset(new GPLVariant(pobj));
}

ConstantValue::ConstantValue(const std::shared_ptr<Animation_block>& panim)
{
	if(!panim)
	{
		throw std::invalid_argument("ConstantValue::ConstantValue - Animation_block is NULL");
	}

	_pVar.reset(new GPLVariant(panim));
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
	return nullptr;
}
	
int ReferenceValue::get_int() const
{
	return _pSymbol->get_value()->get_int();
}

double ReferenceValue::get_double() const
{
	return _pSymbol->get_value()->get_double();
}

std::string ReferenceValue::get_string() const
{
	return _pSymbol->get_value()->get_string();
}
	
std::shared_ptr<Game_object> ReferenceValue::get_game_object() const
{
	return _pSymbol->get_value()->get_game_object();
}

std::shared_ptr<Animation_block> ReferenceValue::get_animation_block() const
{
	return _pSymbol->get_value()->get_animation_block();
}


VariableValue::VariableValue(int val)
	: IVariable(INT)
{
	_pvar.reset(new GPLVariant(val));	
}

VariableValue::VariableValue(double val)
	: IVariable(DOUBLE)
{
	_pvar.reset(new GPLVariant(val));
}

VariableValue::VariableValue(std::string val)
	: IVariable(STRING)
{
	_pvar.reset(new GPLVariant(val));
}

VariableValue::~VariableValue()
{

}

std::shared_ptr<GPLVariant> VariableValue::get_variant() const
{
	return _pvar;
}
