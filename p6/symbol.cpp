#include <iostream>
#include <string>

#include "symbol.h"
#include "symbol_table.h"
#include "gpl_type.h"
#include "value.h"
#include "parser.h"

Symbol::Symbol(const std::string& name, const int& val)
	: GPLVariant(val, false)
{
	_name = name;
}

Symbol::Symbol(const std::string& name, const double& val)
	: GPLVariant(val, false)
{
	_name = name;
}

Symbol::Symbol(const std::string& name, const std::string& val)
	: GPLVariant(val, false)
{
	TRACE_VERBOSE("Symbol::Symbol('" << name << "', '" << val << "')")
	_name = name;
}

Symbol::Symbol(const std::string& name, const std::shared_ptr<Game_object>& val)
	: GPLVariant(val, false)
{
	_name = name;
}

Symbol::Symbol(const std::string& name, const std::shared_ptr<Animation_block>& val)
	: GPLVariant(val, false)
{
	_name = name;
}

Symbol::Symbol(const std::string& name, const Gpl_type& type)
	: GPLVariant(type, false)
{
	_name = name;
}

Symbol::Symbol(const std::string& name, const Gpl_type& type, const std::shared_ptr<IValue>& pval)
	: GPLVariant(type, pval,  false)
{
	TRACE_VERBOSE("Symbol::Symbol(" << name << ", " << gpl_type_to_string(type) 
				<< ", " << pval->to_string() << ")");
	_name = name;
}

Symbol::~Symbol()
{
}

const std::string& Symbol::get_name() const
{
	return _name;
}

std::ostream& Symbol::print(std::ostream& os) const
{
	Gpl_type type = get_type();
	os << gpl_type_to_string(type);
	os << " " + _name;

	if(type == STRING)
	{
		// Put quotes around string literals
		os << " \"" + to_string() + "\"";
	}
	else if(type & (GAME_OBJECT | ANIMATION_BLOCK))
	{
		os << std::endl;
		indent++;
		os << to_string();
		indent--;
	}
	else
	{
		os << " " + to_string();
	}

	os << std::endl;
	return os;
}


Reference::Reference(std::shared_ptr<Symbol> pSymbol)
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
}


//====================================================================================

	
MemberReference::MemberReference(std::string symbol_name, std::string member_name)
	: IValue()
{
	_pSymbol = Symbol_table::instance()->find_symbol(member_name);
	if(!_pSymbol)
	{
		TRACE_ERROR("Symbol '" << symbol_name << "' Not Found")
		throw std::runtime_error("Symbol '" + symbol_name + "' Not Found");
	}

	ConversionStatus conv_status = _pSymbol->get_game_object(_pObj);
	if(conv_status == CONVERSION_ERROR || !_pObj)
	{
		TRACE_ERROR("Failed to get_game_object from Symbol '" + symbol_name + "' - Conversion Error")
		throw std::invalid_argument("Symbol '" + symbol_name + "' does not contain a Game_object");
	}

	_member_name = member_name;
	Gpl_type member_type;

	Status member_status = _pObj->get_member_variable_type(_member_name, member_type);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::MemberReference - " + status_to_string(member_status))
		throw std::runtime_error("Failed to retrieve the member status of '" + _member_name 
					+ "' from Game_object '" + _pSymbol->get_name() + "': "
					+ status_to_string(member_status));
	}

	set_type(member_type);
	set_is_constant(_pSymbol->is_constant());
}

MemberReference::MemberReference(std::shared_ptr<Symbol> symbol, std::string member_name)
	: IValue()
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
		throw std::invalid_argument("Symbol '" + symbol_name + "' does not contain a Game_object");
	}

	_member_name = member_name;
	Gpl_type member_type;
	Status member_status = _pObj->get_member_variable_type(_member_name, member_type);
	if(member_status != OK)
	{
		TRACE_ERROR("MemberReference::MemberReference - " + status_to_string(member_status))
		throw std::runtime_error("Failed to retrieve the member status of '" + _member_name
					+ "' from Game_object '" + _pSymbol->get_name() + "': "
					+ status_to_string(member_status));
	}

	set_type(member_type);
	set_is_constant(_pSymbol->is_constant());
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

