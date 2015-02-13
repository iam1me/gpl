#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include <memory>

#include "GPLVariant.h"
#include "value.h"

//==================================================================
//	F O R W A R D  D E F I N I T I O N S 
//==================================================================
class IExpression;
class IVariableExpression;
class ValueExpression;
class ArrayReferenceExpression;
class ArrayMemberReferenceExpression; //Implement later

// Math Expressions
class IOperationalExpression;
class AddExpression;
class SubtractExpression;
class MultiplyExpression;
class DivideExpression;
class ModExpression;
class SinExpression;
class CosExpression;
class TanExpression;
class AsinExpression;
class AcosExpression;
class AtanExpression;
class SqrtExpression;
class FloorExpression;
class AbsoluteExpression;
class RandomExpression;

// Comparison Expressions
class EqualExpression;
class NotEqualExpression;
class LessThanExpression;
class LessThanEqualExpression;
class GreaterThanExpression;
class GreaterThanEqualExpression;

// Logical Expressions
class AndExpression;
class OrExpression;
class NotExpression;

// Geometric Expressions
class TouchExpression;
class NearExpression;

typedef std::vector<std::shared_ptr<IExpression>> ExpressionList;

// An expression is a statement that can be evaluated to produce a value.
// Values are produced by taking constants & variables and applying operators
// to them. These values maybe a direct reference to a variable or a constant.
// Expressions can take different forms. There are unary operators (prefix and
// postfix ones), binary operators, and function calls that take arguments.
// We will capture the common functionality below in IExpression

// All expressions are structured as a tree - where each node is an Expression.
// All leaf nodes will hold a value of some kind. Intermediate nodes will hold
// operators for acting upon its children, once they have been evaluated.

// NOTE: Expressions do not chnage after construction. So there are no methods
// to add/remove child Expressions.

class IExpression
{
public:
	virtual ~IExpression();

	virtual Gpl_type get_type() const = 0;
	virtual std::shared_ptr<IValue> eval() const = 0;

	virtual int get_child_count() const;
	virtual std::shared_ptr<IExpression> get_child(int ndx) const;

protected:
	virtual ExpressionList& get_children();
	virtual void add_child(std::shared_ptr<IExpression> child);

private:
	ExpressionList _children;
};

// For expressions that evaluate to specific variables
class IVariableExpression : public IExpression
{
public:
	virtual ~IVariableExpression() {};
	virtual const std::string& get_name() const;

protected:
	IVariableExpression(const std::string& var_name);
	void set_name(const std::string& name);

private:
	std::string _name;
};

// Holds a value. a Leaf node. Values should be used for constants, not Variables
class ValueExpression : public IExpression
{
public: 
	ValueExpression(std::shared_ptr<IValue>);
	virtual ~ValueExpression();

	Gpl_type get_type() const;
	std::shared_ptr<IValue> eval() const;
	
private:
	std::shared_ptr<IValue> _pVal;	
};

// Upon evaluation, ReferenceExpression determines which symbol to address
// and returns its value. This is used for calculating array references 
// dynamically. For example: y = myarray[x + 1]; 
class ArrayReferenceExpression : public IVariableExpression
{
public:
	ArrayReferenceExpression(std::string array_name, 
				std::shared_ptr<IExpression>);
	virtual ~ArrayReferenceExpression();

	Gpl_type get_type() const;
	std::shared_ptr<IValue> eval() const;

private:
	std::shared_ptr<IValue> _pVal;
	Gpl_type _type;
};

class ArrayMemberReferenceExpression : public IVariableExpression
{
public:
	ArrayMemberReferenceExpression(std::string array_name, std::string member_name,
					std::shared_ptr<IExpression> ndx_expr);
	virtual ~ArrayMemberReferenceExpression() {};
	Gpl_type get_type() const;

	std::shared_ptr<IValue> eval() const;
	const std::string& get_array_name() const;
	const std::string& get_member_name() const;
private:
	std::shared_ptr<IValue> _pVal;
	std::string _array_name, _member_name;
	Gpl_type _type;
};



class IOperationalExpression : public IExpression
{
public:
	IOperationalExpression(Operator_type type) 
		: IExpression() { _operator = type; };

	virtual ~IOperationalExpression() {};

	Operator_type get_operator() { return _operator; };
private:
	Operator_type _operator;
};

class AddExpression : public IOperationalExpression
{
public:
	AddExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~AddExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const;
private:
	Gpl_type _type;
};

class MinusExpression : public IOperationalExpression
{
public:
	MinusExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~MinusExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const;
private:
	Gpl_type _type;
};


class MultiplyExpression : public IOperationalExpression
{
public:
	MultiplyExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~MultiplyExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const;
private:
	Gpl_type _type;
};

class DivideExpression : public IOperationalExpression
{
public:
	DivideExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~DivideExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const;
private:
	Gpl_type _type;
};

class ModExpression : public IOperationalExpression
{
public:
	ModExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~ModExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const;
private:
	Gpl_type _type;
};

class SinExpression : public IOperationalExpression
{
public:
	SinExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~SinExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return DOUBLE; };
};

class CosExpression : public IOperationalExpression
{
public:
	CosExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~CosExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return DOUBLE; };
};

class TanExpression : public IOperationalExpression
{
public:
	TanExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~TanExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return DOUBLE; };
};


class AsinExpression : public IOperationalExpression
{
public:
	AsinExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~AsinExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return DOUBLE; };
};

class AcosExpression : public IOperationalExpression
{
public:
	AcosExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~AcosExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return DOUBLE; };
};


class AtanExpression : public IOperationalExpression
{
public:
	AtanExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~AtanExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return DOUBLE; };
};


class SqrtExpression : public IOperationalExpression
{
public:
	SqrtExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~SqrtExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return DOUBLE; };
};



class FloorExpression : public IOperationalExpression
{
public:
	FloorExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~FloorExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};


class AbsoluteExpression : public IOperationalExpression
{
public:
	AbsoluteExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~AbsoluteExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return _type; };
private:
	Gpl_type _type;
};

class RandomExpression : public IOperationalExpression
{
public:
	RandomExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~RandomExpression();
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};

class EqualExpression : public IOperationalExpression
{
public:
	EqualExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~EqualExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};

class NotEqualExpression : public IOperationalExpression
{
public:
	NotEqualExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~NotEqualExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};


class LessThanExpression : public IOperationalExpression
{
public:
	LessThanExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~LessThanExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};


class LessThanEqualExpression : public IOperationalExpression
{
public:
	LessThanEqualExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~LessThanEqualExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};


class GreaterThanExpression : public IOperationalExpression
{
public:
	GreaterThanExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~GreaterThanExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};


class GreaterThanEqualExpression : public IOperationalExpression
{
public:
	GreaterThanEqualExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~GreaterThanEqualExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};


class AndExpression : public IOperationalExpression
{
public:
	AndExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~AndExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};


class OrExpression : public IOperationalExpression
{
public:
	OrExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~OrExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};

class NotExpression : public IOperationalExpression
{
public:
	NotExpression(std::shared_ptr<IExpression> pArg1);
	virtual ~NotExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};

class TouchesExpression : public IExpression
{
public:
	TouchesExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~TouchesExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};

class NearExpression : public IExpression
{
public:
	NearExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2);
	virtual ~NearExpression() {};
	std::shared_ptr<IValue> eval() const;
	Gpl_type get_type() const { return INT; };
};

#endif
