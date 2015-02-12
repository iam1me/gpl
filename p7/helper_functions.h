#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "parser.h"
#include "gpl_type.h"
#include "gpl_assert.h"
#include "game_object.h"
#include "animation_block.h"
#include "symbol.h"
#include "symbol_table.h"
#include "triangle.h"
#include "rectangle.h"
#include "circle.h"
#include "pixmap.h"
#include "textbox.h"

std::shared_ptr<Game_object> create_game_object(Game_object_type);

#endif
