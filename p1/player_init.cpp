#ifndef PLAYER_INIT
#define PLAYER_INIT

#include "game_init.cpp"
#include "game_macros.cpp"
#include "player_macros.cpp"

initialization
{
	CREATE_GAME_OBJECT(player_id)
	TRACE_VERBOSE("Player ID: " + player_id)

	PLAYER_OBJ.PROP_OBJECT_STATE = STATE_INIT;
	PLAYER_OBJ.animation_block = PlayerAnimation;
	PLAYER_OBJ.drawing_order = 200;
	PLAYER_OBJ.PROP_OBJECT_TYPE =  "player";

	game_object_max_hp[player_id] = player_total_hp;
	game_object_hp[player_id] = player_total_hp;
	game_object_vulnerable[player_id] = 1;
	game_object_frame_rate[player_id] = 1;
	game_object_move_frame_count[player_id] = 9;
	game_object_attack_frame_count[player_id] = 6;
	game_object_hit_frame_count[player_id] = 1;
	game_object_die_frame_count[player_id] = 6;
	game_object_offset_x[player_id] = 16;
	game_object_offset_y[player_id] = 0;

	#define H_NDX		game_obj.user_int
	#define CUR_OBJ		game_objects[heart_id[H_NDX]]
	for(H_NDX = 0; H_NDX < player_total_hp; H_NDX += 1)
	{
		CREATE_GAME_OBJECT(heart_id[H_NDX])
		CUR_OBJ.PROP_OBJECT_TYPE = "heart";
		CUR_OBJ.PROP_OBJECT_STATE = STATE_INIT;
		CUR_OBJ.animation_block = HeartAnimation;
		CUR_OBJ.drawing_order = 50;

		game_object_direction[heart_id[H_NDX]] = DIRECTION_DOWN;

		CUR_OBJ.PROP_OBJECT_COL = map_columns - H_NDX - 1;
		CUR_OBJ.PROP_OBJECT_ROW = map_rows - 1;
	}
	#undef H_NDX
	#undef CUR_OBJ
}

#endif
