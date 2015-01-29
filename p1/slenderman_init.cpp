#include "game_macros.cpp"
#include "slenderman_macros.cpp"

/*
initialization
{
	CREATE_GAME_OBJECT(slenderman_id)
	SLENDERMAN_OBJ.PROP_OBJECT_TYPE = "slenderman";
	SLENDERMAN_OBJ.PROP_OBJECT_COL = random(map_columns-2) + 1;
	SLENDERMAN_OBJ.PROP_OBJECT_ROW = random(map_rows -2) + 1;
	SLENDERMAN_OBJ.drawing_order = 150;
	SLENDERMAN_OBJ.animation_block = SlendermanAnimation;

	game_object_max_hp[slenderman_id] = 1;
	game_object_hp[slenderman_id] = 1;
	game_object_vulnerable[slenderman_id] = 1;
	game_object_frame_rate[slenderman_id] = 3;
	game_object_move_frame_count[slenderman_id] = 4;
	game_object_attack_frame_count[slenderman_id] = 1;
	game_object_hit_frame_count[slenderman_id] = 5;
	game_object_die_frame_count[slenderman_id] = 5;

	SET_STATE(SLENDERMAN_OBJ, STATE_DEFAULT)
}*/
