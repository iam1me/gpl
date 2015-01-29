#include "player_macros.cpp"
#include "game_macros.cpp"
#include "room_macros.cpp"

animation PlayerAnimation(GAMEOBJECT player_obj)
{
	#define P_OBJ		player_obj
	#define P_ID 		player_obj.PROP_OBJECT_ID
	#define P_COL 		player_obj.PROP_OBJECT_COL
	#define P_ROW		player_obj.PROP_OBJECT_ROW
	#define P_STATE		player_obj.PROP_OBJECT_STATE
	#define P_MAX_HP	game_object_max_hp[player_id]
	#define P_HP		game_object_hp[player_id]
	
	//  HANDLE CHARACTER INITIALIZATION
	if(P_STATE == STATE_INIT)
	{
		game_obj.user_int = random(map_columns - 1);
		game_obj.user_int2 = random(map_rows - 1);
		SET_POSITION(player_obj, game_obj.user_int, game_obj.user_int2)
		SET_STATE(player_obj,STATE_DEFAULT)

		TRACE_VERBOSE("Initial Player Location: (" + game_obj.user_int + ", " + game_obj.user_int2)
	}

	//  HANDLE BEING HIT
	if(P_STATE == STATE_HIT)
	{
		PERFORM_HIT(P_OBJ)
	}	

	//  HANDLE DEATH STATE
	else if(P_STATE == STATE_DIE)
	{
		game_object_direction[player_id] = DIRECTION_DOWN;

		GET_PRCNT_COMPLETE(P_OBJ, TMP_DBL)
		if(TMP_DBL < 1.0)
		{
			PERFORM_DEATH(P_OBJ)
		}
		else
		{
			TRACE_VERBOSE("REVIVE PLAYER!")
			GET_MAX_HP(P_OBJ, TMP_INT)
			SET_HP(P_OBJ, TMP_INT)
			TMP_INT = random(map_columns -1) + 1;
			TMP_INT2 = random(map_rows -1) + 1;
			SET_POSITION(P_OBJ, TMP_INT, TMP_INT2)
			SET_STATE(P_OBJ, STATE_DEFAULT)
		}
	}

	//  HANDLE CHARACTER MOVEMENT
	else if(P_STATE == STATE_MOVE_UP)
	{
		if(P_ROW < map_rows - 1 && tile_walkable[TILE_OFFSET(P_COL,P_ROW+1)])
		{
			MOVE_UP(player_obj)
		}
		else
		{
			SET_STATE(player_obj,STATE_DEFAULT)
		}

		if(game_object_direction[P_ID] != DIRECTION_UP)
		{
			TRACE_ERROR("Character Direction Not UP after MOVE_UP")
		}
	}
	else if(P_STATE == STATE_MOVE_DOWN)
	{
		//TRACE_VERBOSE("OFFSET(" + P_COL + "," + (P_ROW -1) + "): " + TILE_OFFSET(P_COL, (P_ROW-1)))
		if(P_ROW > 0 && tile_walkable[TILE_OFFSET(P_COL,P_ROW-1)])
		{
			MOVE_DOWN(player_obj)
		}
		else
		{
			SET_STATE(player_obj, STATE_DEFAULT)
		}

		if(game_object_direction[P_ID] != DIRECTION_DOWN)
		{
			TRACE_ERROR("Character Direction Not DOWN after MOVE_DOWN")
		}
	}
	else if(P_STATE == STATE_MOVE_LEFT)
	{
		TMP_INT = tile_walkable[TILE_OFFSET(P_COL - 1, P_ROW)];
		if(P_COL > 0 && TMP_INT)
		{
			MOVE_LEFT(player_obj)
		}
		else
		{
			if(!TMP_INT)
			{
				print("Left Boundary Hit (" + (P_COL - 1) + ", " + P_ROW + ")");
			}
			SET_STATE(player_obj, STATE_DEFAULT)
		}

		if(game_object_direction[P_ID] != DIRECTION_LEFT)
		{
			TRACE_ERROR("Character Direction Not LEFT after MOVE_LEFT")
		}
	}
	else if(P_STATE == STATE_MOVE_RIGHT)
	{
		TMP_INT = tile_walkable[TILE_OFFSET(P_COL + 1, P_ROW)];
		if(P_COL < (map_rows - 1) && TMP_INT)
		{
			MOVE_RIGHT(player_obj)
		}
		else
		{
			if(!TMP_INT) 
			{
				print("Right Boundary Hit (" + (P_COL + 1) + ", " + P_ROW + ")");
				print("Player Location: (" + P_COL + ", " + P_ROW + ")");
			}
			SET_STATE(player_obj, STATE_DEFAULT)
		}

		if(game_object_direction[P_ID] != DIRECTION_RIGHT)
		{
			TRACE_ERROR("Character Direction Not RIGHT after MOVE_RIGHT")
		}
	}
	else if(P_STATE == STATE_ATTACK)
	{
		PLAYER_ATTACK()
	}

	// UPDATE SPRITE
	UPDATE_SPRITE(player_obj)

	#undef P_OBJ
	#undef P_ID
	#undef P_COL
	#undef P_ROW
	#undef P_STATE
	#undef P_MAX_HP
	#undef P_HP
}

