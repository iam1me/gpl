#include <stdexcept>
#include <iostream>
#include <memory>

#include "gpl_statement.h"
#include "gpl_exception.h"

gpl_statement::gpl_statement(int line_no)
{
	_line = line_no;
}

const int& gpl_statement::get_line() const
{
	return _line;
}

//============================================================

statement_block::statement_block(int line)
	: gpl_statement(line)
{
}

void statement_block::execute()
{
	for(StatementList::iterator it = _list.begin(); it != _list.end(); it++)
	{
		(*it)->execute();
	}
}

int statement_block::get_count() const
{
	return _list.size();
}

const std::shared_ptr<gpl_statement>& statement_block::get_statement(int i) const
{
	if(i < 0 || i > (int)_list.size())
	{
		throw std::invalid_argument("index out of range");
	}

	return _list[i];
}

int statement_block::insert_statement(const std::shared_ptr<gpl_statement>& statement)
{
	int ndx = _list.size();
	_list.push_back(statement);
	return ndx;
}

//======================================================================

if_statement::if_statement(int line, std::shared_ptr<IExpression> cond_expr,
	std::shared_ptr<gpl_statement> then_statement, std::shared_ptr<gpl_statement> else_statement)
	: gpl_statement(line)
{
	if(!cond_expr) throw std::invalid_argument("Condition Expression is NULL");
	if(!then_statement) throw std::invalid_argument("Then Statement is NULL");	

	if(cond_expr->get_type() != INT)
	{
		throw invalid_if_expression();
	}

	_pCondition = cond_expr;
	_pThen = then_statement;
	_pElse = else_statement;
}

void if_statement::execute()
{
	std::shared_ptr<IValue> pval = _pCondition->eval();
	
	int result;
	if(pval->get_int(result) == CONVERSION_ERROR)
	{
		throw std::runtime_error("if_statement::execute - CONVERSION_ERROR");
	}

	if(result)
	{
		_pThen->execute();
	}
	else if(_pElse)
	{
		_pElse->execute();
	}
}

const std::shared_ptr<gpl_statement>& if_statement::get_then() const
{
	return _pThen;
}

const std::shared_ptr<gpl_statement>& if_statement::get_else() const
{
	return _pElse;
}

//===============================================================


print_statement::print_statement(int line, std::shared_ptr<IExpression> prnt_expr)
	: gpl_statement(line)
{
	if(!prnt_expr) throw std::invalid_argument("Print Expression is NULL");

	if(!(prnt_expr->get_type() & (INT|DOUBLE|STRING)))
	{
		throw invalid_print_expression();
	}

	_prnt_expr = prnt_expr;
}

void print_statement::execute()
{
	std::shared_ptr<IValue> pval = _prnt_expr->eval();

	std::string print_string = pval->to_string();

	std::cout << print_string;	
}

//===================================================================

exit_statement::exit_statement(int line, std::shared_ptr<IExpression> exit_expr)
	: gpl_statement(line)
{
	if(!exit_expr) throw std::invalid_argument("Exit Expression is NULL");

	if(exit_expr->get_type() != INT)
	{
		throw invalid_exit_expression();
	}	

	_exit_expr = exit_expr;
}

void exit_statement::execute()
{
	std::shared_ptr<IValue> pval = _exit_expr->eval();
	
	int result;
	if(pval->get_int(result) == CONVERSION_ERROR)
	{
		throw std::runtime_error("exit_statement::execute - CONVERSION_ERROR");
	}

	exit(result);
}

//===================================================================

assign_statement::assign_statement(int line, std::shared_ptr<IVariableExpression> pLHS, 
				Assignment_type assign_oper, std::shared_ptr<IExpression> pRHS)
	: gpl_statement(line)
{
	if(!pLHS) throw std::invalid_argument("Left-Hand Expression is NULL");
	if(!pRHS) throw std::invalid_argument("Right-Hand Expression is NULL");

	Gpl_type lhs_type = pLHS->get_type();
	Gpl_type rhs_type = pRHS->get_type();

	// if LHS is STRING, SUBTRACT_ASSIGN not supported
	if((lhs_type & STRING) & (assign_oper & SUBTRACT_ASSIGN))
		throw invalid_assign_lhs(assign_oper, pLHS->get_name(), lhs_type);

	// if LHS is ANIMATION_BLOCK, only ASSIGN is supported
	if((lhs_type & ANIMATION_BLOCK) & !(assign_oper & ASSIGN))
		throw invalid_assign_lhs(assign_oper, pLHS->get_name(), lhs_type);


	// Can't assign a Game Object to another one
	if(lhs_type & GAME_OBJECT) throw invalid_assign_rhs(assign_oper, lhs_type, rhs_type);
	if(rhs_type & GAME_OBJECT) throw invalid_assign_rhs(assign_oper, lhs_type, rhs_type);

	// If LHS is an INT, RHS must be an INT
	if((lhs_type & INT) & !(rhs_type & INT))
		throw invalid_assign_rhs(assign_oper, lhs_type, rhs_type);

	// if LHS is a DOUBLE< RHS must be an INT|DOUBLE
	if((lhs_type & DOUBLE) & !(rhs_type & (INT|DOUBLE)))
		throw invalid_assign_rhs(assign_oper, lhs_type, rhs_type);

	// if LHS is a STRING, RHS must be a INT|DOUBLE|STRING
	if((lhs_type & STRING) & !(rhs_type & (INT|DOUBLE|STRING)))
		throw invalid_assign_rhs(assign_oper, lhs_type, rhs_type);

	// if LHS is an ANIMATION_BLOCK, RHS must be an ANIMATION_BLOCK
	if((lhs_type & ANIMATION_BLOCK) & !(rhs_type & ANIMATION_BLOCK))
		throw invalid_assign_rhs(assign_oper, lhs_type, rhs_type);

	_pLHS = pLHS;
	_pRHS = pRHS;
	_operator = assign_oper;
	_assign_type = lhs_type;
}

