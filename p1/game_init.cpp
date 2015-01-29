#ifndef GAME_INIT
#define GAME_INIT

//======================================
//  INIT GAME_OBJ
//======================================
initialization
{
	TRACE_VERBOSE("game_obj initialization...")
	//Give the lowest priority so that it is invisible
	game_obj.drawing_order = -3;
	game_obj.red = 0;
	game_obj.green = 0;
	game_obj.blue = 0;

	//Go to the Start Screen Animation
	game_obj.animation_block = InitGameAnimation;
	txt_curr_anim.visible = enable_debug_mode;
	txt_curr_anim.text = "initializing...";
	txt_curr_anim.x = 0;
	txt_curr_anim.y = 50;
	txt_curr_anim.size = 0.1;

	#define NDX game_obj.user_int
	#define CUR_OBJ game_objects[NDX]

	for(NDX = 0; NDX < max_object_count; NDX += 1)
	{
		CUR_OBJ.PROP_OBJECT_ID = NDX;
		CUR_OBJ.PROP_OBJECT_STATE = STATE_INIT;
		CUR_OBJ.PROP_OBJECT_TYPE = "null";

		CUR_OBJ.PROP_OBJECT_COL = 0;
		CUR_OBJ.PROP_OBJECT_ROW = 0;
		CUR_OBJ.x = -1; //off screen
		CUR_OBJ.y = -1; //off screen

		CUR_OBJ.filename = "tile_black_patch.bmp";
		CUR_OBJ.animation_block = NullAnimation;
		CUR_OBJ.visible = 1;

		free_object_id[NDX] = max_object_count - NDX;
		game_object_max_hp[NDX] = 0;
		game_object_hp[NDX] = 0;
		game_object_vulnerable[NDX] = 0;
		game_object_dest_col[NDX] = -1;
		game_object_dest_row[NDX] = -1;
		game_object_start_tick[NDX] = 0;
		game_object_end_tick[NDX] = 1;
		game_object_duration[NDX] = 1;
		game_object_frame[NDX] = 0;
		game_object_frame_rate[NDX] = 1;
		game_object_frame_count[NDX] = 1;
		game_object_move_frame_count[NDX] = 1;
		game_object_attack_frame_count[NDX] = 1;
		game_object_direction[NDX] = DIRECTION_UP;
		game_object_offset_x[NDX] = 0;
		game_object_offset_y[NDX] = 0;
	}
	#undef NDX
	#undef CUR_OBJ

	game_object_macro_temp.x = -1;
	game_object_macro_temp.y = -1;
	game_object_macro_temp.visible = 0;
}

#endif
