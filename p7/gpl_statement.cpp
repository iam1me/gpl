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


