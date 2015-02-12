#include "helper_functions.h"

std::shared_ptr<Game_object> create_game_object(Game_object_type type)
{
	std::shared_ptr<Game_object> pret = nullptr;
	switch(type)
	{
		case TRIANGLE: pret.reset(new Triangle()); break;
		case RECTANGLE: pret.reset(new Rectangle()); break;
		case CIRCLE: pret.reset(new Circle()); break;
		case PIXMAP: pret.reset(new Pixmap()); break;
		case TEXTBOX: pret.reset(new Textbox()); break;
		default: 
			TRACE_ERROR("create_game_object - Unrecognized Type"); 
			assert(false);
			break;
	}
	return pret;
}
