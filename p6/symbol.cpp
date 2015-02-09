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
	
