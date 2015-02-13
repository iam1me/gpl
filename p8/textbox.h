#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "game_object.h"
#include <string>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif


class Textbox : public Game_object
{
  public:
	static std::shared_ptr<Game_object> Create();
	virtual ~Textbox() {};

    	virtual Game_object_type get_object_type() const
	{ return TEXTBOX; }

  private:
	Textbox();
    virtual void build_display_list();

    std::string m_text;
    double m_size;
    int m_space;
    GLUquadricObj *m_quadric;

    // disable default copy constructor and default assignment
    // done as a precaution, they should never be called
    Textbox(const Textbox &);
    const Textbox &operator=(const Textbox &);
};

#endif // #ifndef TEXTBOX_H
