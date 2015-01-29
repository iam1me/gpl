#include "game_macros.cpp"
#include "player_macros.cpp"

animation NullAnimation(GAMEOBJECT null_obj)
{
	//Do Nothing
}

animation GameAnimation(rectangle game_obj_param)
{
	//  CLEAR FLAGS
	flag_leftarrow = 0;
	flag_rightarrow = 0;
	flag_uparrow = 0;
	flag_downarrow = 0;
	flag_spacebar = 0;

	
	if((tick_count - last_spawn_tick > spawn_rate)
		|| (last_spawn_tick > tick_count))
	{
		TRACE_VERBOSE("Spawning a new Slenderman")
		CREATE_SLENDERMAN_OBJ(TMP_INT)
		last_spawn_tick = tick_count;
	}
}

animation InitGameAnimation(rectangle game_init_obj)
{
/*
	//Initialize Tiles
	#define COL 		game_init_obj.PROP_OBJECT_COL
	#define ROW 		game_init_obj.PROP_OBJECT_ROW
	#define ID 	 	game_init_obj.user_int
	#define TILE_FRAMES	game_init_obj.user_int2
	#define CAN_WALK	game_init_obj.user_int3
	#define TILENAME 	game_init_obj.user_string

//	TRACE_VERBOSE("TileAnimation - Tile Object ID: " + tile_obj.PROP_OBJECT_ID + " COL: " + COL + " ROW: " + ROW)

	for(COL = 0; COL < map_columns; COL += 1)
	{
		for(ROW = 0; ROW < map_rows; ROW += 1)
		{
			
	tile_obj.x = COL * map_tile_width;
	tile_obj.y = ROW * map_tile_height;
	
	GET_TILE_ID(COL, ROW, ID)
	GET_TILE_INFO(29, TILENAME, TILE_FRAMES, CAN_WALK)
		
	if(TILE_FRAMES == 1)
	{
		TILE_OBJ(COL, ROW).filename = TILENAME + ".bmp";
	}		
	else
	{
		TILE_OBJ(COL, ROW).filename = TILENAME + "_0.bmp";
	}

	tile_obj.PROP_OBJECT_STATE = STATE_DEFAULT;

		}		
	}

	#undef COL
	#undef ROW
	#undef ID
	#undef TILE_FRAMES
	#undef CAN_WALK
	#undef TILENAME
*/
	game_init_obj.animation_block = GameAnimation;
	TRACE_VERBOSE("GameInitAnimation: COMPLETE")
}

