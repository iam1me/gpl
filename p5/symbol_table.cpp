#include "symbol_table.h"

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
		out << pCur->to_string() << "\n";
	}	
}

