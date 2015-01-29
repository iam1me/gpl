#include "game_macros.cpp"
#include "room_macros.cpp"

//#define GET_TILE_INFO(tile_id, tilename, frame_count, bcan_walk)

animation TileAnimation(pixmap tile_obj)
{
	#define COL 		tile_obj.PROP_OBJECT_COL
	#define ROW 		tile_obj.PROP_OBJECT_ROW
	#define OBJECT_ID       tile_obj.PROP_OBJECT_ID
	#define ID 	 	tile_id[TILE_OFFSET(COL,ROW)]
	#define TILE_FRAMES	game_obj.user_int4
	#define CAN_WALK	game_obj.user_int5
	#define TILENAME 	game_obj.user_string5

	//TRACE_VERBOSE("TileAnimation - Tile Object ID: " + tile_obj.PROP_OBJECT_ID + " COL: " + COL + " ROW: " + ROW)

	if(tile_obj.PROP_OBJECT_STATE == STATE_INIT)
	{
		//TRACE_VERBOSE("INIT (" + COL + ", " + ROW + ") OBJECT_ID: " + OBJECT_ID + " TILE_ID: " + ID)
		tile_obj.x = COL * map_tile_width;
		tile_obj.y = ROW * map_tile_height;
		GET_TILE_INFO(ID, TILENAME, TILE_FRAMES, CAN_WALK)
		
		SET_WALKABLE(COL, ROW, CAN_WALK) 

		if(TILE_FRAMES == 1)
		{
			TILE_OBJ(COL, ROW).filename = TILENAME + ".bmp";
		}		
		else
		{
			TILE_OBJ(COL, ROW).filename = TILENAME + "_0.bmp";
		}

		//TRACE_VERBOSE("Setting (" + COL + ", " + ROW + "): " + TILE_OBJ(COL,ROW).filename)

		tile_obj.PROP_OBJECT_STATE = STATE_DEFAULT;
	}		

	#undef COL
	#undef ROW
	#undef ID
	#undef TILE_FRAMES
	#undef CAN_WALK
	#undef TILENAME
}
