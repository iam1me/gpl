#ifndef PARAMETER_LIST_H
#define PARAMETER_LIST_H

#include <string>
#include <vector>
#include <memory>
#include "expression.h"

class Parameter
{
public:
	Parameter(std::string param_name, std::shared_ptr<IExpression>& init_expr);
	virtual ~Parameter();

	const std::string& get_name() const;
	const std::shared_ptr<IExpression>& get_expr() const;
private:
	std::string _name;
	std::shared_ptr<IExpression> _expr;
};
typedef std::vector<std::shared_ptr<Parameter>> ParameterList;

class AnimationParameter : public Parameter
{
public:
	static AnimationParameter* Create(Game_object_type type, std::string object_name);
	virtual ~AnimationParameter();

	const std::string& get_game_object_name() const;
	const Game_object_type& get_game_object_type() const;

protected:
	AnimationParameter(Game_object_type type, std::string object_name, 
				std::shared_ptr<IExpression>& param_expr);

private:
	std::string _object_name;
	Game_object_type _object_type;
};

#endif
