#include <sstream>
#include "value.h"
#include "parser.h"
#include "game_object.h"
#include "animation_block.h"

IValue::IValue(Gpl_type type, bool bConstant)
{
	TRACE_VERBOSE("IValue::IValue(" << gpl_type_to_string(type) << ", "
			<< (bConstant? "true" : "false") << ")")
	_type = type;
	_is_const = bConstant;
}

IValue::~IValue()
{

}

Gpl_type IValue::get_type() const
{
	return _type;
}

bool IValue::is_constant() const
{
	return _is_const;
}

ConversionStatus IValue::get_conversion_status(Gpl_type src_type, Gpl_type dest_type) const
{
	if(dest_type == src_type) return CONVERSION_NONE;

	switch(src_type)
	{
		case INT:
			if(dest_type & DOUBLE)
			{
				return CONVERSION_UPCAST_DOUBLE;
			}
			else if(dest_type & STRING)
			{
				return CONVERSION_UPCAST_STRING;
			}
			else
			{
				TRACE_VERBOSE("Unsupported Conversion " << gpl_type_to_string(src_type)
					<< " to " << gpl_type_to_string(dest_type))
			}
			break;
		case DOUBLE:
			if(dest_type & (STRING))
				return CONVERSION_UPCAST_STRING;
			break;

		default:
			break;
	}

	return CONVERSION_ERROR;
}
	
std::string IValue::to_string() const
{
	TRACE_VERBOSE("IValue::to_string() - Type: " << gpl_type_to_string(get_type()))

	std::string ret;
	switch(_type)
	{
		case INT:
		{
			int val;
			get_int(val);
			ret = std::to_string(val);
			break;
		}

		case DOUBLE:
		{
			double val;
			get_double(val);

			char buff[256];
			sprintf(buff, "%g", val);

			ret = buff;
			break;
		}
		case STRING:
		{
			ConversionStatus status = get_string(ret);
			if(status == CONVERSION_ERROR)
			{
				TRACE_ERROR("IValue::to_string() - Failed to convert from a string to a string... ?")
				throw std::logic_error("String => string no work... ?!");
			}
			break;
		}

		case GAME_OBJECT:
		{
			std::shared_ptr<Game_object> pObj;
			get_game_object(pObj);

			std::stringstream temp;
			pObj->print(temp);

			ret = temp.str();
			break;
		}

		case ANIMATION_BLOCK:
		{
			std::shared_ptr<Animation_block> pAnim;
			get_animation_block(pAnim);

			std::stringstream temp;
			pAnim->print(temp);
			ret = temp.str();
			break;
		}

		default:
			std::string err = "GPLVariant::to_string - Unsupported Type (";
			err += std::to_string((int)_type);
			err += ")";
			throw std::runtime_error(err);
	}

	return ret;
}

/*
const int& IValue::get_int() const
{
	int ret;
	ConversionStatus status = get_int(ret);
	if(status == CONVERSION_ERROR)
	{
		throw std::logic_error("IValue::get_int() - Conversion Error. Cannot convert from type " 
				+ gpl_type_to_string(_type));
	}
	return ret;
}

const double& IValue::get_double() const
{
	double ret;
	ConversionStatus status = get_double(ret);
	if(status == CONVERSION_ERROR)
	{
		throw std::logic_error("IValue::get_double() - Conversion Error. Cannot convert from type " 
				+ gpl_type_to_string(_type));
	}
	return ret;
}

const std::string& IValue::get_string() const
{
	std::string ret;
	ConversionStatus status = get_string(ret);
	if(status == CONVERSION_ERROR)
	{
		throw std::logic_error("IValue::get_string() - Conversion Error. Cannot convert from type " 
				+ gpl_type_to_string(_type));
	}
	return ret;
}

const std::shared_ptr<Game_object>& IValue::get_game_object() const
{
	std::shared_ptr<Game_object> ret;
	ConversionStatus status = get_game_object(ret);
	if(status == CONVERSION_ERROR)
	{
		throw std::logic_error("IValue::get_game_object() - Conversion Error. Cannot convert from type " 
				+ gpl_type_to_string(_type));
	}
	return ret;
}

const std::shared_ptr<Animation_block>& IValue::get_animation_block() const
{
	std::shared_ptr<Animation_block> ret;
	ConversionStatus status = get_animation_block(ret);
	if(status == CONVERSION_ERROR)
	{
		throw std::logic_error("IValue::get_animation_block() - Conversion Error. Cannot convert from type " 
				+ gpl_type_to_string(_type));
	}
	return ret;
}


// Wrappers for the set methods. These will throw an exception on CONVERSION_ERROR
void IValue::set_int(const int& val)
{
	ConversionStatus status = set_int(val);
	if(status == CONVERSION_ERROR)
	{
		throw std::logic_error("IValue::set_int() - Conversion Error. Cannot convert to value type "
				+ gpl_type_to_string(_type));
	}
}

void IValue::set_double(const double& val)
{
	ConversionStatus status = set_double(val);
	if(status == CONVERSION_ERROR)
	{
		throw std::logic_error("IValue::set_double() - Conversion Error. Cannot convert to value type "
				+ gpl_type_to_string(_type));
	}
}

void IValue::set_string(const std::string& val)
{
	ConversionStatus status = set_string(val);
	if(status == CONVERSION_ERROR)
	{
		throw std::logic_error("IValue::set_string() - Conversion Error. Cannot convert to value type "
				+ gpl_type_to_string(_type));
	}
}

void IValue::set_game_object(const std::shared_ptr<Game_object>& val)
{
	ConversionStatus status = set_game_object(val);
	if(status == CONVERSION_ERROR)
	{
		throw std::logic_error("IValue::set_game_object() - Conversion Error. Cannot convert to value type "
				+ gpl_type_to_string(_type));
	}
}

void IValue::set_animation_block(const std::shared_ptr<Animation_block>& val)
{
	ConversionStatus status = set_animation_block(val);
	if(status == CONVERSION_ERROR)
	{
		throw std::logic_error("IValue::set_animation_block() - Conversion Error. Cannot convert to value type "
				+ gpl_type_to_string(_type));
	}
}
*/
	
