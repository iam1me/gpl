#ifndef GPLVARIANT_H
#define GPLVARIANT_H

#include <stdexcept>
#include <typeinfo>
#include <memory>
#include "gpl_type.h"
#include "value.h"

class Game_object;
class Animation_block;

// GPLVariant is a union-like class that encapsulates the logic
// for safely setting, accessing, and releasing the value for a
// GPL variable. Serves as the base class for ConstantValue and
// VariableValue respectively.
class GPLVariant : public IValue
{
public:
	GPLVariant(int val, bool bConstant = true);
	GPLVariant(double val, bool bConstant = true);
	GPLVariant(const std::string& val, bool bConstant = true);
	GPLVariant(const std::shared_ptr<Game_object>& val, bool bConstant = true);
	GPLVariant(const std::shared_ptr<Animation_block>& val, bool bConstant = true);
	GPLVariant(Gpl_type type, const std::shared_ptr<IValue>& pval, bool bConstant = true);
	GPLVariant(Gpl_type type, bool bConstant = true); //not initialized
	~GPLVariant();

	virtual ConversionStatus get_int(int&) const;
	virtual ConversionStatus get_double(double&) const;
	virtual ConversionStatus get_string(std::string&) const;
	virtual ConversionStatus get_game_object(std::shared_ptr<Game_object>&) const;
	virtual ConversionStatus get_animation_block(std::shared_ptr<Animation_block>&) const;

	virtual ConversionStatus set_int(const int&);
	virtual ConversionStatus set_double(const double&);
	virtual ConversionStatus set_string(const std::string&);
	virtual ConversionStatus set_game_object(const std::shared_ptr<Game_object>&);
	virtual ConversionStatus set_animation_block(const std::shared_ptr<Animation_block>&);	

	bool is_initialized() const;

private:
	bool _binit; 
	Gpl_type _type;

	union{
		int _val_int;
		double _val_double;
		std::string* _val_pstr;		
		std::shared_ptr<Game_object>* _val_pobj;
		std::shared_ptr<Animation_block>* _val_panim;
	};
};

#endif
