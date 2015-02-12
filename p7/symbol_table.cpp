#include "symbol_table.h"
#include "parser.h"

Symbol_table* Symbol_table::_pTable;

Symbol_table::Symbol_table()
{
	if(!_pTable) _pTable = this;
}

Symbol_table::~Symbol_table()
{
	_symbols.clear();
}

Symbol_table* Symbol_table::instance()
{
	if(!_pTable) _pTable = new Symbol_table();
	return _pTable;
}

std::shared_ptr<Symbol> Symbol_table::find_symbol
		(const std::string& name) const
{
	SymbolMap::const_iterator it = _symbols.find(name);
	if(it != _symbols.cend()) return it->second;
	else return NULL;
}

bool Symbol_table::insert_symbol(std::shared_ptr<Symbol> pSymbol)
{
	TRACE_VERBOSE("Symbol_table::insert_symbol - \"" << pSymbol->get_name() << "\"")
	if(!pSymbol)
	{
		throw std::runtime_error("Symbol_table::insert_symbol - Symbol is NULL");
	}

	std::pair<SymbolMap::iterator, bool> result;
	result = _symbols.insert(std::make_pair(pSymbol->get_name(), pSymbol));
	return result.second;
}

void Symbol_table::print(std::ostream& out) const
{
	for(SymbolMap::const_iterator it = _symbols.cbegin(); it != _symbols.cend(); it++)
	{
		const std::shared_ptr<Symbol>& pCur = it->second;
		pCur->print(out);
	}	
}

bool Symbol_table::get(std::string name, int &val)
{
	std::shared_ptr<Symbol> pSymbol = find_symbol(name);
	if(!pSymbol || pSymbol->get_type() != INT) return false;

	ConversionStatus result = pSymbol->get_int(val);
	return (result != CONVERSION_ERROR);
}

bool Symbol_table::get(std::string name, double& val)
{
	std::shared_ptr<Symbol> pSymbol = find_symbol(name);
	if(!pSymbol || pSymbol->get_type() != DOUBLE) return false;

	ConversionStatus result = pSymbol->get_double(val);
	return (result != CONVERSION_ERROR);
}

bool Symbol_table::get(std::string name, std::string& val)
{
	std::shared_ptr<Symbol> pSymbol = find_symbol(name);
	if(!pSymbol || pSymbol->get_type() != STRING) return false;

	ConversionStatus result = pSymbol->get_string(val);
	return (result != CONVERSION_ERROR);
}

bool Symbol_table::get_type(std::string name, Gpl_type& type)
{
	std::shared_ptr<Symbol> pSymbol = find_symbol(name);
	if(!pSymbol) return false;

	type = pSymbol->get_type();
	return true;
}

bool Symbol_table::set(std::string name, int val)
{
	std::shared_ptr<Symbol> pSymbol = find_symbol(name);
	if(!pSymbol || !(pSymbol->get_type() & INT)) return false;

	ConversionStatus result = pSymbol->set_int(val);
	return (result != CONVERSION_ERROR);
}
