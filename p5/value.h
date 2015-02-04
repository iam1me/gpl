#ifndef VALUE_H
#define VALUE_H

#include <memory>

#include "GPLVariant.h"
#include "symbol.h"

// Forward Declarations
class IValue;
class ConstantValue;
class ReferenceValue;

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

	virtual Gpl_type get_type() const = 0;
	
	virtual int get_int() const;
	virtual double get_double() const;
	virtual std::string get_string() const;
	// Add other types later

	virtual bool is_reference() const = 0;
	virtual bool is_constant() const = 0;
	virtual std::shared_ptr<Symbol> get_reference() const = 0;

protected:
	virtual std::shared_ptr<GPLVariant> get_variant() const = 0;
};

class ConstantValue : public IValue
{
public:
	ConstantValue(std::shared_ptr<GPLVariant> var);
	ConstantValue(int val);
	ConstantValue(double val);
	ConstantValue(std::string val);
	virtual ~ConstantValue();
	
	Gpl_type get_type() const;

	bool is_reference() const;
	bool is_constant() const;
	std::shared_ptr<Symbol> get_reference() const;

protected:
	std::shared_ptr<GPLVariant> get_variant() const;

private:
	std::shared_ptr<GPLVariant> _pVar;
};

class ReferenceValue: public IValue
{
public:
	ReferenceValue(std::shared_ptr<Symbol> symbol);
	virtual ~ReferenceValue();	

	Gpl_type get_type() const;
	bool is_reference() const;
	bool is_constant() const;
	std::shared_ptr<Symbol> get_reference() const;

protected:
	std::shared_ptr<GPLVariant> get_variant() const;

private:
	std::shared_ptr<Symbol> _pSymbol;
};

#endif
