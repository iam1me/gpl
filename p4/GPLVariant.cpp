#include "GPLVariant.h"
#include <typeinfo>

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

GPLVariant::~GPLVariant()
{
	if(_type & STRING)
	{
		delete _val_pstr;
	}
}

Gpl_type GPLVariant::get_type() const
{
	return _type;
}

void GPLVariant::set_type(const Gpl_type& type)
{
	if(type == _type) return;

	if(_type == STRING)
	{
		delete _val_pstr;
	}

	_type = type;
	switch(_type)
	{
		case INT:
			_val_int = 42;
			break;

		case DOUBLE:
			_val_double = 3.14159;
			break;
		
		case STRING:
			_val_pstr = new std::string("Hello World");
			break;
		
		default:
			throw std::runtime_error("GPLVariant::set_type - Unsupported Type");
	}
}

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
			sprintf(buff, "%.5f", _val_double);
			ret += buff;
			break;
		}
		case STRING:
			ret = *_val_pstr;	
			break;

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
