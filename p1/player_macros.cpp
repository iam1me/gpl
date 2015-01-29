#ifndef PLAYER_MACROS
#define PLAYER_MACROS

#include "game_macros.cpp"

#define PLAYER_OBJ game_objects[player_id]

#define PLAYER_ATTACK()\
	if(PLAYER_OBJ.PROP_OBJECT_STATE != STATE_ATTACK)\
	{\
		SET_STATE(PLAYER_OBJ, STATE_ATTACK)\
	}\
	GET_PRCNT_COMPLETE(PLAYER_OBJ, game_obj.user_double)\
	if(game_obj.user_double == 1.0)\
	{\
		SET_STATE(PLAYER_OBJ, STATE_DEFAULT)\
	}


#endif
