/** Symbol.h
 ** Defines the symbol class used by the Symbol_table class.
 ** Holds information relevant to a particular symbol in the
 ** source file being parsed.
 **/

#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>
#include <string.h>
#include <stdexcept>
#include <memory>

#include "value.h"
#include "GPLVariant.h"

// Symbol = named GPLVariant
class Symbol : public GPLVariant
{
public:
	Symbol(const std::string& name, const int& val);
	Symbol(const std::string& name, const double& val);
	Symbol(const std::string& name, const std::string& val);
	Symbol(const std::string& name, const std::shared_ptr<Game_object>& val);
	Symbol(const std::string& name, const std::shared_ptr<Animation_block>& val);

	Symbol(const std::string& name, const Gpl_type& type, const std::shared_ptr<IValue>& pval);
	Symbol(const std::string& name, const Gpl_type& type); // uninitialized symbol
	virtual ~Symbol();

	const std::string& get_name() const;

	virtual std::ostream& print(std::ostream& os) const;

private:
	std::string _name;
	bool _bInitialized;
};



// Points to a symbol. This reference may not be changed.
// The get/set calls are all routed back to the symbol
class Reference: public IValue
{
public:
	Reference(std::shared_ptr<Symbol> pSymbol);
	virtual ~Reference();	

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
	
private:
	std::shared_ptr<Symbol> _pSymbol;
};

/*
// Points to the member variable of a specific object. This reference cannot be modified.
// However, by means of the get/set functions one can access and modify the value of the 
// member variable
class MemberReferenceValue : public IValue
{
public:
	MemberReferenceValue(std::shared_ptr<Symbol> symbol, std::string member_name);
	virtual ~MemberReferenceValue();	

	Gpl_type get_type() const;
	bool is_reference() const;
	bool is_constant() const;
	
	int get_int() const;
	double get_double() const;
	std::string get_string() const;
	std::shared_ptr<Game_object> get_game_object() const;
	std::shared_ptr<Animation_block> get_animation_block() const;

	void set_int(int);
	void set_double(double);
	void set_string(const std::string&);
	void set_game_object(const std::shared_ptr<Game_object>&);
	void set_animation_block(const std::shared_ptr<Animation_block>&);

	std::shared_ptr<Symbol> get_reference() const;
	const std::string& get_member_name() const;

protected:
	std::shared_ptr<GPLVariant> get_variant() const;

private:
	std::shared_ptr<Symbol> _pSymbol;
	std::string _member;
};
*/

#endif
