#include <math.h>
#include "expression.h"
#include "symbol_table.h"
#include "gpl_exception.h"
#include "parser.h"

#define PI 3.14159265
#define CONVERT_TO_RADIANS(deg) (deg)*PI/180
	

IExpression::~IExpression()
{
}

int IExpression::get_child_count() const
{
	return _children.size();
}
	
std::shared_ptr<IExpression> IExpression::get_child(int ndx) const
{
	if(ndx < 0 || ndx >= (int) _children.size())
	{
		throw std::invalid_argument("ndx out of bounds");
	}

	return _children[ndx];	
}

ExpressionList& IExpression::get_children()
{
	return _children;
}

void IExpression::add_child(std::shared_ptr<IExpression> child)
{
	_children.push_back(child);
}

ValueExpression::ValueExpression(std::shared_ptr<IValue> val)
	: IExpression()
{
	if(!val)
	{
		throw std::invalid_argument("val is NULL");
	}

	_pVal = val;
}

ValueExpression::~ValueExpression()
{}

std::shared_ptr<IValue> ValueExpression::eval() const
{
	//TRACE_VERBOSE("ValueExpression::eval()")
	return _pVal;
}
	
Gpl_type ValueExpression::get_type() const
{
	return _pVal->get_type();
}

ArrayReferenceExpression::ArrayReferenceExpression
	(std::string array_name, std::shared_ptr<IExpression> ndx_expr)
{
	_arrayName = array_name;

	std::string reference = _arrayName + "[0]";
	std::shared_ptr<Symbol> pSymbol = 
			Symbol_table::instance()->find_symbol(reference);

	if(!pSymbol)
	{
		throw std::runtime_error("Undefined Array Name");
	}

	_type = pSymbol->get_type();
	add_child(ndx_expr);
}

ArrayReferenceExpression::~ArrayReferenceExpression()
{
	
}

std::shared_ptr<IValue> ArrayReferenceExpression::eval() const
{	
	TRACE_VERBOSE("ArrayReferenceExpression::eval()")
	std::shared_ptr<IExpression> ndx_expr = get_child(0);
	const std::shared_ptr<IValue>& ndx_val = ndx_expr->eval();

	std::string reference = _arrayName + "[";
	reference += std::to_string(ndx_val->get_int()) + "]";
					
	std::shared_ptr<Symbol> pSymbol = 
			Symbol_table::instance()->find_symbol(reference);

	if(!pSymbol)
	{
		throw index_out_of_bounds();
	}

	TRACE_VERBOSE("ArrayReferenceExpression::eval() - Symbol found. Returning ReferenceValue")
	std::shared_ptr<IValue> ret(new ReferenceValue(pSymbol));
	return ret;
}

std::string ArrayReferenceExpression::get_arrayName() const
{
	return _arrayName;
}
	
Gpl_type ArrayReferenceExpression::get_type() const
{
	return _type;
}

AddExpression::AddExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(PLUS)
{
	if(!pArg1 || !pArg2)
	{
		throw std::invalid_argument("Argument cannot be NULL");
	}

	_type = pArg1->get_type();
	if(_type != INT && _type != DOUBLE && _type != STRING)
	{
		throw invalid_operand_type(PLUS, true);
	}

	switch(pArg2->get_type())
	{
		case INT:
			break;
		case DOUBLE:
			if(_type == INT) _type = DOUBLE;
			break;
		case STRING:
			_type = STRING;
			break;
		default:
			throw invalid_operand_type(PLUS, false);
	}

	add_child(pArg1);
	add_child(pArg2);
}

AddExpression::~AddExpression() 
{}

std::shared_ptr<IValue> AddExpression::eval() const
{
	std::shared_ptr<IValue> val1 = get_child(0)->eval();
	std::shared_ptr<IValue> val2 = get_child(1)->eval();
	std::shared_ptr<IValue> ret = nullptr;
	
	switch(_type)
	{
		case INT:
		{
			int result = val1->get_int() + val2->get_int();
			ret.reset(new ConstantValue(result));
			break;
		}
		case DOUBLE:
		{
			double result = val1->get_double() + val2->get_double();
			ret.reset(new ConstantValue(result));
			break;
		}
		case STRING:
		{
			std::string result = val1->get_string() + val2->get_string();
			ret.reset(new ConstantValue(result));
			break;
		}
		default:
			throw std::logic_error("AddExpression::eval() - Invalid Type: " + gpl_type_to_string(_type));	
	}

	return ret;
}

Gpl_type AddExpression::get_type() const
{
	return _type;
}

MinusExpression::MinusExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(MINUS)
{
	if(!pArg1 || !pArg2)
	{
		throw std::invalid_argument("Argument cannot be NULL");
	}

	_type = pArg1->get_type();
	if(_type != INT && _type != DOUBLE)
	{
		throw invalid_operand_type(MINUS, true);
	}

	switch(pArg2->get_type())
	{
		case INT:
			break;

		case DOUBLE:
			_type = DOUBLE;
			break;

		default:
			throw invalid_operand_type(MINUS, false);
	}

	add_child(pArg1);
	add_child(pArg2);
}

