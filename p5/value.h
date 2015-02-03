#ifndef VALUE_H
#define VALUE_H

#include <memory>

#include "GPLVariant.h"
#include "symbol.h"

// A value is either a constant or else a variable reference. There are 3 kinds
// of variables: simple (int, string, char, animation_block), member variables
// (obj.x, obj.drawing_order, etc), and array references (my_stuff[35]). In all
// cases, the value held must be one specified by the Gpl_type enum. Below is 
// the common base-class for all these cases.

// Since we will often need to cast a Value from one Gpl_type to another, Value
// provides functions for retrieving the value as different data types.
// Of course, not every Gpl_type can be cast to every type - so an inappropriate
// cast attempt will result in an error.

class IValue
{
public:
	virtual ~IValue();

	virtual Gpl_type get_type() = 0;
	
	virtual int get_int();
	virtual double get_double();
	virtual std::string get_string();
	// Add other types later

	virtual bool is_reference() = 0;
	virtual bool is_constant() = 0;
	virtual const std::shared_ptr<Symbol>& get_reference() = 0;

protected:
	virtual const std::shared_ptr<GPLVariant>& get_variant() = 0;
};

class ConstantValue : public IValue
{
public:
	ConstantValue(std::shared_ptr<GPLVariant> var);
	virtual ~ConstantValue();
	
private:
	std::shared_ptr<GPLVariant> _pVar;
};


class ReferenceValue: public IValue
{
public:
	ReferenceValue(std::shared_ptr<Symbol> symbol);
	virtual ~ReferenceValue();	

private:
	std::shared_ptr<Symbol> _pSymbol;
};

#endif
