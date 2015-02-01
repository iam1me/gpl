#include "Symbol_table.h"

Symbol_table::Symbol_table()
{

}

Symbol_table::~Symbol_table()
{
	for(SymbolMap::iterator it = _symbols.begin(); it != _symbols.end(); it++)
	{
		delete it->second;
	}
	_symbols.clear();
}

Symbol_table* Symbol_table::instance()
{
	if(!_pTable) _pTable = new Symbol_table();
	return _pTable;
}

Symbol* Symbol_table::find_symbol(const std::string& name) const
{
	SymbolMap::const_iterator it = _symbols.find(name);
	if(it != _symbols.cend()) return it->second;
	else return NULL;
}

void Symbol_table::insert_symbol(Symbol* pSymbol)
{
	if(!pSymbol)
	{
		throw std::runtime_error("Symbol_table::insert_symbol - Symbol is NULL");
	}

	std::pair<SymbolMap::iterator, bool> result;
	result = _symbols.insert(std::make_pair(pSymbol->get_name(), pSymbol));

	if(!result.second)
	{
		throw std::runtime_error("Symbol_table::insert_symbol - Duplicate Symbol");
	}
}

void Symbol_table::print(std::ostream& out) const
{
	for(SymbolMap::const_iterator it = _symbols.cbegin(); it != _symbols.cend(); it++)
	{
		const Symbol* pCur = it->second;
		out << pCur->to_string() << "\n";
	}	
}

