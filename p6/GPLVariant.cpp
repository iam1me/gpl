#include <typeinfo>
#include <iostream>
#include <sstream>
#include <string>
#include "GPLVariant.h"
#include "parser.h"

GPLVariant::GPLVariant(int val, bool bConstant)
	: IValue(INT, bConstant)
{
	_val_int = val;
	_binit = true;
}

GPLVariant::GPLVariant(double val, bool bConstant)
	: IValue(DOUBLE, bConstant)
{
	_val_double = val;
	_binit = true;
}

GPLVariant::GPLVariant(const std::string& val, bool bConstant)
	: IValue(STRING, bConstant)
{
	TRACE_VERBOSE("GPLVariant::GPLVariant('" << val << "', " << (bConstant? "true" : "false") << ")")
	_val_pstr = new std::string(val);
	_binit = true;
}

GPLVariant::GPLVariant(const std::shared_ptr<Game_object>& val, bool bConstant)
	: IValue(GAME_OBJECT, bConstant)
{
	_val_pobj = new std::shared_ptr<Game_object>(val);
	_binit = true;
}

GPLVariant::GPLVariant(const std::shared_ptr<Animation_block>& val, bool bConstant)
	: IValue(ANIMATION_BLOCK, bConstant)
{
	_val_panim = new std::shared_ptr<Animation_block>(val);
	_binit = true;
}

GPLVariant::GPLVariant(Gpl_type type, const std::shared_ptr<IValue>& pval, bool bConstant)
	: IValue(type, bConstant)
{
	TRACE_VERBOSE("GPLVariant::GPLVariant(" << gpl_type_to_string(type) << ", "
				<< pval->to_string() << ")")
	ConversionStatus status;
	switch(type)
	{
		case INT:
		{
			int num;
			status = pval->get_int(num);
			if(status == CONVERSION_ERROR) break;

			status = set_int(num);
			break;
		}

		case DOUBLE:
		{
			double val;
			status = pval->get_double(val);
			if(status == CONVERSION_ERROR) break;

			status = set_double(val);
			break;
		}

		case STRING:
		{
			std::string str;
			status = pval->get_string(str);
			if(status == CONVERSION_ERROR) break;
		
			TRACE_VERBOSE("GPLVariant::GPLVariant - calling set_string('" << str <<"')")
			status = set_string(str);
			break;
		}

		case GAME_OBJECT:
		{
			std::shared_ptr<Game_object> pobj;
			status = pval->get_game_object(pobj);
			if(status == CONVERSION_ERROR) break;

			status = set_game_object(pobj);
			break;
		}

		case ANIMATION_BLOCK:
		{
			std::shared_ptr<Animation_block> panim;
			status = pval->get_animation_block(panim);
			if(status == CONVERSION_ERROR) break;

			status = set_animation_block(panim);
			break;
		}

		default:
			status = CONVERSION_ERROR;
			break;
	}

	if(status == CONVERSION_ERROR)
	{
		std::string err = "GPLVariant::GPLVariant - CONVERSION_ERROR. Failed to convert a value of type "
					+ gpl_type_to_string(pval->get_type()) + " to a value of type "
					+ gpl_type_to_string(type);

		TRACE_ERROR(err);
		throw std::logic_error(err);
	}
	_binit = true;
	TRACE_VERBOSE("GPLVariant::GPLVariant - Complete")
}

GPLVariant::GPLVariant(Gpl_type type, bool bConstant)
	: IValue(type, bConstant)
{
	TRACE_VERBOSE("GPLVariant::GPLVariant(" << gpl_type_to_string(type) << ", "
				<< (bConstant? "true" : "false") << ") Defaulting Value...")
	_binit = false;
	switch(type)
	{
		case INT:
			set_int(0);
			break;
		case DOUBLE:
			set_double(0.0);
			break;
		case STRING:
			set_string("");
			break;
		default:
			TRACE_VERBOSE("No Default Value for type " << gpl_type_to_string(type))
			break;
	}
}


GPLVariant::~GPLVariant()
{
	if(!_binit) return;

	Gpl_type type = get_type();
	if(type & STRING)
	{
		delete _val_pstr;
	}
	else if(type & GAME_OBJECT)
	{
		(*_val_pobj).reset();
	}
	else if(type & ANIMATION_BLOCK)
	{
		(*_val_panim).reset();
	}
}


ConversionStatus GPLVariant::get_int(int& ret_val) const
{
	if(!_binit)
	{ 
		TRACE_ERROR("GPLVariant::get_string - Not Initialized")
		throw std::logic_error("GPLVariant is not initialized");
	}

	ConversionStatus result = get_conversion_status(get_type(), INT);
	if(result == CONVERSION_ERROR)
	{
		return result;
	}
	ret_val = _val_int;
	return result;
}

