/** Symbol.h
 ** Defines the symbol class used by the Symbol_table class.
 ** Holds information relevant to a particular symbol in the
 ** source file being parsed.
 **/

#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include <string.h>
#include <stdexcept>
#include "GPLVariant.h"
#include "gpl_assert.h"

class Symbol
{
public:
	template<class T>
	Symbol(const std::string& name, const T& val);
	~Symbol();

	const std::string& get_name() const;
	Gpl_type get_type() const;
	
	template<class T>
	const T& get_value() const;

	std::string to_string() const;

private:
	std::string _name;
	GPLVariant* _pvar;
};

template<class T>
Symbol::Symbol(const std::string& name, const T& val)
{
	_name = name;
	_pvar = new GPLVariant(val);
}

template<class T>
const T& Symbol::get_value() const
{
	return _pvar->get_value<T>();
}

#endif
