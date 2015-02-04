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
class ValueExpression;
class ArrayReferenceExpression;
class MemberReferenceExpression; //Implement later
class OperationalExpression;

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
	virtual const ExpressionList& get_children() const = 0;
};


// Holds a value. a Leaf node. Values maybe constants or direct references.
class ValueExpression : public IExpression
{
public: 
	ValueExpression(std::shared_ptr<IValue>);
	~ValueExpression();

	Gpl_type get_type() const;
	std::shared_ptr<IValue> eval() const;
	
protected:
	const ExpressionList& get_children() const;

private:
	ExpressionList _children;
	std::shared_ptr<IValue> _pVal;	
};


// Upon evaluation, ReferenceExpression determines which symbol to address
// and returns its value. This is used for calculating array references 
// dynamically. For example: y = myarray[x + 1]; 
class ArrayReferenceExpression : public IExpression
{
public:
	ArrayReferenceExpression(std::string array_name, 
				std::shared_ptr<IExpression>);
	~ArrayReferenceExpression();

	Gpl_type get_type() const;

	std::shared_ptr<IValue> eval() const;
	std::string get_arrayName() const;

protected:
	const ExpressionList& get_children() const;

private:
	ExpressionList _children;
	std::shared_ptr<IValue> _pVal;
	std::string _arrayName;
	Gpl_type _type;
};


class OperationalExpression
{
public:
	OperationalExpression(std::shared_ptr<IExpression> pArg1, Operator_type op);
	OperationalExpression(std::shared_ptr<IExpression> pArg1, Operator_type op, std::shared_ptr<IExpression> pArg2);
	~OperationalExpression();

	std::shared_ptr<IValue> eval() const;
protected:
	const ExpressionList& get_children();

private:
	bool _bUnary;
	ExpressionList _children;
	Gpl_type _type;
	Operator_type _op;
};

#endif
