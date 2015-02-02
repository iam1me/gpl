#ifndef VALUE_H
#define VALUE_H

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
	virtual Symbol* get_reference() = 0;

protected:
	virtual GPLVariant* get_variant() = 0;

	template<class T>
	T cast_value();
};

// Specializations are implemented in the cpp
template<class T>
T IValue::cast_value()
{
	throw std::logic_error("Cast Exception");
}


class ConstantValue : public IValue
{
public:

private:
	GPLVariant _var;
};


class ReferenceValue: public IValue
{
public:

private:
	Symbol* _symbol;
};

#endif
