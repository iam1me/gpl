//======================================
//  DECLARATIONS SECTION
//=====================================

//  Include Macros
#include "game_macros.cpp"
#include "map_macros.cpp"
#include "player_macros.cpp"
#include "slenderman_macros.cpp"
#include "room_macros.cpp"

//  Include Definitions
#include "game_vars.h"
#include "map_vars.h"
#include "grid_vars.h"
//#include "creature_vars.h"
#include "player_vars.h"
#include "slenderman_vars.h"
#include "room_vars.h"
#include "clock_vars.h"

//  SYSTEM VARIABLES
int window_width = map_tile_width * map_columns;
int window_height = map_tile_height * map_rows;
string window_title = "The Gypsy Rover";

//  Include Initializations
#include "game_init.cpp" //this must come first
#include "grid_init.cpp"
#include "map_init.cpp"
//#include "creature_init.cpp"
#include "player_init.cpp"
#include "slenderman_init.cpp"
#include "clock_init.cpp"
#include "room_init.cpp"

//  Include Animations
#include "map_anim.cpp"
#include "player_anim.cpp"
#include "slenderman_anim.cpp"
#include "clock_anim.cpp"
#include "game_anim.cpp"
#include "room_anim.cpp"
