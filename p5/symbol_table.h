#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <iostream>
#include <map>
#include "symbol.h"

class Symbol_table
{
public:
	typedef std::map<std::string, std::shared_ptr<Symbol>> SymbolMap;
	virtual ~Symbol_table();
	static Symbol_table* instance();

	// returns null if not found
	std::shared_ptr<Symbol> find_symbol(const std::string& name) const;
	bool insert_symbol(std::shared_ptr<Symbol> pSymbol);

	void print(std::ostream&) const;
protected:
	Symbol_table();

private:
	static Symbol_table* _pTable;
	SymbolMap _symbols;
};

#endif
