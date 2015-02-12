#ifndef GPL_STATEMENT_H
#define GPL_STATEMENT_H

#include <vector>
#include "gpl_type.h"
#include "value.h"
#include "expression.h"

class gpl_statement
{
public:
	virtual ~gpl_statement() {};
	virtual void execute() = 0;
	virtual const int& get_line() const;
	
protected:
	gpl_statement(int line_no);

private:
	int _line;
};

class statement_block : gpl_statement
{
public:
	statement_block(int line);
	virtual ~statement_block() {};
	virtual void execute();

	int get_count() const;
	const std::shared_ptr<gpl_statement>& get_statement(int i) const;
	int insert_statement(const std::shared_ptr<gpl_statement>& statement);

protected:
	typedef std::vector<std::shared_ptr<gpl_statement>> StatementList;
	
private:
	StatementList _list;
};

class if_statement : public gpl_statement
{
public:
	if_statement(int line, std::shared_ptr<IExpression> cond_expr, 
		std::shared_ptr<gpl_statement> then_statement,
		std::shared_ptr<gpl_statement> else_statement = nullptr);

	virtual ~if_statement() {};
	virtual void execute();	

	const std::shared_ptr<gpl_statement>& get_then() const;
	const std::shared_ptr<gpl_statement>& get_else() const;

private:
	std::shared_ptr<IExpression> _pCondition;
	std::shared_ptr<gpl_statement> _pThen, _pElse;
};

class print_statement : public gpl_statement
{
public:
	print_statement(int line, std::shared_ptr<IExpression> prnt_expr);
	virtual ~print_statement(){};
	virtual void execute();
private:
	std::shared_ptr<IExpression> _prnt_expr;
};

class exit_statement : public gpl_statement
{
public:
	exit_statement(int line, std::shared_ptr<IExpression> exit_expr);
	virtual ~exit_statement() {};
	virtual void execute();
private:
	std::shared_ptr<IExpression> _exit_expr;
};


class assign_statement : public gpl_statement
{
public:
	assign_statement(int line, std::shared_ptr<IExpression> pLHS, Operator_type type,
		std::shared_ptr<IExpression> pRHS);
	virtual ~assign_statement() {};
	virtual void execute();
private:
	std::shared_ptr<IExpression> _pLHS, _pRHS;
	Operator_type _operator;	
};

class for_statement : public gpl_statement
{
public:
	for_statement(int line, std::shared_ptr<IExpression> pCondition,
		std::shared_ptr<statement_block> pinitialization_block,
		std::shared_ptr<statement_block> pincrement_block,
		std::shared_ptr<statement_block> pbody_block);
	virtual ~for_statement() {};
	virtual void execute();
private:
	std::shared_ptr<IExpression> _pCondition;
	std::shared_ptr<statement_block> _pInit, _pIncrement, _pBody;
};


#endif
