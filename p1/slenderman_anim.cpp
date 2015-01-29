#include "game_macros.cpp"
#include "slenderman_macros.cpp"

animation SlendermanAnimation(GAMEOBJECT slenderman_obj)
{
	#define S_OBJ		slenderman_obj
	#define S_ID		S_OBJ.PROP_OBJECT_ID
	#define S_COL		S_OBJ.PROP_OBJECT_COL
	#define S_ROW		S_OBJ.PROP_OBJECT_ROW
	#define S_STATE		S_OBJ.PROP_OBJECT_STATE
	#define P_OBJ		PLAYER_OBJ
		
	GET_VULNERABLE(S_OBJ, TMP_INT)
	GET_VULNERABLE(S_OBJ, TMP_INT2)

	// CHECK FOR PLAYER INTERACTION
	if(S_OBJ touches P_OBJ && TMP_INT || TMP_INT2 && P_OBJ.PROP_OBJECT_STATE != STATE_HIT && S_OBJ.PROP_OBJECT_STATE != STATE_HIT)
	{
		if(S_OBJ.PROP_OBJECT_COL == P_OBJ.PROP_OBJECT_COL && S_OBJ.PROP_OBJECT_ROW == P_OBJ.PROP_OBJECT_ROW && TMP_INT2)
		{
			//  HIT!
			PERFORM_HIT(P_OBJ)
		}
		else if(P_OBJ.PROP_OBJECT_STATE == STATE_ATTACK && TMP_INT)
		{
			//  CHECK TO SEE IF THE PLAYER HIT SLENDERMAN
			if(game_object_direction[player_id] == DIRECTION_UP && S_OBJ.PROP_OBJECT_ROW > P_OBJ.PROP_OBJECT_ROW)
			{
				PERFORM_HIT(S_OBJ)
			}
			else if(game_object_direction[player_id] == DIRECTION_LEFT && S_OBJ.PROP_OBJECT_COL < P_OBJ.PROP_OBJECT_COL)
			{
				PERFORM_HIT(S_OBJ)
			}
			else if(game_object_direction[player_id] == DIRECTION_DOWN && S_OBJ.PROP_OBJECT_ROW < P_OBJ.PROP_OBJECT_ROW)
			{
				PERFORM_HIT(S_OBJ)
			}
			else if(game_object_direction[player_id] == DIRECTION_RIGHT && S_OBJ.PROP_OBJECT_COL > P_OBJ.PROP_OBJECT_COL)
			{
				PERFORM_HIT(S_OBJ)
			}
			else if(TMP_INT2)
			{
				PERFORM_HIT(P_OBJ)
			}
		}
	}

	if(S_STATE == STATE_MOVE_UP)
	{
		MOVE_UP(S_OBJ)
	}
	else if(S_STATE == STATE_MOVE_DOWN)
	{
		MOVE_DOWN(S_OBJ)
	}
	else if(S_STATE == STATE_MOVE_LEFT)
	{
		MOVE_LEFT(S_OBJ)
	}
	else if(S_STATE == STATE_MOVE_RIGHT)
	{
		MOVE_RIGHT(S_OBJ)
	}
	else if(S_STATE == STATE_HIT)
	{
		PERFORM_HIT(S_OBJ)
	}
	else if(S_STATE == STATE_DIE)
	{
		PERFORM_DEATH(S_OBJ)
	}

	#define P_COL		P_OBJ.PROP_OBJECT_COL
	#define P_ROW		P_OBJ.PROP_OBJECT_ROW
	#define DIFF_X		game_obj.user_int
	#define DIFF_Y		game_obj.user_int2
	#define DIR_X		game_obj.user_int3
	#define DIR_Y		game_obj.user_int4
	//figure out the fastest route to the player
	else if(S_STATE == STATE_DEFAULT)
	{
		if(P_COL > S_COL)
		{
			DIFF_X = P_COL - S_COL;
			DIR_X = DIRECTION_RIGHT;
		}
		else
		{
			DIFF_X = S_COL - P_COL;
			DIR_X = DIRECTION_LEFT;
		}

		if(P_ROW > S_ROW)
		{
			DIFF_Y = P_ROW - S_ROW;
			DIR_Y = DIRECTION_UP;
		}
		else
		{
			DIFF_Y = S_ROW - P_ROW;
			DIR_Y = DIRECTION_DOWN;
		}

		if(DIFF_X >= DIFF_Y)
		{
			if(DIR_X == DIRECTION_RIGHT)
			{
				MOVE_RIGHT(S_OBJ)
			}
			else
			{
				MOVE_LEFT(S_OBJ)
			}
		}
		else
		{
			if(DIR_Y == DIRECTION_UP)
			{
				MOVE_UP(S_OBJ)
			}
			else
			{
				MOVE_DOWN(S_OBJ)
			}
		}
	}
	#undef P_COL
	#undef P_ROW	

	UPDATE_SPRITE(slenderman_obj)

	#undef S_ID
	#undef S_COL
	#undef S_ROW
	#undef S_STATE
	#undef P_OBJ
}