animation HeartAnimation(GAMEOBJECT heart_obj)
{
	#define OBJ_ID		heart_obj.PROP_OBJECT_ID
	#define H_NDX		(OBJ_ID - heart_id[0])
	#define H_COL		heart_obj.PROP_OBJECT_COL
	#define H_ROW		heart_obj.PROP_OBJECT_ROW
	#define H_X		heart_obj.x
	#define H_Y		heart_obj.y
	#define H_STATE		heart_obj.PROP_OBJECT_STATE
	#define H_FILE		heart_obj.filename
	#define P_HP		game_object_hp[player_id]

	if(H_STATE == STATE_INIT)
	{
		CALC_X_OFFSET(H_COL, H_X)
		CALC_Y_OFFSET(H_ROW, H_Y)
		H_FILE = "heart_full.bmp";
		H_STATE = STATE_DEFAULT;
	}
	else if(H_STATE == STATE_DEFAULT)
	{
		if(P_HP > H_NDX)
		{
			H_FILE = "heart_full.bmp";
		}
		else
		{
			H_FILE = "heart_empty.bmp";
		}
	}

	#undef OBJ_ID
	#undef H_COL
	#undef H_ROW
	#undef H_X
	#undef H_Y
	#undef H_STATE
	#undef H_FILE
	#undef P_HP
}

on space
{
	flag_spacebar = 1;
	if(PLAYER_OBJ.PROP_OBJECT_STATE == STATE_DEFAULT)
	{
		PLAYER_ATTACK()
	}
}

on leftarrow
{
	flag_leftarrow = 1;
	if(PLAYER_OBJ.PROP_OBJECT_STATE == STATE_DEFAULT)
	{
		if(game_object_direction[player_id] == DIRECTION_LEFT)
		{
			MOVE_LEFT(PLAYER_OBJ)
		}
		else
		{
			game_object_direction[player_id] = DIRECTION_LEFT;
		}
	}
}

on rightarrow
{
	flag_rightarrow = 1;
	if(PLAYER_OBJ.PROP_OBJECT_STATE == STATE_DEFAULT)
	{
		if(game_object_direction[player_id] == DIRECTION_RIGHT)
		{
			MOVE_RIGHT(PLAYER_OBJ)
		}
		else
		{
			game_object_direction[player_id] = DIRECTION_RIGHT;
		}
	}
}

on uparrow
{
	flag_uparrow = 1;
	if(PLAYER_OBJ.PROP_OBJECT_STATE == STATE_DEFAULT)
	{
		if(game_object_direction[player_id] == DIRECTION_UP)
		{
			MOVE_UP(PLAYER_OBJ)
		}
		else
		{
			game_object_direction[player_id] = DIRECTION_UP;
		}
	}
}

on downarrow
{
	flag_downarrow = 1;
	if(PLAYER_OBJ.PROP_OBJECT_STATE == STATE_DEFAULT)
	{
		if(game_object_direction[player_id] == DIRECTION_DOWN)
		{
			MOVE_DOWN(PLAYER_OBJ)
		}
		else
		{
			game_object_direction[player_id] = DIRECTION_DOWN;
		}
	}
}
