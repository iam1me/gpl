#include "helper_functions.h"

std::shared_ptr<Game_object> create_game_object(Game_object_type type)
{
	switch(type)
	{
		case TRIANGLE: return Triangle::Create();
		case RECTANGLE: return Rectangle::Create();
		case CIRCLE: return Circle::Create();
		case PIXMAP: return Pixmap::Create();
		case TEXTBOX: return Textbox::Create(); 
		default: 
			TRACE_ERROR("create_game_object - Unrecognized Type"); 
			assert(false);
			break;
	}
	return nullptr;
}
