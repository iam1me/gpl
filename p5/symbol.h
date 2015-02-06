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
#include <memory>

#include "GPLVariant.h"
#include "gpl_assert.h"

// Forward Declaration
class IValue;


class Symbol
{
public:
	Symbol(const std::string& name, Gpl_type type); // Declares a symbol, but doesn't initialize it
	Symbol(const std::string& name, std::shared_ptr<IValue> val); // Declares & initializes. val can't be null

	//template<class T>
	//Symbol(const std::string& name, const T& val);
	~Symbol();

	const std::string& get_name() const;
	Gpl_type get_type() const;
	
	//template<class T>
	//const T& get_value() const;

	const std::shared_ptr<IValue>& get_value();	
	void set_value(const std::shared_ptr<IValue>& pval);

	//std::shared_ptr<GPLVariant> get_variant();

	std::string to_string() const;

private:
	std::string _name;
	std::shared_ptr<IValue> _pval;
	bool _bInitialized;
};

/*template<class T>
Symbol::Symbol(const std::string& name, const T& val)
{
	_name = name;
	_pvar.reset(new GPLVariant(val));
}

template<class T>
const T& Symbol::get_value() const
{
	return _pvar->get_value<T>();
}*/


#endif
