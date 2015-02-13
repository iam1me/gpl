#include <iostream>
#include <string>

#include "symbol.h"
#include "symbol_table.h"
#include "gpl_type.h"
#include "value.h"
#include "parser.h"
#include "gpl_exception.h"
#include "indent.h"

Symbol::Symbol(const std::string& name, const int& val)
	: IVariable(name, INT)
{
	_pvar.reset(new GPLVariant(val, false));	
}

Symbol::Symbol(const std::string& name, const double& val)
	: IVariable(name, DOUBLE)
{
	_pvar.reset(new GPLVariant(val, false));
}

Symbol::Symbol(const std::string& name, const std::string& val)
	: IVariable(name, STRING)
{
	_pvar.reset(new GPLVariant(val, false));
}

Symbol::Symbol(const std::string& name, const std::shared_ptr<Game_object>& val)
	: IVariable(name, GAME_OBJECT)
{
	_pvar.reset(new GPLVariant(val, false));
}

Symbol::Symbol(const std::string& name, const std::shared_ptr<Animation_block>& val)
	: IVariable(name, ANIMATION_BLOCK)
{
	_pvar.reset(new GPLVariant(val, false));
}

Symbol::Symbol(const std::string& name, const Gpl_type& type)
	: IVariable(name, type)
{
	_pvar.reset(new GPLVariant(type, false));
}

Symbol::Symbol(const std::string& name, const Gpl_type& type, const std::shared_ptr<IValue>& pval)
	: IVariable(name, type)
{
	_pvar.reset( new GPLVariant(type, pval,  false));
}

Symbol::~Symbol()
{
}

std::ostream& Symbol::print(std::ostream& os) const
{
	Gpl_type type = get_type();
	os << gpl_type_to_string(type);
	os << " " + get_name();

	if(type & STRING)
	{
		// Put quotes around string literals
		os << " \"" + to_string() + "\"";
	}
	else if(type & GAME_OBJECT)
	{
		os << std::endl;
		indent++;
		os << to_string();
		indent--;
	}
	else if(type & ANIMATION_BLOCK)
	{
		os << to_string();
	}
	else
	{
		os << " " + to_string();
	}

	os << std::endl;
	return os;
}

ConversionStatus Symbol::get_int(int& val) const
{ return _pvar->get_int(val); }

ConversionStatus Symbol::get_double(double& val) const
{ return _pvar->get_double(val); }

ConversionStatus Symbol::get_string(std::string& val) const
{ return _pvar->get_string(val); }

ConversionStatus Symbol::get_game_object(std::shared_ptr<Game_object>& val) const
{ return _pvar->get_game_object(val); }

ConversionStatus Symbol::get_animation_block(std::shared_ptr<Animation_block>& val) const
{ return _pvar->get_animation_block(val); }

ConversionStatus Symbol::set_int(const int& val)
{ return _pvar->set_int(val); }

ConversionStatus Symbol::set_double(const double& val)
{ return _pvar->set_double(val); }

ConversionStatus Symbol::set_string(const std::string& val)
{ return _pvar->set_string(val); }

ConversionStatus Symbol::set_game_object(const std::shared_ptr<Game_object>& val)
{ return _pvar->set_game_object(val); }

ConversionStatus Symbol::set_animation_block(const std::shared_ptr<Animation_block>& val)
{ return _pvar->set_animation_block(val); }

//================================================================================

