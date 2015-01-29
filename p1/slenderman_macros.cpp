#ifndef SLENDERMAN_MACROS
#define SLENDERMAN_MACROS

#include "game_macros.cpp"

#define SLENDERMAN_OBJ\
	game_objects[slenderman_id]

#define CREATE_SLENDERMAN_OBJ(out_id)\
	CREATE_GAME_OBJECT(out_id)\
	game_objects[out_id].PROP_OBJECT_TYPE = "slenderman";\
	game_objects[out_id].PROP_OBJECT_COL = random(map_columns-1) + 1;\
	game_objects[out_id].PROP_OBJECT_ROW = random(map_rows -1) + 1;\
	game_objects[out_id].drawing_order = 100;\
	game_objects[out_id].animation_block = SlendermanAnimation;\
	\
	game_object_max_hp[out_id] = 1;\
	game_object_hp[out_id] = 1;\
	game_object_vulnerable[out_id] = 1;\
	game_object_frame_rate[out_id] = 2;\
	game_object_move_frame_count[out_id] = 4;\
	game_object_attack_frame_count[out_id] = 1;\
	game_object_hit_frame_count[out_id] = 5;\
	game_object_die_frame_count[out_id] = 5;\
	\
	SET_STATE(game_objects[out_id], STATE_DEFAULT)

#endif
