#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>

#include "GPLVariant.h"
#include "value.h"


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
	virtual IValue eval() = 0 const;

	virtual int get_child_count() const;
	virtual const IExpression* get_child(int ndx) const;

protected:
	typedef std::vector<IExpression*> ExpressionList;
	virtual const ExpressionList& get_children() const = 0;
};

// Holds a value. a Leaf node. Values maybe constants or references.
class ValueExpression : public IExpression
{};

// This type of Expression is configured to perform some kind of operation
// on its child Expressions after they have been evaluated.
class OperativeExpression : public IExpression
{

};

// This type of Expression is configured to use the evaluated values of its
// children as the inputs for a function
class FunctionalExpression : public IExpression
{

};

#endif
