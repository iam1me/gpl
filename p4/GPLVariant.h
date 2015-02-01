#ifndef GPLVARIANT_H
#define GPLVARIANT_H

#include <stdexcept>
#include "gpl_type.h"

// GPLVariant is a union-like class that encapsulates the logic
// for safely setting, accessing, and releasing the value for a
// GPL variable. Used by the Symbol class.
class GPLVariant
{
public:
	template<class T>
	GPLVariant(const T& val);
	~GPLVariant();

	Gpl_type get_type() const;
	void set_type(const Gpl_type& type);

	template<class T>
	const T& get_value() const;

	template<class T>
	void set_value(const T&);

private:
	Gpl_type _type;

	union{
		int _val_int;
		double _val_double;
		std::string* _val_pstr;		
	};
};

template<class T>
GPLVariant::GPLVariant(const T& val)
{
	throw std::runtime_error("GPLVariant::GPLVariant - Type not recognized");
}

template<class T>
const T& GPLVariant::get_value() const
{
	throw std::runtime_error("GPLVariant::get_value - Unsupported Template Specialization");
}

template<class T>
void GPLVariant::set_value(const T&)
{
	throw std::runtime_error("GPLVariant::set_value - Type Not Recognized");
}

#endif
