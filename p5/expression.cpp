#include "expression.h"
#include "symbol_table.h"
#include "gpl_exception.h"
#include "parser.h"

IExpression::~IExpression()
{
}

int IExpression::get_child_count() const
{
	const ExpressionList& children = get_children();
	return children.size();
}
	
std::shared_ptr<IExpression> IExpression::get_child(int ndx) const
{
	const ExpressionList& children = get_children();
	if(ndx < 0 || ndx >= (int) children.size())
	{
		throw std::invalid_argument("ndx out of bounds");
	}

	return children[ndx];	
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
	TRACE_VERBOSE("ValueExpression::eval()")
	return _pVal;
}

const ExpressionList& ValueExpression::get_children() const
{
	return _children;
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
	_children.push_back(ndx_expr);
}

ArrayReferenceExpression::~ArrayReferenceExpression()
{
	
}

std::shared_ptr<IValue> ArrayReferenceExpression::eval() const
{	
	TRACE_VERBOSE("ArrayReferenceExpression::eval()")
	const std::shared_ptr<IExpression>& ndx_expr = _children[0];
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

const ExpressionList& ArrayReferenceExpression::get_children() const
{
	return _children;
}


std::string ArrayReferenceExpression::get_arrayName() const
{
	return _arrayName;
}
	
Gpl_type ArrayReferenceExpression::get_type() const
{
	return _type;
}

OperationalExpression::OperationalExpression(std::shared_ptr<IExpression> pArg1, Operator_type op)
	: IExpression()
{
	if(!pArg1)
	{
		throw std::invalid_argument("pArg1 is null");
	}

	Gpl_type arg_type = pArg1->get_type();

	// Confirm this is a unary operator or a function that takes a single parameter like sin(expr)
	// also confirm that the type of the sub-expression is OK
	switch(op)
	{
		case UNARY_MINUS:
			if(arg_type != INT && arg_type != DOUBLE)
			{
				throw invalid_operand_type(op, true);
			}
			_type = arg_type;
			break;

		case NOT:
			if(arg_type != INT)
			{
				throw invalid_operand_type(op, true);
			}
			_type = INT;
			break;

		case SIN:
		case COS:
		case TAN:
		case ASIN:
		case ACOS:
		case ATAN:
		case SQRT:
			if(arg_type != INT && arg_type != DOUBLE)
			{
				throw invalid_operand_type(op, true);
			}
			_type = DOUBLE;
			break;

		case FLOOR:
		case ABS:
			if(arg_type != INT && arg_type != DOUBLE)
			{
				throw invalid_operand_type(op, true);
			}
			_type = INT;
			break;
		
		case RANDOM:
			if(arg_type != INT && arg_type != DOUBLE)
			{
				throw invalid_operand_type(op, true);
			}
			_type = INT;
			break;

		default:
			throw invalid_operand_type(op, true);
	}

	_children.push_back(pArg1);
	_op = op;
	_bUnary = true;
}

OperationalExpression::OperationalExpression (std::shared_ptr<IExpression> pArg1, 
		Operator_type op, std::shared_ptr<IExpression> pArg2)
	: IExpression()
{
	_bUnary = false;
	if(!pArg1 || !pArg2)
	{
		throw std::invalid_argument("Argument is NULL");
	}

	Gpl_type arg1_type = pArg1->get_type();
	Gpl_type arg2_type = pArg2->get_type();

	// Confirm that the selected Operation takes two arguments
	// Confirm that the arguments are of the right type(s)
	switch(op)
	{
		case PLUS:
			if(arg1_type == STRING && (arg2_type == INT || arg2_type == DOUBLE || arg2_type == STRING))
			{
				_type = STRING;
			}
			else if(arg2_type == STRING && (arg1_type == INT || arg1_type == DOUBLE || arg1_type == STRING))
			{
				_type = STRING;
			}
			else if(arg1_type == DOUBLE && (arg2_type == INT || arg2_type == DOUBLE))
			{
				_type = DOUBLE;
			}
			else if(arg2_type == DOUBLE && (arg1_type == INT || arg1_type == DOUBLE))
			{
				_type = DOUBLE;
			}
			else if(arg1_type == INT && (arg2_type == INT))
			{
				_type = INT;
			}
			else if(arg1_type == GAME_OBJECT ||
					arg1_type == ANIMATION_BLOCK)
			{
				throw invalid_operand_type(op, true);
			}
			else if(arg2_type == GAME_OBJECT ||
					arg2_type == ANIMATION_BLOCK)
			{
				throw invalid_operand_type(op, false);
			}
			else
			{
				throw std::logic_error("Invalid PLUS usage detected");
			}
			break;

		case MINUS:
		case MULTIPLY:
		case DIVIDE:
			if(arg1_type == DOUBLE && (arg2_type == INT || arg2_type == DOUBLE))
			{
				_type = DOUBLE;
			}
			else if(arg2_type == DOUBLE && (arg1_type == INT || arg1_type == DOUBLE))
			{
				_type = DOUBLE;
			}
			else if(arg1_type == INT && arg2_type == INT)
			{
				_type = INT;
			}
			else if(arg1_type != INT && arg1_type != DOUBLE)
			{
				throw invalid_operand_type(op, true);
			}
			else
			{
				throw invalid_operand_type(op, false);
			}
			break;

		case MOD:
			if(arg1_type != INT)
			{
				throw invalid_operand_type(op, true);
			}
			else if(arg2_type != INT)
			{
				throw invalid_operand_type(op, false);
			}
			_type = INT;
			break;

		case AND:
		case OR:
		case LESS_THAN:
		case LESS_THAN_EQUAL:
		case GREATER_THAN:
		case GREATER_THAN_EQUAL:
			if(arg1_type == GAME_OBJECT || arg1_type == ANIMATION_BLOCK)
			{
				throw invalid_operand_type(op, true);
			}
			else if(arg2_type == GAME_OBJECT || arg2_type == ANIMATION_BLOCK)
			{
				throw invalid_operand_type(op, false);
			}
			_type = INT;
			break;

		case EQUAL:
		case NOT_EQUAL:
			if(arg1_type == GAME_OBJECT || arg1_type == ANIMATION_BLOCK)
			{
				throw invalid_operand_type(op, true);
			}
			else if(arg2_type == GAME_OBJECT || arg2_type == ANIMATION_BLOCK)
			{
				throw invalid_operand_type(op, false);
			}
			_type = INT;
			break;

		case NEAR:
		case TOUCHES:
			if(arg1_type != GAME_OBJECT)
			{
				throw object_operand_expected();
			}
			else if(arg2_type != GAME_OBJECT)
			{
				throw object_operand_expected();
			}
			throw std::logic_error("NEAR/TOUCHES Operators are not yet implemented");
			_type = INT;
			break;

		default:
			// Any other operator requires 2 operands
			throw invalid_operand_type(op, false);
	}

	_op = op;
	_children.push_back(pArg1);
	_children.push_back(pArg2);
}

OperationalExpression::~OperationalExpression()
{}

const ExpressionList& OperationalExpression::get_children() const
{
	return _children;
}

Gpl_type OperationalExpression::get_type() const
{
	return _type;
}

std::shared_ptr<IValue> OperationalExpression::eval() const
{
	TRACE_VERBOSE("OperationalExpression::eval() - Operator: '" << operator_to_string(_op)
			<< "' Type: " << gpl_type_to_string(get_type()))

	std::shared_ptr<IValue> pret = nullptr;

	std::shared_ptr<IValue> pArg1, pArg2;
	pArg1 = _children[0]->eval();
	TRACE_VERBOSE("OperationalExpression::eval() - Arg1 Type: " << gpl_type_to_string(pArg1->get_type()))

	if(!_bUnary)
	{
		pArg2 = _children[1]->eval();
		TRACE_VERBOSE("OperationalExpression::eval() - Arg2 Type: " << gpl_type_to_string(pArg2->get_type()))
	}

	std::shared_ptr<GPLVariant> pvar;
	switch(_op)
	{
		case PLUS:
			switch(_type)
			{
				case INT:
				{
					int temp = pArg1->get_int();
					temp += pArg2->get_int();
					pret.reset(new ConstantValue(temp));
					break;
				}
				case DOUBLE:
				{
					double temp = pArg1->get_double();
					temp += pArg2->get_double();
					pret.reset(new ConstantValue(temp));
					break;
				}
				case STRING:
				{
					std::string temp = pArg1->get_string();
					temp += pArg2->get_string();
					pret.reset(new ConstantValue(temp));
					break;
				}
				default: 
					break;
			}
			break;
						
		case MINUS:
			switch(_type)
			{
				case INT:
				{
					int temp = pArg1->get_int();
					temp -= pArg2->get_int();
					pret.reset(new ConstantValue(temp));
					break;
				}
				case DOUBLE:
				{
					double temp = pArg2->get_double();
					temp -= pArg2->get_double();
					pret.reset(new ConstantValue(temp));
					break;
				}				
				default:
					break;
			}
			break;

		case MULTIPLY:
			switch(_type)
			{
				case INT:
				{
					int temp = pArg1->get_int();
					temp *= pArg2->get_int();
					pret.reset(new ConstantValue(temp));
					TRACE_VERBOSE("MULTIPLY. pret->get_int(): " << pret->get_int())
					break;
				}
				case DOUBLE:
				{
					int temp = pArg1->get_double();
					temp *= pArg2->get_double();
					pret.reset(new ConstantValue(temp));
					break;
				}
				default:
					break;
			}
			break;

		case DIVIDE:
			switch(_type)
			{
				case INT:
				{
					int temp = pArg1->get_int();
					temp /= pArg2->get_int();
					pret.reset(new ConstantValue(temp));
					break;
				}
				case DOUBLE:
				{
					double temp = pArg1->get_double();
					temp /= pArg2->get_double();
					pret.reset(new ConstantValue(temp));
				}
				default:
					break;
			}
			break;	

		case MOD:
			switch(_type)
			{
				case INT:
				{
					int temp = pArg1->get_int();
					temp %= pArg2->get_int();
					pret.reset(new ConstantValue(temp));
				}
				default:
					break;
			}
			break;

		default:
			throw std::logic_error("Operator not Implemented");
	}

	if(!pret)
	{
		throw new std::logic_error("Operator not Implemented for the requested return type");
	}

	return pret;
}


