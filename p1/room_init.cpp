#include "game_macros.cpp"
#include "room_macros.cpp"

initialization
{
	#define COL game_obj.user_int
	#define ROW game_obj.user_int2
	#define OFF game_obj.user_int3
	#define ID  game_obj.user_int4

	for(COL = 0; COL < map_columns; COL += 1)
	{
		for(ROW = 0; ROW < map_rows; ROW += 1)
		{
	OFF = TILE_OFFSET(COL,ROW);
	CREATE_GAME_OBJECT(ID)

	tile_object_id[OFF] = ID;
	tile_fileset[OFF] = "tile_black_patch";

	TILE_OBJ(COL, ROW).PROP_OBJECT_TYPE = "tile";
	TILE_OBJ(COL, ROW).filename = "tile_black_patch.bmp";

	if((COL == 0) || (ROW == 0) || (COL == map_columns -1) || (ROW == map_rows -1))
	{
		tile_id[OFF] = TILESET_BLACK + TILE_PATCH;
	}
	else
	{
		tile_id[OFF] = TILESET_GRASS + TILE_PATCH;
	}


	TILE_OBJ(COL, ROW).drawing_order = 0;
	TILE_OBJ(COL, ROW).visible = 1;
	TILE_OBJ(COL, ROW).animation_block = TileAnimation;
	TILE_OBJ(COL, ROW).PROP_OBJECT_STATE = STATE_INIT;

	TILE_OBJ(COL, ROW).PROP_OBJECT_COL = COL;
	TILE_OBJ(COL, ROW).PROP_OBJECT_ROW = ROW;
	CALC_X_OFFSET(COL, TILE_OBJ(COL, ROW).x)
	CALC_Y_OFFSET(ROW, TILE_OBJ(COL, ROW).y)
		}		
	}

	TRACE_VERBOSE("First Tile ID: " + tile_id[TILE_OFFSET(0,0)])
	TRACE_VERBOSE("Last Tile ID: " + tile_id[TILE_OFFSET(map_columns -1, map_rows -1)])

	#undef COL
	#undef ROW
	#undef OFF
	#undef ID


	//====================================
	//  ADD SOME NICE MAP FEATURES
	//  #1. A ROAD
	//====================================
	TMP_INT = (map_columns -2)/2 + 1;
	for(TMP_INT2 = (map_rows -2)/2 + 1; TMP_INT2 < map_rows -1; TMP_INT2 += 1)
	{
		tile_id[TILE_OFFSET(TMP_INT-1, TMP_INT2)] = TILESET_ROAD + TILE_LEFT;
		tile_id[TILE_OFFSET(TMP_INT, TMP_INT2)] = TILESET_ROAD + TILE_RIGHT;
	}

	TMP_INT2 = (map_rows -2)/2 + 1;
	for(TMP_INT = (map_columns-2)/2 + 2; TMP_INT < map_columns -1; TMP_INT += 1)
	{
		tile_id[TILE_OFFSET(TMP_INT, TMP_INT2-1)] = TILESET_ROAD + TILE_BOTTOM_CENTER;
		tile_id[TILE_OFFSET(TMP_INT, TMP_INT2)] = TILESET_ROAD + TILE_TOP_CENTER;
	}

	TMP_INT = (map_columns -2)/2 + 1;
	TMP_INT2 = (map_rows -2)/2 + 1;

	tile_id[TILE_OFFSET(TMP_INT, TMP_INT2)] = TILESET_ROAD + TILE_CENTER;
	tile_id[TILE_OFFSET(TMP_INT-1, TMP_INT2-1)] = TILESET_ROAD + TILE_BOTTOM_LEFT;
	tile_id[TILE_OFFSET(TMP_INT, TMP_INT2-1)] = TILESET_ROAD + TILE_BOTTOM_CENTER;

	//===============================
	//  INSERT A DIRT PATCH 
	//===============================
	INSERT_DIRT_PATCH(9,(map_rows-7),14,(map_rows-2))
}