/*Reference::Reference(std::shared_ptr<Symbol> pSymbol)
	: IValue(pSymbol->get_type(), pSymbol->is_constant())
{
	if(!pSymbol) throw std::invalid_argument("ReferenceValue::ReferenceValue() - pSymbol is NULL");
	_pSymbol = pSymbol;
}

Reference::~Reference()
{}	

ConversionStatus Reference::get_int(int& val) const
{
	return _pSymbol->get_int(val);
}

ConversionStatus Reference::get_double(double& val) const
{
	return _pSymbol->get_double(val);
}

ConversionStatus Reference::get_string(std::string& val) const
{
	return _pSymbol->get_string(val);
}

ConversionStatus Reference::get_game_object(std::shared_ptr<Game_object>& val) const
{
	return _pSymbol->get_game_object(val);
}

ConversionStatus Reference::get_animation_block(std::shared_ptr<Animation_block>& val) const
{
	return _pSymbol->get_animation_block(val);
}

ConversionStatus Reference::set_int(const int& val)
{
	return _pSymbol->set_int(val);
}

ConversionStatus Reference::set_double(const double& val)
{
	return _pSymbol->set_double(val);
}

ConversionStatus Reference::set_string(const std::string& val)
{
	return _pSymbol->set_string(val);
}

ConversionStatus Reference::set_game_object(const std::shared_ptr<Game_object>& val)
{
	return _pSymbol->set_game_object(val);
}

ConversionStatus Reference::set_animation_block(const std::shared_ptr<Animation_block>& val)
{
	return _pSymbol->set_animation_block(val);
}*/


//====================================================================================

	
MemberReference::MemberReference(std::string symbol_name, std::string member_name)
	: IVariable(symbol_name + "." + member_name)
{
	_pSymbol = Symbol_table::instance()->find_symbol(symbol_name);
	if(!_pSymbol)
	{
		TRACE_ERROR("Symbol '" << symbol_name << "' Not Found")
		throw undeclared_variable(symbol_name);
	}

	ConversionStatus conv_status = _pSymbol->get_game_object(_pObj);
	if(conv_status == CONVERSION_ERROR || !_pObj)
	{
		TRACE_ERROR("Failed to get_game_object from Symbol '" + symbol_name + "' - Conversion Error")
		throw object_expected_lhs(symbol_name);	
	}

	_member_name = member_name;
	Gpl_type member_type;

	Status member_status = _pObj->get_member_variable_type(_member_name, member_type);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::MemberReference - " + status_to_string(member_status))
		throw undeclared_member(symbol_name, member_name);
	}

	set_type(member_type);
	set_is_constant(_pSymbol->is_constant());
	_full_name = symbol_name + "." + member_name;
}

MemberReference::MemberReference(std::shared_ptr<Symbol> symbol, std::string member_name)
	: IVariable(symbol->get_name() + "." + member_name)
{
	_pSymbol = symbol;
	if(!_pSymbol)
	{
		throw std::invalid_argument("MemberReference::MemberReference - Symbol is NULL");
	}

	std::string symbol_name = _pSymbol->get_name();

	ConversionStatus conv_status = _pSymbol->get_game_object(_pObj);
	if(conv_status == CONVERSION_ERROR || !_pObj)
	{
		TRACE_ERROR("Failed to get_game_object from Symbol '" + symbol_name + "' - Conversion Error")
		throw object_expected_lhs(symbol_name);
	}

	_member_name = member_name;
	Gpl_type member_type;
	Status member_status = _pObj->get_member_variable_type(_member_name, member_type);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::MemberReference - " + status_to_string(member_status))
		throw undeclared_member(symbol_name, member_name);
	}

	set_type(member_type);
	set_is_constant(_pSymbol->is_constant());
	_full_name = symbol_name + "." + member_name;
}

MemberReference::~MemberReference()
{}

const std::string& MemberReference::get_symbol_name() const
{
	return _pSymbol->get_name();
}

const std::string& MemberReference::get_member_name() const
{
	return _member_name;
}

ConversionStatus MemberReference::get_int(int& val) const
{
	ConversionStatus status = get_conversion_status(get_type(), INT);
	if(status == CONVERSION_ERROR) return status;

	Status member_status = _pObj->get_member_variable(_member_name, val);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::get_int() - Failed to retrieve member variable's value")
		//throw std::runtime_error("MemberReference::get_int() - Failed to retrieve member variable's value");
		return CONVERSION_ERROR;
	}

	return status;	
}

ConversionStatus MemberReference::get_double(double& val) const
{
	ConversionStatus status = get_conversion_status(get_type(), DOUBLE);
	if(status == CONVERSION_ERROR) return status;

	Status member_status = _pObj->get_member_variable(_member_name, val);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::get_double() - Failed to retrieve member variable's value")
		//throw std::runtime_error("MemberReference::get_double() - Failed to retrieve member variable's value");
		return CONVERSION_ERROR;
	}

	return status;	
}

