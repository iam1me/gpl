#ifndef VALUE_H
#define VALUE_H

#include <memory>

#include "GPLVariant.h"
#include "symbol.h"

// Forward Declarations
class IValue;
class IVariable;
class ConstantValue;
class ReferenceValue;
class VariableValue;

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
	virtual std::shared_ptr<Game_object> get_game_object() const;
	virtual std::shared_ptr<Animation_block> get_animation_block() const;

	virtual bool is_reference() const = 0;
	virtual bool is_constant() const = 0;
	virtual std::shared_ptr<Symbol> get_reference() const = 0;

protected:
	virtual std::shared_ptr<GPLVariant> get_variant() const = 0;
};

// An interface for values of simple types that can be modified
// the value can be changed but not the type
class IVariable : public IValue
{
public:
	IVariable(Gpl_type type);
	virtual ~IVariable();

	Gpl_type get_type() const;
	bool is_constant() const;
	bool is_reference() const;
	std::shared_ptr<Symbol> get_reference() const;

	virtual void set_value(int val);
	virtual void set_value(double val);
	virtual void set_value(std::string val);
	// Add other types later, if necessary	

private:
	Gpl_type _type;
};

// holds onto a Gpl_type variable and doesn't change it
class ConstantValue : public IValue
{
public:
	ConstantValue(std::shared_ptr<GPLVariant> var);
	ConstantValue(int val);
	ConstantValue(double val);
	ConstantValue(std::string val);
	ConstantValue(const std::shared_ptr<Game_object>& pobj);
	ConstantValue(const std::shared_ptr<Animation_block>& panim);
	
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

// Points to a symbol. This reference may not be changed.
// If you need to edit the underlying GPLVariant, do so
// by means of the referenced Symbol
class ReferenceValue: public IValue
{
public:
	ReferenceValue(std::shared_ptr<Symbol> symbol);
	virtual ~ReferenceValue();	

	Gpl_type get_type() const;
	bool is_reference() const;
	bool is_constant() const;
	
	int get_int() const;
	double get_double() const;
	std::string get_string() const;
	std::shared_ptr<Game_object> get_game_object() const;
	std::shared_ptr<Animation_block> get_animation_block() const;

	std::shared_ptr<Symbol> get_reference() const;

protected:
	std::shared_ptr<GPLVariant> get_variant() const;

private:
	std::shared_ptr<Symbol> _pSymbol;
};


// A variable value has a simple type (int, double, string) but
// can modify its value after construction. 
class VariableValue: public IVariable
{
public:
	VariableValue(int val);
	VariableValue(double val);
	VariableValue(std::string val);
	virtual ~VariableValue();

protected:
	std::shared_ptr<GPLVariant> get_variant() const;

private:
	std::shared_ptr<GPLVariant> _pvar;
};

#endif
