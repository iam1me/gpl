#include <typeinfo>
#include <iostream>
#include <sstream>
#include <string>
#include "GPLVariant.h"
#include "parser.h"

template<>
GPLVariant::GPLVariant<int>(const int& val)
{
	_type = INT;
	_val_int = val;
}

template<>
GPLVariant::GPLVariant<double>(const double& val)
{
	_type = DOUBLE;
	_val_double = val;
}

template<>
GPLVariant::GPLVariant<std::string>(const std::string& val)
{
	_type = STRING;
	_val_pstr = new std::string(val);
}

template<>
GPLVariant::GPLVariant<const char*&>(const char*& val)
{
	_type = STRING;
	_val_pstr = new std::string(val);
}

template<>
GPLVariant::GPLVariant(const std::shared_ptr<Game_object>& pval)
{
	TRACE_VERBOSE("GPLVariant::GPLVariant(const std::shared_ptr<Game_object>&) - BEGIN")
	_type = GAME_OBJECT;
	_val_pobj = new std::shared_ptr<Game_object>(pval);
	TRACE_VERBOSE("GPLVariant::GPLVariant(const std::shared_ptr<Game_object>&) - END")
}

template<>
GPLVariant::GPLVariant(const std::shared_ptr<Animation_block>& pval)
{
	TRACE_VERBOSE("GPLVariant::GPLVariant(const std::shared_ptr<Animation_block>&) - BEGIN")
	_type = ANIMATION_BLOCK;
	_val_panim = new std::shared_ptr<Animation_block>(pval);
	TRACE_VERBOSE("GPLVariant::GPLVariant(const std::shared_ptr<Animation_block>&) - END")
}

GPLVariant::~GPLVariant()
{
	if(_type & STRING)
	{
		delete _val_pstr;
	}
	else if(_type & GAME_OBJECT)
	{
		(*_val_pobj).reset();
	}
	else if(_type & ANIMATION_BLOCK)
	{
		(*_val_panim).reset();
	}
}

Gpl_type GPLVariant::get_type() const
{
	return _type;
}

/*void GPLVariant::set_type(const Gpl_type& type)
{
	if(type == _type) return;

	if(_type & STRING)
	{
		delete _val_pstr;
	}
	else if(_type & GAME_OBJECT)
	{
		_val_pobj.reset();
	}
	else if(_type & ANIMATION_OBJECT)
	{
		_val_panim.reset();
	}

	_type = type;
	switch(_type)
	{
		case INT:
			_val_int = 0;
			break;

		case DOUBLE:
			_val_double = 0;
			break;
		
		case STRING:
			_val_pstr = new std::string("");
			break;

		case GAME_OBJECT:
			_val_pobj = new Game_object();
			break;
		
		default:
			throw std::runtime_error("GPLVariant::set_type - Unsupported Type");
	}
}*/

std::string GPLVariant::to_string() const
{
	std::string ret;
	switch(_type)
	{
		case INT:
			ret = std::to_string(_val_int);
			break;

		case DOUBLE:
		{
			char buff[256];
			sprintf(buff, "%g", _val_double);
			ret += buff;
			break;
		}
		case STRING:
			ret = *_val_pstr;	
			break;

		case GAME_OBJECT:
		{
			std::stringstream temp;
			(*_val_pobj)->print(temp);
			return temp.str();
		}

		case ANIMATION_BLOCK:
		{
			std::stringstream temp;
			(*_val_panim)->print(temp);
			return temp.str();
		}

		default:
			std::string err = "GPLVariant::to_string - Unsupported Type (";
			err += std::to_string((int)_type);
			err += ")";
			throw std::runtime_error(err);
	}

	return ret;
}

template<>
void GPLVariant::set_value<int>(const int& val)
{
	if(_type != INT)
	{
		throw std::logic_error("GPLVariant::set_value<int> - the variant is not set to type INT");
	}
	_val_int = val;
}

template<>
void GPLVariant::set_value<double>(const double& val)
{
	if(_type != DOUBLE)
	{
		throw std::logic_error("GPLVariant::set_value<double> - the variant is not set to type DOUBLE");
	}

	_val_double = val;
}


template<>
void GPLVariant::set_value<std::string>(const std::string& val)
{
	if(_type != STRING)
	{
		throw std::logic_error("GPLVariant::set_value<std::string> - the variant is not set to type STRING");
	}

	*_val_pstr = val;
}

template<>
void GPLVariant::set_value<const char*&>(const char*& val)
{
	if(_type != STRING)
	{
		throw std::logic_error("GPLVariant::set_value<std::string> - the variant is not set to type STRIGN");
	}
	*_val_pstr = val;
}

template<>
void GPLVariant::set_value<const std::shared_ptr<Game_object>&>(const std::shared_ptr<Game_object>& val)
{
	if(_type != GAME_OBJECT)
	{
		throw std::logic_error("GPLVariant::set_value<std::shared_ptr<Game_object>&> - the variant is not set to type GAME_OBJECT");
	}
	*_val_pobj = val;
}

template<>
void GPLVariant::set_value<const std::shared_ptr<Animation_block>&>(const std::shared_ptr<Animation_block>& val)
{
	if(_type != ANIMATION_BLOCK)
	{
		throw std::logic_error("GPLVariant::set_value<std::shared_ptr<Game_object>&> - the variant is not set to type ANIMATION_BLOCK");
	}
	*_val_panim = val;
}

template<>
const int& GPLVariant::get_value<int>() const
{
	if(_type != INT)
	{
		throw std::logic_error("GPLVariant::get_value<int> - the variant is not of type INT");
	}

	return _val_int;
}

template<>
const double& GPLVariant::get_value<double>() const
{
	if(_type != DOUBLE)
	{
		throw std::logic_error("GPLVariant::get_value<double> - the variant is not of type DOUBLE");
	}

	return _val_double;
}

template<>
const std::string& GPLVariant::get_value<std::string>() const
{
	if(_type != STRING)
	{
		throw std::logic_error("GPLVariant::get_value<std::string> - the variant is not of type STRING");
	}
	
	return *_val_pstr;
}

template<>
const std::shared_ptr<Game_object>& GPLVariant::get_value<std::shared_ptr<Game_object>>() const
{
	if(_type != GAME_OBJECT)
	{
		throw std::logic_error("GPLVariant::get_value<Game_object> - the variant is not of type GAME_OBJECT");
	}
	return *_val_pobj;
}

template<>
const std::shared_ptr<Animation_block>& GPLVariant::get_value<std::shared_ptr<Animation_block>>() const
{
	if(_type != ANIMATION_BLOCK)
	{
		throw std::logic_error("GPLVariant::get_value<Animation_block> - the variant is not of type ANIMATION_BLOCK");
	}
	return *_val_panim;
}