ConversionStatus MemberReference::get_string(std::string& val) const
{
	ConversionStatus status = get_conversion_status(get_type(), STRING);
	if(status == CONVERSION_ERROR) return status;

	Status member_status = 	_pObj->get_member_variable(_member_name, val);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::get_string() - Failed to retrieve member variable's value")
		//throw std::runtime_error("MemberReference::get_string() - Failed to retrieve member variable's value");
		return CONVERSION_ERROR;
	}

	return status;	
}

ConversionStatus MemberReference::get_game_object(std::shared_ptr<Game_object>&) const
{
	// Not Supported
	return CONVERSION_ERROR;
}

ConversionStatus MemberReference::get_animation_block(std::shared_ptr<Animation_block>& val) const
{
	ConversionStatus status = get_conversion_status(get_type(), ANIMATION_BLOCK);
	if(status == CONVERSION_ERROR) return status;

	Status member_status = _pObj->get_member_variable(_member_name, val);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::get_animation_block() - Failed to retrieve member variable's value")
		//throw std::runtime_error("MemberReference::get_animation_block() - Failed to retrieve member variable's value");
		return CONVERSION_ERROR;
	}

	return status;	
}

ConversionStatus MemberReference::set_int(const int& val)
{
	ConversionStatus status = get_conversion_status(get_type(), INT);
	if(status == CONVERSION_ERROR) return status;

	Status member_status = _pObj->set_member_variable(_member_name, val);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::set_int() - Failed to set the member variable's value")
		//throw std::runtime_error("MemberReference::set_int() - Failed to set the member variable's value");
		return CONVERSION_ERROR;
	}

	return status;	
}

ConversionStatus MemberReference::set_double(const double& val)
{
	ConversionStatus status = get_conversion_status(get_type(), DOUBLE);
	if(status == CONVERSION_ERROR) return status;

	Status member_status = _pObj->set_member_variable(_member_name, val);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::set_double() - Failed to set the member variable's value")
		//throw std::runtime_error("MemberReference::set_double() - Failed to set the member variable's value");
		return CONVERSION_ERROR;
	}

	return status;	
}

ConversionStatus MemberReference::set_string(const std::string& val)
{
	ConversionStatus status = get_conversion_status(get_type(), STRING);
	if(status == CONVERSION_ERROR) return status;

	Status member_status = _pObj->set_member_variable(_member_name, val);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::set_string() - Failed to set the member variable's value")
		//throw std::runtime_error("MemberReference::set_string() - Failed to set the member variable's value");
		return CONVERSION_ERROR;
	}

	return status;	
}

ConversionStatus MemberReference::set_game_object(const std::shared_ptr<Game_object>& val)
{
	// Not Supported
	return CONVERSION_ERROR;
}

ConversionStatus MemberReference::set_animation_block(const std::shared_ptr<Animation_block>& val)
{
	ConversionStatus result = get_conversion_status(get_type(), ANIMATION_BLOCK);
	if(result == CONVERSION_ERROR) return result;

	Status status = _pObj->set_member_variable(_member_name, val);
	if(status != OK)
	{
		TRACE_ERROR("MemberReference::set_animation_block() - Failed to set the member variable's value. "
				+ status_to_string(status))
		//throw std::runtime_error("MemberReference::set_animation_block() - Failed to set the member variable's value");
		return CONVERSION_ERROR;
	}

	return result;	
}

//========================================================================

ReferenceExpression::ReferenceExpression(std::shared_ptr<IVariable> pVar)
	: IVariableExpression(pVar->get_name())
{
	if(!pVar) throw std::invalid_argument("Variable is NULL");
	_pRef = pVar;
}

Gpl_type ReferenceExpression::get_type() const
{
	return _pRef->get_type();
}

std::shared_ptr<IValue> ReferenceExpression::eval() const
{
	std::shared_ptr<IValue> pVal = std::dynamic_pointer_cast<IValue>(_pRef);
	if(!pVal) throw undefined_error();

	return pVal;
}

