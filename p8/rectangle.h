#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "game_object.h"

class Rectangle : public Game_object
{
  public:
	static std::shared_ptr<Game_object> Create();
	virtual ~Rectangle() {};

    	virtual Game_object_type get_object_type() const
	{ return RECTANGLE; }

  private:
	Rectangle();

    // disable default copy constructor and default assignment
    // done as a precaution, they should never be called
    Rectangle(const Rectangle &);
    const Rectangle &operator=(const Rectangle &);

    virtual void build_display_list();

    double m_rotation;

};

#endif // #ifndef RECTANGLE_H
