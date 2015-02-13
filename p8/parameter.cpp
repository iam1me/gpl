#include "parameter.h"
#include "helper_functions.h"

Parameter::Parameter(std::string param_name, std::shared_ptr<IExpression>& init_expr)
{
	_name = param_name;
	_expr = init_expr;
}

Parameter::~Parameter()
{}

const std::string& Parameter::get_name() const
{
	return _name;
}

const std::shared_ptr<IExpression>& Parameter::get_expr() const
{
	return _expr;
}

AnimationParameter* AnimationParameter::Create(Game_object_type type, std::string object_name)
{
	std::shared_ptr<IValue> val_obj(new GPLVariant(create_game_object(type)));
	std::shared_ptr<IValue> val_symbol(new Symbol(object_name, GAME_OBJECT, val_obj));
	std::shared_ptr<IExpression> param_expr(new ValueExpression(val_symbol));
	return new AnimationParameter(type, object_name, param_expr);
}

AnimationParameter::AnimationParameter(Game_object_type type, std::string object_name, 
						std::shared_ptr<IExpression>& param_expr)
	: Parameter("animation_block", param_expr)
{
	_object_type = type;
	_object_name = object_name;
}

AnimationParameter::~AnimationParameter()
{}

const Game_object_type& AnimationParameter::get_game_object_type() const
{
	return _object_type;
}

const std::string& AnimationParameter::get_game_object_name() const
{
	return _object_name;
}