void assign_statement::execute()
{
	// Evaluate the LHS & RHS, as necessary
	std::shared_ptr<IValue> pLHS_Val;
	std::shared_ptr<IValue> pRHS_Val = _pRHS->eval();

	if(_operator != ASSIGN)
		pLHS_Val = _pLHS->eval();

	// Set the LHS to the above value
	switch(_assign_type)
	{
		case INT:
		{
			int lhs_num, rhs_num;
			if(pRHS_Val->get_int(rhs_num) == CONVERSION_ERROR)
				throw std::runtime_error("assign_statement::execute -"
					" Failed to retrieve the INT value of the RHS Expression");

			if(_operator == ASSIGN)
			{
				lhs_num = rhs_num;
			}
			else
			{
				if(pLHS_Val->get_int(lhs_num) == CONVERSION_ERROR) 
					throw std::runtime_error("assign_statement::execute -" 
						" Failed to retrieve the INT value of the LHS Expression");

				if(_operator == ADD_ASSIGN)
					lhs_num += rhs_num;
				else
					lhs_num -= rhs_num;
			}
			
			if(pLHS_Val->set_int(lhs_num) == CONVERSION_ERROR)
				throw std::runtime_error("assign_statement::execute -"
					" Failed to set the INT value of the LHS Expression");
		}	
		break;

		case DOUBLE:
		{
			double lhs_dbl, rhs_dbl;
			if(pRHS_Val->get_double(rhs_dbl) == CONVERSION_ERROR)
				throw std::runtime_error("assign_statement::execute - "
					" Failed to get the DOUBLE value of the RHS Expression");

			if(_operator == ASSIGN)
			{
				lhs_dbl = rhs_dbl;
			}
			else
			{
				if(pLHS_Val->get_double(lhs_dbl) == CONVERSION_ERROR)
					throw std::runtime_error("assign_statement::execute - "
						" Failed to get the DOUBLE value of the LHS Expression");

				if(_operator == ADD_ASSIGN) lhs_dbl += rhs_dbl;
				else lhs_dbl -= rhs_dbl;		
			}

			if(pLHS_Val->set_double(lhs_dbl) == CONVERSION_ERROR)
				throw std::runtime_error("assign_statement::execute - "
					" Failed to set the DOUBLE value of the LHS Expression");
		}
		break;

		case STRING:
		{
			std::string lhs_str, rhs_str;
			if(pRHS_Val->get_string(rhs_str) == CONVERSION_ERROR)
				throw std::runtime_error("assign_statement::execute -"
					" Failed to get the STRING value of the RHS Expression");

			if(_operator == ASSIGN)
			{
				lhs_str = rhs_str;
			}
			else if(_operator == ADD_ASSIGN)
			{
				if(pLHS_Val->get_string(lhs_str) == CONVERSION_ERROR)
					throw std::runtime_error("assign_statement::execute -"
						" Failed to get the STRING value of the LHS Expresssion");

				lhs_str += rhs_str;
			}
			else throw undefined_error();

			if(pLHS_Val->set_string(lhs_str) == CONVERSION_ERROR)
				throw std::runtime_error("assign_statement::execute -"
					" Failed to set the STRING value of the LHS Expression");
		}
		break;

		case ANIMATION_BLOCK:
		{
			std::shared_ptr<Animation_block> rhs_anim;
			if(pRHS_Val->get_animation_block(rhs_anim) == CONVERSION_ERROR)
				throw std::runtime_error("assign_statement::execute -"
					" Failed to get the ANIMATION_BLOCK value of the RHS Expression");

			if(_operator != ASSIGN) throw undefined_error();

			if(pLHS_Val->set_animation_block(rhs_anim) == CONVERSION_ERROR)
				throw std::runtime_error("assign_statement::execute -"
					" Failed to set the ANIMATiON_BLOCK value of the LHS Expression");
		}
		break;

		default:
			throw undefined_error();
	}
}

//===================================================================

for_statement::for_statement(int line, const std::shared_ptr<assign_statement>& pInit,
		const std::shared_ptr<IExpression>& pCondition,
		const std::shared_ptr<assign_statement>& pIncrement,
		const std::shared_ptr<statement_block>& pBody)
	: gpl_statement(line)
{
	if(!pInit) throw std::invalid_argument("Initialization Statement is NULL");
	if(!pCondition) throw std::invalid_argument("Condition Statement is NULL");
	if(!pIncrement) throw std::invalid_argument("Increment Statement is NULL");
	if(!pBody) throw std::invalid_argument("Body Statement Block is NULL");

	if(pCondition->get_type() != INT) throw invalid_for_expression();

	_pInit = pInit;
	_pCondition = pCondition;
	_pIncrement = pIncrement;
	_pBody = pBody;
}

void for_statement::execute()
{
	// Run the Init Statement
	_pInit->execute();

	// Loop
	int bcond = 1;
	std::shared_ptr<IValue> pval;
	while(true)
	{
		// Evaluate the Condition
		pval = _pCondition->eval();
		if(pval->get_int(bcond) == CONVERSION_ERROR)
		{
			throw undefined_error();
		}
		
		if(!bcond) break;

		// Execute the Body & Increment
		_pBody->execute();
		_pIncrement->execute();		
	}
}

