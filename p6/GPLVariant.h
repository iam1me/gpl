#ifndef GPLVARIANT_H
#define GPLVARIANT_H

#include <stdexcept>
#include <typeinfo>
#include <memory>
#include "gpl_type.h"

class Game_object;
class Animation_block;

// GPLVariant is a union-like class that encapsulates the logic
// for safely setting, accessing, and releasing the value for a
// GPL variable. Used by the Symbol class.
class GPLVariant
{
public:
	template<class T>
	GPLVariant(const T& val);

	//template<class T>
	//GPLVariant(const std::shared_ptr<T>& val);

	~GPLVariant();

	Gpl_type get_type() const;
	//void set_type(const Gpl_type& type);

	template<class T>
	const T& get_value() const;

	template<class T>
	void set_value(const T&);

	std::string to_string() const;

private:
	Gpl_type _type;

	union{
		int _val_int;
		double _val_double;
		std::string* _val_pstr;		
		std::shared_ptr<Game_object>* _val_pobj;
		std::shared_ptr<Animation_block>* _val_panim;
	};
};

template<class T>
GPLVariant::GPLVariant(const T& val)
{
	std::string err = "GPLVariant::GPLVariant - Type (";
	err += typeid(T).name();
	err += ") Not Supported";
	
	throw std::runtime_error(err);
}

/*template<class T>
GPLVariant::GPLVariant(const std::shared_ptr<T>& pval)
{
	std::string err = "GPLVariant::GPLVariant - Type(";
	err += typeid(T).name();
	err += ") Not Supported";
	throw std::runtime_error(err);
}*/

template<class T>
const T& GPLVariant::get_value() const
{
	std::string err = "GPLVariant::get_value - Type (";
	err += typeid(T).name();
	err += ") Not Supported";

	throw std::runtime_error(err);
}

template<class T>
void GPLVariant::set_value(const T&)
{
	std::string err = "GPLVariant::set_value - Type (";
	err += typeid(T).name();
	err += ") Not Supported";

	throw std::runtime_error("GPLVariant::set_value - Type Not Recognized");
}

#endif
