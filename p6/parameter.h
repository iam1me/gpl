#ifndef PARAMETER_LIST_H
#define PARAMETER_LIST_H

#include <string>
#include <vector>
#include <memory>
#include "expression.h"

class Parameter
{
public:
	Parameter(std::string param_name, std::shared_ptr<IExpression> init_expr);
	~Parameter();

	const std::string& get_name() const;
	const std::shared_ptr<IExpression>& get_expr() const;
private:
	std::string _name;
	std::shared_ptr<IExpression> _expr;
};

typedef std::vector<std::shared_ptr<Parameter>> ParameterList;

#endif