MinusExpression::~MinusExpression()
{}

std::shared_ptr<IValue> MinusExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();
	std::shared_ptr<IValue> pret = nullptr;

	if(_type == INT)
	{
		int result = pval1->get_int() + pval2->get_int();
		pret.reset(new ConstantValue(result));
	}
	else // DOUBLE
	{
		double result = pval2->get_double() + pval2->get_double();
		pret.reset(new ConstantValue(result));
	}

	return pret;
}

Gpl_type MinusExpression::get_type() const
{
	return _type;
}


MultiplyExpression::MultiplyExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(MULTIPLY)
{
	if(!pArg1 || !pArg2) throw std::invalid_argument("Argument cannot be NULL");

	_type = pArg1->get_type();
	if(_type != INT && _type != DOUBLE)
	{
		throw invalid_operand_type(MULTIPLY, true);
	}

	switch(pArg2->get_type())
	{
		case INT:
			break;
		case DOUBLE:
			_type = DOUBLE;
			break;
		default:
			throw invalid_operand_type(MULTIPLY, false);
	}
	
	add_child(pArg1);
	add_child(pArg2);
}

MultiplyExpression::~MultiplyExpression()
{}

std::shared_ptr<IValue> MultiplyExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();
	std::shared_ptr<IValue> pret = nullptr;

	if(_type == INT)
	{
		int result = pval1->get_int() + pval2->get_int();
		pret.reset(new ConstantValue(result));
	}
	else
	{
		double result = pval1->get_double() + pval2->get_double();
		pret.reset(new ConstantValue(result));
	}

	return pret;
}

Gpl_type MultiplyExpression::get_type() const
{
	return _type;
}
	
	
DivideExpression::DivideExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(DIVIDE)
{
	if(!pArg1 || !pArg2) throw std::invalid_argument("Argument cannot be NULL");

	_type = pArg1->get_type();
	if(_type != INT && _type != DOUBLE)
	{
		throw invalid_operand_type(DIVIDE, true);
	}

	switch(pArg2->get_type())
	{
		case INT:
			break;
		case DOUBLE:
			_type = DOUBLE;
		default:
			throw invalid_operand_type(DIVIDE, false);
	}

	add_child(pArg1);
	add_child(pArg2);
}

DivideExpression::~DivideExpression()
{}

std::shared_ptr<IValue> DivideExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();
	std::shared_ptr<IValue> pret = nullptr;

	if(pval2->get_double() == 0)
	{
		// On Divide By Zero - just return zero
		Error::error(Error::DIVIDE_BY_ZERO_AT_PARSE_TIME);
		if(_type == INT) pret.reset(new ConstantValue(0));
		else pret.reset(new ConstantValue(0.0));	
	}
	else if(_type == INT)
	{
		int result = pval1->get_int() / pval2->get_int();
		pret.reset(new ConstantValue(result));
	}
	else
	{
		double result = pval1->get_double() / pval2->get_double();
		pret.reset(new ConstantValue(result));
	}	

	return pret;
}

Gpl_type DivideExpression::get_type() const
{
	return _type;
}

ModExpression::ModExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	:  IOperationalExpression(MOD)
{
	if(!pArg1 || !pArg2) 
		throw std::invalid_argument("Argument cannot be NULL");

	if(pArg1->get_type() != INT) 
		throw invalid_operand_type(MOD, true);

	if(pArg2->get_type() != INT)
		throw invalid_operand_type(MOD, false);

	_type = INT;
	add_child(pArg1);
	add_child(pArg2);
}

ModExpression::~ModExpression()
{}

std::shared_ptr<IValue> ModExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();
	std::shared_ptr<IValue> pret = nullptr;
	
	if(pval2->get_int() == 0)
	{
		Error::error(Error::MOD_BY_ZERO_AT_PARSE_TIME);
		pret.reset(new ConstantValue(0));
	}
	else
	{
		pret.reset(new ConstantValue(pval1->get_int() % pval2->get_int()));
	}

	return pret;
}

Gpl_type ModExpression::get_type() const
{
	return _type;
}


SinExpression::SinExpression(std::shared_ptr<IExpression> pArg1)
	: IOperationalExpression(SIN)
{
	if(!pArg1) throw std::invalid_argument("Argument cannot be NULL");

	Gpl_type type = pArg1->get_type();
	if(type != INT && type != DOUBLE)
		throw invalid_operand_type(SIN, true);

	add_child(pArg1);
}

SinExpression::~SinExpression()
{}

std::shared_ptr<IValue> SinExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pret = nullptr;

	double rads = CONVERT_TO_RADIANS(pval1->get_double());
	double result = sin(rads);
	pret.reset(new ConstantValue(result));

	return pret;
}