ConversionStatus GPLVariant::get_double(double& ret_val) const
{
	if(!_binit)
	{ 
		TRACE_ERROR("GPLVariant::get_double - Not Initialized")
		throw std::logic_error("GPLVariant is not initialized");
	}

	TRACE_VERBOSE("GPLVariant::get_double - Checking conversion status. Current Type: " << gpl_type_to_string(get_type()));
	ConversionStatus result = get_conversion_status(get_type(), DOUBLE);
	if(result == CONVERSION_ERROR)
	{
		TRACE_VERBOSE("GPLVariant::get_double - Conversion Error");
		return result;
	}

	if(result == CONVERSION_UPCAST_DOUBLE)
	{
		TRACE_VERBOSE("CONVERSION_UPCAST_DOUBLE");
		ret_val = (double)_val_int;
	}
	else
	{
		ret_val = _val_double;
	}

	return result;
}

ConversionStatus GPLVariant::get_string(std::string& ret_val) const
{
	if(!_binit)
	{
		TRACE_ERROR("GPLVariant::get_string - Not Initialized")
		throw std::logic_error("GPLVariant is not initialized");
	}

	ConversionStatus result = get_conversion_status(get_type(), STRING);
	if(result == CONVERSION_ERROR)
	{
		return result;
	}

	if(result == CONVERSION_UPCAST_STRING)
		ret_val = to_string();
	else
		ret_val = *_val_pstr;

	return result;
}

ConversionStatus GPLVariant::get_game_object(std::shared_ptr<Game_object>& ret_val) const
{
	if(!_binit)
	{ 
		TRACE_ERROR("GPLVariant::get_string - Not Initialized")
		throw std::logic_error("GPLVariant is not initialized");
	}

	if(get_type() != GAME_OBJECT) return CONVERSION_ERROR;

	ret_val = *_val_pobj;
	return CONVERSION_NONE;
}

ConversionStatus GPLVariant::get_animation_block(std::shared_ptr<Animation_block>& ret_val) const
{
	if(!_binit)
	{ 
		TRACE_ERROR("GPLVariant::get_string - Not Initialized")
		throw std::logic_error("GPLVariant is not initialized");
	}

	if(get_type() != ANIMATION_BLOCK) return CONVERSION_ERROR;

	ret_val =  *_val_panim;
	return CONVERSION_NONE;
}

ConversionStatus GPLVariant::set_int(const int& val)
{
	ConversionStatus result = get_conversion_status(get_type(), INT);
	if(result == CONVERSION_ERROR) return result;

	switch(result)
	{
		case CONVERSION_NONE:
			_val_int = val;
			break;

		case CONVERSION_UPCAST_DOUBLE:
			_val_double = (double)val;
			break;

		case CONVERSION_UPCAST_STRING:
			*_val_pstr = std::to_string(val);
			break;

		default:
			throw std::logic_error("GPLVariant::set_int() - Conversion Not Implemented");
	}
	_binit = true;
	return result;
}

ConversionStatus GPLVariant::set_double(const double& val)
{
	ConversionStatus result = get_conversion_status(get_type(), DOUBLE);
	if(result == CONVERSION_ERROR) return result;

	if(result == CONVERSION_UPCAST_STRING)
	{
		char buff[256];
		sprintf(buff, "%g", val);
		*_val_pstr = std::string(buff);
	}
	else
	{
		_val_double = val;
	}

	_binit = true;
	return result;
}

ConversionStatus GPLVariant::set_string(const std::string& val)
{
	if(get_type() != STRING) return CONVERSION_ERROR;

	if(!_binit)
	{
		_val_pstr = new std::string(val);
		_binit = true;
	}
	else
	{
		*_val_pstr = val;
	}
	return CONVERSION_NONE;
}

ConversionStatus GPLVariant::set_game_object(const std::shared_ptr<Game_object>& val)
{
	if(get_type() != GAME_OBJECT) return CONVERSION_ERROR;

	if(!_binit)
	{
		_val_pobj = new std::shared_ptr<Game_object>(val);
		_binit = true;
	}
	else
	{
		*_val_pobj = val;
	}

	return CONVERSION_NONE;
}

ConversionStatus GPLVariant::set_animation_block(const std::shared_ptr<Animation_block>& val)
{	
	if(get_type() != ANIMATION_BLOCK) return CONVERSION_ERROR;

	if(!_binit)
	{
		_val_panim = new std::shared_ptr<Animation_block>(val);
		_binit = true;
	}else{
		*_val_panim = val;
	}
	return CONVERSION_NONE;
}

bool GPLVariant::is_initialized() const
{
	return _binit;
}
