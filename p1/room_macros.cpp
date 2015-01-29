#ifndef ROOM_MACROS
#define ROOM_MACROS

#include "game_macros.cpp"

//Macros for tiles and loading a rooms tiles
//TILE ID CONVENTIONS
//	all top lefts are 0, 10, 20,...
//	all top centers are 1, 11, 21, ...
//	all top rights are 2, 12, 22, ...
//	all lefts are 3, 13, 23, ...
//	all centers are 4, 14, 24, ...
//	all rights are 5, 15, 25, ...
//	all bottom lefts are 6, 16, 26, ...
//	all bottom centers are 7, 17, 27, ...
//	all bottom rights are 8, 18, 28, ...
//	all individual patches are 9, 19, 29, ...

#define TILE_TOP_LEFT 0
#define TILE_TOP_CENTER 1
#define TILE_TOP_RIGHT 2
#define TILE_LEFT 3
#define TILE_CENTER 4
#define TILE_RIGHT 5
#define TILE_BOTTOM_LEFT 6
#define TILE_BOTTOM_CENTER 7
#define TILE_BOTTOM_RIGHT 8
#define TILE_PATCH 9

// TILESETS
// Combine with the above tile orientations to
// select a specific tile. Example: TILESET_ROAD + TILE_CENTER
#define TILESET_ROAD 10
#define TILESET_DIRT 20

//  These tileset only supports TILE_PATCH right now
#define TILESET_BLACK 0
#define TILESET_GRASS 30 

#define GET_TILE_INFO(tile_id, tilename, frame_count, bcan_walk)\
	TMP_INT = tile_id % 10;\
	TMP_INT2 = tile_id - TMP_INT;\
	if(TMP_INT2 == TILESET_ROAD)\
	{\
		tilename = "tile_road";\
		frame_count = 1;\
		bcan_walk = 1;\
	}\
	else if(TMP_INT2 == TILESET_DIRT)\
	{\
		tilename = "tile_dirt";\
		frame_count = 1; \
		bcan_walk = 1;\
	}\
	else if(TMP_INT2 == TILESET_GRASS)\
	{\
		tilename = "tile_grass";\
		frame_count = 1;\
		bcan_walk = 1;\
	}\
	else if(TMP_INT2 == TILESET_BLACK)\
	{\
		tilename = "tile_black";\
		frame_count = 1;\
		bcan_walk = 0;\
	}\
	else \
	{\
		TRACE_ERROR("Unrecognized Tile ID: " + tile_id)\
		exit(1);\
	}\
	\
	if(TMP_INT == 0)\
	{\
		tilename += "_top_left";\
	}\
	else if(TMP_INT == 1)\
	{\
		tilename += "_top_center";\
	}\
	else if(TMP_INT == 2)\
	{\
		tilename += "_top_right";\
	}\
	else if(TMP_INT == 3)\
	{\
		tilename += "_left";\
	}\
	else if(TMP_INT == 4)\
	{\
		tilename += "_center"; \
	}\
	else if(TMP_INT == 5)\
	{\
		tilename += "_right";\
	}\
	else if(TMP_INT == 6)\
	{\
		tilename += "_bottom_left";\
	}\
	else if(TMP_INT == 7)\
	{\
		tilename += "_bottom_center";\
	}\
	else if(TMP_INT == 8)\
	{\
		tilename += "_bottom_right";\
	}\
	else if(TMP_INT == 9)\
	{\
		tilename += "_patch";\
	}

// We create all of the tile objects at once so that
//	they are all can be referenced using the first
//	tile id as the starting index
#define TILE_OFFSET(col, row)\
	(col) + (row) * map_columns

#define GET_TILE_ID(col, row, id)\
	id = tile_id[TILE_OFFSET(col,row)];

#define SET_TILE_ID(col, row, id)\
	tile_id[TILE_OFFSET(col,row)] = id;

#define GET_TILE_OBJECT_ID(col, row, obj_id)\
	obj_id = tile_game_object_id[TILE_OFFSET(col,row)];

#define TILE_OBJ(col, row)\
	game_objects[tile_object_id[TILE_OFFSET(col,row)]]

#define SET_WALKABLE(col, row, bwalk)\
	tile_walkable[TILE_OFFSET(col,row)] = bwalk;

#define GET_WALKABLE(col, row, bwalk)\
	bwalk = tile_walkable[TILE_OFFSET(col,row)];


#define INSERT_DIRT_PATCH(start_col, start_row, end_col, end_row)\
	TMP_INT2 = start_row;\
	TMP_INT3 = end_row;\
	for(TMP_INT = start_col + 1; TMP_INT < end_col; TMP_INT += 1)\
	{\
		tile_id[TILE_OFFSET(TMP_INT, TMP_INT2)] = \
			TILESET_DIRT + TILE_BOTTOM_CENTER;\
		tile_id[TILE_OFFSET(TMP_INT, TMP_INT3)] = \
			TILESET_DIRT + TILE_TOP_CENTER;\
	}\
	TMP_INT = start_col;\
	TMP_INT3 = end_col;\
	for(TMP_INT2 = start_row + 1; TMP_INT2 < end_row; TMP_INT2 += 1)\
	{\
		tile_id[TILE_OFFSET(TMP_INT, TMP_INT2)] = \
			TILESET_DIRT + TILE_LEFT;\
		tile_id[TILE_OFFSET(TMP_INT3, TMP_INT2)] = \
			TILESET_DIRT + TILE_RIGHT;\
	}\
	\
	tile_id[TILE_OFFSET(start_col, start_row)] = \
		TILESET_DIRT + TILE_BOTTOM_LEFT;\
	tile_id[TILE_OFFSET(start_col, end_row)] = \
		TILESET_DIRT + TILE_TOP_LEFT;\
	tile_id[TILE_OFFSET(end_col, start_row)] = \
		TILESET_DIRT + TILE_BOTTOM_RIGHT;\
	tile_id[TILE_OFFSET(end_col, end_row)] = \
		TILESET_DIRT + TILE_TOP_RIGHT;\
	\
	for(TMP_INT = start_col + 1; TMP_INT < end_col; TMP_INT += 1)\
	{\
		for(TMP_INT2 = start_row + 1; TMP_INT2 < end_row; TMP_INT2 += 1)\
		{\
			tile_id[TILE_OFFSET(TMP_INT, TMP_INT2)] = \
				TILESET_DIRT + TILE_CENTER;\
		}\
	}
	

#endif
