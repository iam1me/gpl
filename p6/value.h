#ifndef VALUE_H
#define VALUE_H

#include <memory>
#include "gpl_type.h"

//#include "game_object.h"
//#include "animation_block.h"

class Game_object;
class Animation_block;

enum ConversionStatus
{
	CONVERSION_NONE, // ex. INT => INT
	CONVERSION_UPCAST_DOUBLE,
	CONVERSION_UPCAST_STRING,
	CONVERSION_ERROR // ex. GAME_OBJECT => ANIMATION_BLOCK
};

class IValue
{
public:
	virtual ~IValue();

	virtual Gpl_type get_type() const;
	virtual bool is_constant() const;

	virtual ConversionStatus get_int(int&) const = 0;
	virtual ConversionStatus get_double(double&) const = 0;
	virtual ConversionStatus get_string(std::string&) const = 0;
	virtual ConversionStatus get_game_object(std::shared_ptr<Game_object>&) const = 0;
	virtual ConversionStatus get_animation_block(std::shared_ptr<Animation_block>&) const = 0;

	virtual ConversionStatus set_int(const int&) = 0;
	virtual ConversionStatus set_double(const double&) = 0;
	virtual ConversionStatus set_string(const std::string&) = 0;
	virtual ConversionStatus set_game_object(const std::shared_ptr<Game_object>&) = 0;
	virtual ConversionStatus set_animation_block(const std::shared_ptr<Animation_block>&) = 0;	

	virtual std::string to_string() const;
	virtual ConversionStatus get_conversion_status(Gpl_type dest_type, Gpl_type src_type) const;

	// Convenient wrappers for the get methods that return ConversionStatus
	// these ones will throw exceptions on CONVERSION_ERROR
	/*virtual const int& get_int() const;
	virtual const double& get_double() const;
	virtual const std::string& get_string() const;
	virtual const std::shared_ptr<Game_object>& get_game_object() const;
	virtual const std::shared_ptr<Animation_block>& get_animation_block() const;

	// Wrappers for the set methods. These will throw an exception on CONVERSION_ERROR
	virtual void set_int(const int& val);
	virtual	void set_double(const double& val);
	virtual void set_string(const std::string& val);
	virtual void set_game_object(const std::shared_ptr<Game_object>& val);
	virtual void set_animation_block(const std::shared_ptr<Animation_block>& val);
	*/
	
protected:
	IValue(Gpl_type type, bool bConstant);

private:
	Gpl_type _type;
	bool _is_const;
};

#endif
