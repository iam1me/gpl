#include <cmath>
#include <random>
#include <chrono>
#include "expression.h"
#include "symbol_table.h"
#include "gpl_exception.h"
#include "parser.h"

#define PI 3.14159265
#define CONVERT_TO_RADIANS(deg) (deg)*PI/180
#define CONVERT_TO_DEGREES(rad) (rad)*180/PI
	

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

	if(ndx_expr->get_type() != INT)
	{
		throw invalid_index_type(array_name, ndx_expr->get_type());
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
		throw index_out_of_bounds(_arrayName, ndx_val->get_int());
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

	//TRACE_VERBOSE("MINUS Type: " << gpl_type_to_string(_type))
	if(_type == INT)
	{
		int result = pval1->get_int() - pval2->get_int();
		pret.reset(new ConstantValue(result));
	}
	else // DOUBLE
	{
		double result = pval1->get_double() - pval2->get_double();
		pret.reset(new ConstantValue(result));
	}
	
	//TRACE_VERBOSE("MINUS(" << pval1->get_string() << ", " << pval2->get_string()
	//		<< "): " << pret->get_string())
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
		int result = pval1->get_int() * pval2->get_int();
		pret.reset(new ConstantValue(result));
	}
	else
	{
		double result = pval1->get_double() * pval2->get_double();
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
			break;
		default:
			TRACE_ERROR("DivideExpression: Invalid right operand type " << gpl_type_to_string(pArg2->get_type()))
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
		throw invalid_operand_type(SIN, false);

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

CosExpression::CosExpression(std::shared_ptr<IExpression> pArg)
	: IOperationalExpression(COS)
{
	if(!pArg) throw std::invalid_argument("Argument cannot be NULL");

	Gpl_type type = pArg->get_type();
	if(type != INT && type != DOUBLE)
		throw invalid_operand_type(COS, false);

	add_child(pArg);
}

CosExpression::~CosExpression()
{}

std::shared_ptr<IValue> CosExpression::eval() const
{
	std::shared_ptr<IValue> pval = get_child(0)->eval();
	
	double result = CONVERT_TO_RADIANS(pval->get_double());
	result = cos(result);

	std::shared_ptr<IValue>pret(new ConstantValue(result));
	return pret;
}


TanExpression::TanExpression(std::shared_ptr<IExpression> pArg)
	: IOperationalExpression(TAN)
{
	if(!pArg) throw std::invalid_argument("Argument cannot be NULL");

	Gpl_type type = pArg->get_type();
	if(!(type & (INT | DOUBLE)))
		throw invalid_operand_type(TAN, false);

	add_child(pArg);
}

TanExpression::~TanExpression()
{}

std::shared_ptr<IValue> TanExpression::eval() const
{
	std::shared_ptr<IValue> pval = get_child(0)->eval();
	double result = CONVERT_TO_RADIANS(pval->get_double());
	result = tan(result);
	return std::shared_ptr<IValue>(new ConstantValue(result));
}

AsinExpression::AsinExpression(std::shared_ptr<IExpression> pArg)
	: IOperationalExpression(ASIN)
{
	if(!pArg) throw std::invalid_argument("Argument cannot be NULL");

	Gpl_type type = pArg->get_type();
	if(!(type & (INT | DOUBLE)))
		throw invalid_operand_type(ASIN, false);

	add_child(pArg);
}

AsinExpression::~AsinExpression()
{}

std::shared_ptr<IValue> AsinExpression::eval() const
{
	std::shared_ptr<IValue> pval = get_child(0)->eval();
	double result = pval->get_double();
	result = asin(result);
	result = CONVERT_TO_DEGREES(result);
	return std::shared_ptr<IValue>(new ConstantValue(result));
}

AcosExpression::AcosExpression(std::shared_ptr<IExpression> pArg)
	: IOperationalExpression(ACOS)
{
	if(!pArg) throw std::invalid_argument("Argument cannot be NULL");

	Gpl_type type = pArg->get_type();
	if(!(type & (INT | DOUBLE)))
		throw invalid_operand_type(ACOS, false);

	add_child(pArg);
}

AcosExpression::~AcosExpression()
{}

std::shared_ptr<IValue> AcosExpression::eval() const
{
	std::shared_ptr<IValue> pval = get_child(0)->eval();
	double result = pval->get_double();
	result = acos(result);
	result = CONVERT_TO_DEGREES(result);
	return std::shared_ptr<IValue>(new ConstantValue(result));
}

AtanExpression::AtanExpression(std::shared_ptr<IExpression> pArg)
	: IOperationalExpression(ATAN)
{
	if(!pArg) throw std::invalid_argument("Argument cannot be NULL");

	Gpl_type type = pArg->get_type();
	if(!(type & (INT | DOUBLE)))
		throw invalid_operand_type(ATAN, false);

	add_child(pArg);
}

AtanExpression::~AtanExpression()
{}

std::shared_ptr<IValue> AtanExpression::eval() const
{
	std::shared_ptr<IValue> pval = get_child(0)->eval();
	double result = pval->get_double();
	result = atan(result);
	result = CONVERT_TO_DEGREES(result);
	return std::shared_ptr<IValue>(new ConstantValue(result));
}

SqrtExpression::SqrtExpression(std::shared_ptr<IExpression> pArg)
	: IOperationalExpression(SQRT)
{
	if(!pArg) throw std::invalid_argument("Argument cannot be NULL");

	Gpl_type type = pArg->get_type();
	if(!(type & (INT | DOUBLE)))
		throw invalid_operand_type(SQRT, false);

	add_child(pArg);
}

SqrtExpression::~SqrtExpression()
{}

std::shared_ptr<IValue> SqrtExpression::eval() const
{
	std::shared_ptr<IValue> pval = get_child(0)->eval();
	double result = sqrt(pval->get_double());

	TRACE_VERBOSE("SQRT(" << pval->get_double() << "): " << result);
	return std::shared_ptr<IValue>(new ConstantValue(result));
}


FloorExpression::FloorExpression(std::shared_ptr<IExpression> pArg)
	: IOperationalExpression(FLOOR)
{
	if(!pArg) throw std::invalid_argument("Argument cannot be NULL");

	Gpl_type type = pArg->get_type();
	if(!(type & (INT | DOUBLE)))
		throw invalid_operand_type(FLOOR, false);

	add_child(pArg);
}

FloorExpression::~FloorExpression()
{}

std::shared_ptr<IValue> FloorExpression::eval() const
{
	std::shared_ptr<IValue> pval = get_child(0)->eval();
	int result = floor(pval->get_double());
	return std::shared_ptr<IValue>(new ConstantValue(result));
}


AbsoluteExpression::AbsoluteExpression(std::shared_ptr<IExpression> pArg)
	: IOperationalExpression(ABS)
{
	if(!pArg) throw std::invalid_argument("Argument cannot be NULL");

	_type = pArg->get_type();
	if(!(_type & (INT | DOUBLE)))
		throw invalid_operand_type(ABS, false);

	TRACE_VERBOSE("AbsoluteExpression::AbsoluteExpression - Type: " << gpl_type_to_string(_type))
	add_child(pArg);
}

AbsoluteExpression::~AbsoluteExpression()
{}

std::shared_ptr<IValue> AbsoluteExpression::eval() const
{
	std::shared_ptr<IValue> pval = get_child(0)->eval();
	std::shared_ptr<IValue> pret = nullptr;	

	if(_type == INT)
	{
		int result= pval->get_int();
		result = std::abs(result);

		TRACE_VERBOSE("ABS(" << pval->get_int() << ") = " << result);
		pret.reset(new ConstantValue(result));
	}
	else // DOUBLE
	{
		double result = pval->get_double();
		result = std::abs(result);

		TRACE_VERBOSE("ABS(" << pval->get_double() << ") = " << result);
		pret.reset(new ConstantValue(result));
	}
	return pret;
}


RandomExpression::RandomExpression(std::shared_ptr<IExpression> pArg)
	: IOperationalExpression(RANDOM)
{
	if(!pArg) throw std::invalid_argument("Argument cannot be NULL");

	Gpl_type type = pArg->get_type();
	if(!(type & (INT | DOUBLE)))
		throw invalid_operand_type(RANDOM, false);

	add_child(pArg);
}

RandomExpression::~RandomExpression()
{}

std::shared_ptr<IValue> RandomExpression::eval() const
{
	static unsigned seed = 
		std::chrono::system_clock::now().time_since_epoch().count();

	static std::default_random_engine generator(seed);

	std::shared_ptr<IValue> pval = get_child(0)->eval();

	int result = abs(floor(pval->get_double()));
	std::uniform_int_distribution<int> distribution(0, result? result - 1 : 0);
	result = distribution(generator);

	return std::shared_ptr<IValue>(new ConstantValue(result));
}


EqualExpression::EqualExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(EQUAL)
{
	if(!pArg1 || !pArg2) throw std::invalid_argument("Argument cannot be NULL");

	if(!(pArg1->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(EQUAL, true);

	if(!(pArg2->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(EQUAL, false);

	add_child(pArg1);
	add_child(pArg2);
}

std::shared_ptr<IValue> EqualExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();
	std::shared_ptr<IValue> pret = nullptr;

	int result = 0;

	Gpl_type t1 = pval1->get_type();
	Gpl_type t2 = pval2->get_type();
	if((t1 | t2) & STRING)
	{
		std::string str1 = pval1->get_string();
		std::string str2 = pval2->get_string();
		result = (str1 == str2);

		TRACE_VERBOSE("EQUAL('" << str1 << "', '" << str2 << "') = " << result)
	}
	else
	{
		double dbl1 = pval1->get_double();
		double dbl2 = pval2->get_double();
		result = (dbl1 == dbl2);

		TRACE_VERBOSE("EQUAL(" << dbl1 << ", " << dbl2 << ") = " << result)
	}
	
	pret.reset(new ConstantValue(result));
	return pret;
}


NotEqualExpression::NotEqualExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(NOT_EQUAL)
{
	if(!pArg1 || !pArg2) throw std::invalid_argument("Argument cannot be NULL");

	if(!(pArg1->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(NOT_EQUAL, true);

	if(!(pArg2->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(NOT_EQUAL, false);

	add_child(pArg1);
	add_child(pArg2);
}

std::shared_ptr<IValue> NotEqualExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();
	std::shared_ptr<IValue> pret = nullptr;

	int result = 0;

	Gpl_type t1 = pval1->get_type();
	Gpl_type t2 = pval2->get_type();
	if((t1 | t2) & STRING)
	{
		std::string str1 = pval1->get_string();
		std::string str2 = pval2->get_string();
		result = (str1 != str2);
	}
	else
	{
		double dbl1 = pval1->get_double();
		double dbl2 = pval2->get_double();
		result = (dbl1 != dbl2);
	}
	
	pret.reset(new ConstantValue(result));
	return pret;
}


LessThanExpression::LessThanExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(LESS_THAN)
{
	if(!pArg1 || !pArg2) throw std::invalid_argument("Argument cannot be NULL");

	if(!(pArg1->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(LESS_THAN, true);

	if(!(pArg2->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(LESS_THAN, false);

	add_child(pArg1);
	add_child(pArg2);
}

std::shared_ptr<IValue> LessThanExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();
	std::shared_ptr<IValue> pret = nullptr;

	int result = 0;

	Gpl_type t1 = pval1->get_type();
	Gpl_type t2 = pval2->get_type();
	if((t1 | t2) & STRING)
	{
		std::string str1 = pval1->get_string();
		std::string str2 = pval2->get_string();
		result = (str1 < str2);
	}
	else
	{
		double dbl1 = pval1->get_double();
		double dbl2 = pval2->get_double();
		result = (dbl1 < dbl2);
	}
	
	pret.reset(new ConstantValue(result));
	return pret;
}


LessThanEqualExpression::LessThanEqualExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(LESS_THAN_EQUAL)
{
	if(!pArg1 || !pArg2) throw std::invalid_argument("Argument cannot be NULL");

	if(!(pArg1->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(LESS_THAN_EQUAL, true);

	if(!(pArg2->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(LESS_THAN_EQUAL, false);

	add_child(pArg1);
	add_child(pArg2);
}

std::shared_ptr<IValue> LessThanEqualExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();
	std::shared_ptr<IValue> pret = nullptr;

	int result = 0;

	Gpl_type t1 = pval1->get_type();
	Gpl_type t2 = pval2->get_type();
	if((t1 | t2) & STRING)
	{
		std::string str1 = pval1->get_string();
		std::string str2 = pval2->get_string();
		result = (str1 <= str2);
	}
	else
	{
		double dbl1 = pval1->get_double();
		double dbl2 = pval2->get_double();
		result = (dbl1 <= dbl2);
	}
	
	pret.reset(new ConstantValue(result));
	return pret;
}


GreaterThanExpression::GreaterThanExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(GREATER_THAN)
{
	if(!pArg1 || !pArg2) throw std::invalid_argument("Argument cannot be NULL");

	if(!(pArg1->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(GREATER_THAN, true);

	if(!(pArg2->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(GREATER_THAN, false);

	add_child(pArg1);
	add_child(pArg2);
}

std::shared_ptr<IValue> GreaterThanExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();
	std::shared_ptr<IValue> pret = nullptr;

	int result = 0;

	Gpl_type t1 = pval1->get_type();
	Gpl_type t2 = pval2->get_type();
	if((t1 | t2) & STRING)
	{
		std::string str1 = pval1->get_string();
		std::string str2 = pval2->get_string();
		result = (str1 > str2);
	}
	else
	{
		double dbl1 = pval1->get_double();
		double dbl2 = pval2->get_double();
		result = (dbl1 > dbl2);
	}
	
	pret.reset(new ConstantValue(result));
	return pret;
}


GreaterThanEqualExpression::GreaterThanEqualExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(GREATER_THAN_EQUAL)
{
	if(!pArg1 || !pArg2) throw std::invalid_argument("Argument cannot be NULL");

	if(!(pArg1->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(GREATER_THAN_EQUAL, true);

	if(!(pArg2->get_type() & (INT | DOUBLE | STRING)))
		throw invalid_operand_type(GREATER_THAN_EQUAL, false);

	add_child(pArg1);
	add_child(pArg2);
}

std::shared_ptr<IValue> GreaterThanEqualExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();
	std::shared_ptr<IValue> pret = nullptr;

	int result = 0;

	Gpl_type t1 = pval1->get_type();
	Gpl_type t2 = pval2->get_type();
	if((t1 | t2) & STRING)
	{
		std::string str1 = pval1->get_string();
		std::string str2 = pval2->get_string();
		result = (str1 >= str2);
	}
	else
	{
		double dbl1 = pval1->get_double();
		double dbl2 = pval2->get_double();
		result = (dbl1 >= dbl2);
	}
	
	pret.reset(new ConstantValue(result));
	return pret;
}


AndExpression::AndExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(AND)
{
	if(!pArg1 || !pArg2) throw std::invalid_argument("Argument cannot be NULL");

	if(!(pArg1->get_type() & (INT | DOUBLE)))
		throw invalid_operand_type(AND, true);

	if(!(pArg2->get_type() & (INT | DOUBLE)))
		throw invalid_operand_type(AND, false);

	add_child(pArg1);
	add_child(pArg2);
}

std::shared_ptr<IValue> AndExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();

	double dbl1 = pval1->get_double();
	double dbl2 = pval2->get_double();
	int result = (dbl1 && dbl2);
		
	std::shared_ptr<IValue> pret(new ConstantValue(result));
	return pret;
}


OrExpression::OrExpression(std::shared_ptr<IExpression> pArg1, std::shared_ptr<IExpression> pArg2)
	: IOperationalExpression(OR)
{
	if(!pArg1 || !pArg2) throw std::invalid_argument("Argument cannot be NULL");

	if(!(pArg1->get_type() & (INT | DOUBLE)))
		throw invalid_operand_type(OR, true);

	if(!(pArg2->get_type() & (INT | DOUBLE)))
		throw invalid_operand_type(OR, false);

	add_child(pArg1);
	add_child(pArg2);
}

std::shared_ptr<IValue> OrExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	std::shared_ptr<IValue> pval2 = get_child(1)->eval();

	double dbl1 = pval1->get_double();
	double dbl2 = pval2->get_double();
	int result = (dbl1 || dbl2);
		
	std::shared_ptr<IValue> pret(new ConstantValue(result));
	return pret;
}


NotExpression::NotExpression(std::shared_ptr<IExpression> pArg)
	: IOperationalExpression(NOT)
{
	if(!pArg) throw std::invalid_argument("Argument cannot be NULL");

	if(!(pArg->get_type() & (INT | DOUBLE)))
		throw invalid_operand_type(NOT, false);

	add_child(pArg);
}

std::shared_ptr<IValue> NotExpression::eval() const
{
	std::shared_ptr<IValue> pval1 = get_child(0)->eval();
	
	double dbl1 = pval1->get_double();
	int result = !dbl1;
		
	std::shared_ptr<IValue> pret(new ConstantValue(result));
	return pret;
}


