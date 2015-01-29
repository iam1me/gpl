#ifndef GAME_MACROS_H
#define GAME_MACROS_H
//  GAME MACROS
//  General macros used by the game_anim.cpp

//  This is the base game object type
//  right now this is a circle, but it will be upgraded to
//  a pixmap ultimately
#define GAMEOBJECT pixmap

//  ASSUMPTIONS
//  In order to handle game objects in a general fashion,
//  the following assumptions are made when using these
//  macros:
//    1. The user_int holds the object id used for other game arrays
//    2. The user_int2 is currently unused
//    3. The user_int3 holds the current state of the object
//    4. The user_int4 holds the column that the object is at
//    5. The user_int5 holds the row that the object is at
//    6. The user_string holds the objects type
//
//  Here are the list of valid object states:
//    0. STATE_DEFAULT - The Object isn't doing anything.
//    1. STATE_MOVE_UP - The Object is in the process of moving up
//    2. STATE_MOVE_DOWN - The Object is in the process of moving down
//    3. STATE_MOVE_LEFT - The Object is in the process of moving left
//    4. STATE_MOVE_RIGHT - The Object is in the process of moving right
//
//  All of these settings must be set prior to calling these macros.
//  Do so in the initialization blocks. All coordinates are positive
//  so a target column/row of -1 will stand for NONE.

//  OBJ_ID_NDX is purposefully blank, correspond to how the user_xxx
//  are named. Example: user_int, user_int2, user_int3....
#define OBJ_ID_NDX abc
#define OBJ_TYPE_NDX 2
#define OBJ_STATE_NDX 3
#define OBJ_COL_NDX 4
#define OBJ_ROW_NDX 5

#define STATE_DEFAULT 0
#define STATE_MOVE_UP 1
#define STATE_MOVE_DOWN 2
#define STATE_MOVE_LEFT 3
#define STATE_MOVE_RIGHT 4 
#define STATE_INIT 5
#define STATE_ATTACK 6
#define STATE_HIT 7
#define STATE_DIE 8

#define PROP_OBJECT_ID 			user_int
#define PROP_OBJECT_ALLOCATED 		user_int2
#define PROP_OBJECT_STATE 		user_int3
#define PROP_OBJECT_COL 		user_int4
#define PROP_OBJECT_ROW 		user_int5

#define PROP_OBJECT_TYPE 		user_string

//#define PROP_OBJECT_HP			user_double
//#define PROP_OBJECT_FRAME_RATE		user_double2

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_LEFT 2
#define DIRECTION_RIGHT 3

#define TMP_INT  game_object_macro_temp.user_int
#define TMP_INT2 game_object_macro_temp.user_int2
#define TMP_INT3 game_object_macro_temp.user_int3
#define TMP_INT4 game_object_macro_temp.user_int4
#define TMP_INT5 game_object_macro_temp.user_int5

#define TMP_DBL  game_object_macro_temp.user_double
#define TMP_DBL2 game_object_macro_temp.user_double2
#define TMP_DBL3 game_object_macro_temp.user_double3
#define TMP_DBL4 game_object_macro_temp.user_double4
#define TMP_DBL5 game_object_macro_temp.user_double5

#define TMP_STR  game_object_macro_temp.user_string
#define TMP_STR2 game_object_macro_temp.user_string2
#define TMP_STR3 game_object_macro_temp.user_string3
#define TMP_STR4 game_object_macro_temp.user_string4
#define TMP_STR5 game_object_macro_temp.user_string5

//=================================
//  Define TRACE Macros
//=================================
#define __DEBUG

#ifdef __DEBUG
#define ENABLE_VERBOSE_TRACE
#define ENABLE_INFO_TRACE
#define ENABLE_WARNING_TRACE
#define ENABLE_ERROR_TRACE
#endif

#ifdef ENABLE_VERBOSE_TRACE
#define TRACE_VERBOSE(msg) print("VERBOSE: " + msg);
#else
#define TRACE_VERBOSE(msg)
#endif

#ifdef ENABLE_INFO_TRACE
#define TRACE_INFO(msg) print("INFO: " + msg);
#else
#define TRACE_INFO(msg) 
#endif

#ifdef ENABLE_WARNING_TRACE
#define TRACE_WARNING(msg) print("WARNING: " + msg);
#else
#define TRACE_WARNING(msg) 
#endif


#ifdef ENABLE_ERROR_TRACE
#define TRACE_ERROR(msg) print("ERROR: " + msg);
#else
#define TRACE_ERROR(msg) 
#endif


//  GET_COLUMN
//  Retrieves the objects current column
#define GET_COLUMN(obj, out_col)\
	out_col = obj.PROP_OBJECT_COL;

//  GET_ROW
//  Retrieves the objects current row
#define GET_ROW(object, out_row)\
	out_row = object.PROP_OBJECT_ROW;

//  GET_START_TICK
//  Retrieves the start tick of the objects current activity
//  The value is stored in out_tick
#define GET_START_TICK(obj, out_tick)\
	out_tick = game_object_start_tick[obj.PROP_OBJECT_ID];

//  GET_END_TICK
//  Retrieves the end tick of the objects current activity
//  The value is stored in out_tick
#define GET_END_TICK(obj, out_tick)\
	out_tick = game_object_end_tick[obj.PROP_OBJECT_ID];

//  GET_DURATION
//  Retrieves the duration of the objects current activity
//  The value is stored in out_duration
#define GET_DURATION(obj, out_duration)\
	out_duration = game_object_duration[obj.PROP_OBJECT_ID];

//  GET_MAX_HP
//  Returns the max hp for the object
#define GET_MAX_HP(obj, out_maxhp)\
	out_maxhp = game_object_max_hp[obj.PROP_OBJECT_ID];

//  GET_HP
//  Returns the current hp for the object
#define GET_HP(obj, out_hp)\
	out_hp = game_object_hp[obj.PROP_OBJECT_ID];

//  GET_VULNERABLE
//  Returns whether or not the object is vulnerable
#define GET_VULNERABLE(obj, out_vul)\
	out_vul = game_object_vulnerable[obj.PROP_OBJECT_ID];


//  SET_MAX_HP
//  Sets the max hp for the object. 
//  if hp > max_hp, hp will be set to max_hp
#define SET_MAX_HP(obj, max_hp)\
	game_object_max_hp[obj.PROP_OBJECT_ID] = (max_hp);\
	if(game_object_hp[obj.PROP_OBJECT_ID] > max_hp)\
	{\
		game_object_hp[obj.PROP_OBJECT_ID] = max_hp;\
	}

//  SET_HP
//  Sets the current hp for the object.
//  If hp > max_hp, hp will be set to max_hp
#define SET_HP(obj, cur_hp)\
	game_object_hp[obj.PROP_OBJECT_ID] = (cur_hp);\
	if((cur_hp) > game_object_max_hp[obj.PROP_OBJECT_ID])\
	{\
		game_object_hp[obj.PROP_OBJECT_ID] = \
			game_object_max_hp[obj.PROP_OBJECT_ID];\
	}

#define SET_VULNERABLE(obj, b_vul)\
	if(b_vul)\
	{\
		game_object_vulnerable[obj.PROP_OBJECT_ID] = 1;\
	}\
	else\
	{\
		game_object_vulnerable[obj.PROP_OBJECT_ID] = 0;\
	}


//  GET_TICKS_COMPLETE
//  Determines how many ticks have been completed for the
//  objects current action based upon the start tick and current tick
#define GET_TICKS_COMPLETE(obj, out_ticks)\
	GET_START_TICK(obj, out_ticks)\
	if(out_ticks > tick_count)\
	{\
		out_ticks -= max_tick_count;\
	}\
	out_ticks = tick_count - out_ticks;\
	if(out_ticks > game_object_duration[obj.PROP_OBJECT_ID])\
	{\
		out_ticks = game_object_duration[obj.PROP_OBJECT_ID];\
	}\

//  GET_PRCNT_COMPLETE
//  Determines how much of the action duration is complete
//  The value returned is between 0.0 and 1.0. 
//  The return value is stored in out_prcnt
#define GET_PRCNT_COMPLETE(obj, out_prcnt)\
	GET_TICKS_COMPLETE(obj, out_prcnt)\
	out_prcnt = (out_prcnt * 1.0) / (1.0 * game_object_duration[obj.PROP_OBJECT_ID]);

//  SET_DURATION
//  Sets the start and end tick for the current activity
#define SET_DURATION(obj, duration)\
	game_object_start_tick[obj.PROP_OBJECT_ID] = tick_count;\
	game_object_end_tick[obj.PROP_OBJECT_ID] = tick_count + duration;\
	if(game_object_end_tick[obj.PROP_OBJECT_ID] > max_tick_count)\
	{\
		game_object_end_tick[obj.PROP_OBJECT_ID] -= max_tick_count;\
	}\
	game_object_duration[obj.PROP_OBJECT_ID] = duration;

//  SET STATE
//  Sets the state and any other associated values
#define SET_STATE(obj, state)\
	obj.PROP_OBJECT_STATE = state;\
	if(state == STATE_DEFAULT)\
	{\
		game_object_frame_count[obj.PROP_OBJECT_ID] = 1;\
		CALC_OBJ_POS(obj)\
		SET_VULNERABLE(obj, true)\
	}\
	else if(state == STATE_MOVE_UP)\
	{\
		game_object_direction[obj.PROP_OBJECT_ID] = \
			DIRECTION_UP;\
		game_object_frame_count[obj.PROP_OBJECT_ID] = \
			game_object_move_frame_count[obj.PROP_OBJECT_ID];\
		SET_DURATION(obj, game_object_frame_count[obj.PROP_OBJECT_ID]\
			 * game_object_frame_rate[obj.PROP_OBJECT_ID])\
	}\
	else if(state == STATE_MOVE_DOWN)\
	{\
		game_object_direction[obj.PROP_OBJECT_ID] = \
			DIRECTION_DOWN;\
		game_object_frame_count[obj.PROP_OBJECT_ID] = \
			game_object_move_frame_count[obj.PROP_OBJECT_ID];\
		SET_DURATION(obj, game_object_frame_count[obj.PROP_OBJECT_ID]\
			 * game_object_frame_rate[obj.PROP_OBJECT_ID])\
	}\
	else if(state == STATE_MOVE_LEFT)\
	{\
		game_object_direction[obj.PROP_OBJECT_ID] = \
			DIRECTION_LEFT;\
		game_object_frame_count[obj.PROP_OBJECT_ID] = \
			game_object_move_frame_count[obj.PROP_OBJECT_ID];\
		SET_DURATION(obj, game_object_frame_count[obj.PROP_OBJECT_ID]\
			 * game_object_frame_rate[obj.PROP_OBJECT_ID])\
	}\
	else if(state == STATE_MOVE_RIGHT)\
	{\
		game_object_direction[obj.PROP_OBJECT_ID] = \
			DIRECTION_RIGHT;\
		game_object_frame_count[obj.PROP_OBJECT_ID] = \
			game_object_move_frame_count[obj.PROP_OBJECT_ID];\
		SET_DURATION(obj, game_object_frame_count[obj.PROP_OBJECT_ID]\
			 * game_object_frame_rate[obj.PROP_OBJECT_ID])\
	}\
	else if(state == STATE_ATTACK)\
	{\
		game_object_frame_count[obj.PROP_OBJECT_ID] = \
			game_object_attack_frame_count[obj.PROP_OBJECT_ID];\
		SET_DURATION(obj, game_object_frame_count[obj.PROP_OBJECT_ID]\
			 * game_object_frame_rate[obj.PROP_OBJECT_ID])\
	}\
	else if(state == STATE_HIT)\
	{\
		CALC_OBJ_POS(obj)\
		game_object_frame_count[obj.PROP_OBJECT_ID] = \
			game_object_hit_frame_count[obj.PROP_OBJECT_ID];\
		SET_DURATION(obj, game_object_frame_count[obj.PROP_OBJECT_ID]\
			* game_object_frame_rate[obj.PROP_OBJECT_ID])\
		SET_VULNERABLE(obj, false)\
	}\
	else if(state == STATE_DIE)\
	{\
		game_object_frame_count[obj.PROP_OBJECT_ID] = \
			game_object_die_frame_count[obj.PROP_OBJECT_ID];\
		SET_DURATION(obj, game_object_frame_count[obj.PROP_OBJECT_ID]\
			* game_object_frame_rate[obj.PROP_OBJECT_ID])\
	}

//  CREATE_GAME_OBJECT
//  Reserves the enxt free game object and returs its id.
//  This also increments the current count of game_objects
#define CREATE_GAME_OBJECT(out_id)\
	if((game_object_count == max_object_count)\
		|| (free_object_count == 0))\
	{\
		TRACE_ERROR("ERROR: The Max Game Object Count Exceeded")\
		exit(1);\
	}\
	out_id = free_object_id[free_object_count - 1];\
	if(game_objects[out_id].PROP_OBJECT_ALLOCATED != 0)\
	{\
		TRACE_ERROR("ERROR: Object Already Allocated")\
		exit(1);\
	}\
	TRACE_VERBOSE("CREATE_GAME_OBJECT - NEW ID: " + out_id)\
	free_object_id[free_object_count - 1] = -1;\
	free_object_count -= 1;\
	game_object_count += 1;\
	\
	game_objects[out_id].visible = 1;\
	game_objects[out_id].x = 0;\
	game_objects[out_id].y = map_tile_height;\
	game_objects[out_id].PROP_OBJECT_ID = out_id;\
	game_objects[out_id].PROP_OBJECT_ALLOCATED = 1;




#define DESTROY_GAME_OBJECT(obj_id)\
	if(game_object_count == 0\
		 || free_object_count == max_object_count)\
	{\
		TRACE_ERROR("DESTROY_GAME_OBJECT: No Game Objects have yet been allocated.")\
		exit(1);\
	}\
	else if(game_objects[obj_id].PROP_OBJECT_ALLOCATED == 0)\
	{\
		TRACE_ERROR("DESTROY_GAME_OBJECT: Object Not Allocated")\
		exit(1);\
	}\
	game_object_count -= 1;\
	free_object_id[free_object_count] = obj_id;\
	free_object_count += 1;\
	\
	game_objects[obj_id].x = -1;\
	game_objects[obj_id].y = -1;\
	game_objects[obj_id].PROP_OBJECT_TYPE = "null";\
	game_objects[obj_id].PROP_OBJECT_ALLOCATED = 0;\
	game_objects[obj_id].animation_block = NullAnimation;

//  CALC_Y_OFFSET
//  Takes a row # and calculates the y-offset to the bottom thereof
#define CALC_Y_OFFSET(row, out_y)\
	out_y = row * map_tile_height;

//  CALC_X_OFFSET
//  Takes a col # and calculates the x-offset to the left thereof
#define CALC_X_OFFSET(col, out_x)\
	out_x = col * map_tile_width - map_tile_width;

//  CALC_OBJ_POS
//  Takes an object and sets is position based upon its settings
#define CALC_OBJ_POS(obj)\
	CALC_X_OFFSET(obj.PROP_OBJECT_COL, obj.x)\
	CALC_Y_OFFSET(obj.PROP_OBJECT_ROW, obj.y)\
	obj.x += game_object_offset_x[obj.PROP_OBJECT_ID];\
	obj.y += game_object_offset_y[obj.PROP_OBJECT_ID];

//  UPDATE_SPRITE
//  Sets the game_objects sprite to the appropriate file
//  File Naming Convention:
//        <object_type>_<action_name>_<direction>_<frame#>.bmp
#define UPDATE_SPRITE(obj)\
	TMP_STR = obj.PROP_OBJECT_TYPE;\
	if(obj.PROP_OBJECT_STATE == STATE_MOVE_UP || \
		obj.PROP_OBJECT_STATE == STATE_MOVE_DOWN || \
		obj.PROP_OBJECT_STATE == STATE_MOVE_LEFT || \
		obj.PROP_OBJECT_STATE == STATE_MOVE_RIGHT)\
	{\
		TMP_STR += "_walk";\
	}\
	else if(obj.PROP_OBJECT_STATE == STATE_ATTACK)\
	{\
		TMP_STR += "_attack";\
	}\
	else if(obj.PROP_OBJECT_STATE == STATE_HIT)\
	{\
		TMP_STR += "_hit";\
	}\
	else if(obj.PROP_OBJECT_STATE == STATE_DIE)\
	{\
		TMP_STR += "_die";\
	}\
	else if(obj.PROP_OBJECT_STATE == STATE_DEFAULT)\
	{\
		TMP_STR += "_" + "default";\
	}\
	else\
	{\
		TRACE_ERROR("UPDATE_SPRITE - Unrecognized State: " + obj.PROP_OBJECT_STATE)\
		exit(1);\
	}\
	TMP_INT = game_object_direction[obj.PROP_OBJECT_ID];\
	if(TMP_INT == DIRECTION_UP)\
	{\
		TMP_STR += "_up";\
	}\
	else if(TMP_INT == DIRECTION_DOWN)\
	{\
		TMP_STR += "_down";\
	}\
	else if(TMP_INT == DIRECTION_LEFT)\
	{\
		TMP_STR += "_left";\
	}\
	else if(TMP_INT == DIRECTION_RIGHT)\
	{\
		TMP_STR += "_right";\
	}\
	else\
	{\
		TRACE_ERROR("UPDATE_SPRITE - Unrecognized Direction: " + game_obj.user_int)\
		exit(1);\
	}\
	TMP_INT2 = game_object_frame_count[obj.PROP_OBJECT_ID];\
	GET_PRCNT_COMPLETE(obj, TMP_DBL)\
	TMP_STR += "_" + floor(TMP_DBL * (TMP_INT2 - 1));\
	obj.filename = TMP_STR + ".bmp";
	

#define PERFORM_HIT(obj)\
	if(obj.PROP_OBJECT_STATE != STATE_HIT)\
	{\
		TRACE_VERBOSE("PERFORM_HIT on " + obj.PROP_OBJECT_ID)\
		GET_VULNERABLE(obj, TMP_INT)\
		if(TMP_INT)\
		{\
			GET_HP(obj, TMP_INT)\
			TMP_INT -= 1;\
			SET_HP(obj, TMP_INT)\
			SET_STATE(obj, STATE_HIT)\
		}\
	}\
	else\
	{\
		GET_PRCNT_COMPLETE(obj, TMP_DBL)\
		if(TMP_DBL >= 1.0)\
		{\
			GET_HP(obj, TMP_INT)\
			if(TMP_INT > 0)\
			{\
				TMP_INT = random(map_columns -2) + 1;\
				TMP_INT2 = random(map_rows -2) + 1;\
				SET_POSITION(obj, TMP_INT, TMP_INT2)\
				SET_STATE(obj, STATE_DEFAULT)\
			}\
			else\
			{\
				PERFORM_DEATH(obj)\
			}\
		}\
	}

#define PERFORM_DEATH(obj)\
	if(obj.PROP_OBJECT_STATE != STATE_DIE)\
	{\
		SET_STATE(obj, STATE_DIE)\
	}\
	else\
	{\
		GET_PRCNT_COMPLETE(obj, TMP_DBL)\
		if(TMP_DBL >= 1.0)\
		{\
			DESTROY_GAME_OBJECT(obj.PROP_OBJECT_ID)\
		}\
	}


//  MOVE_UP
//  Takes the supplied GAMEOBJECT and moves it up one tile
//  This occurs over the specified time interval.
#define MOVE_UP(obj)\
	if(obj.PROP_OBJECT_STATE != STATE_MOVE_UP)\
	{\
                SET_STATE(obj, STATE_MOVE_UP)\
	}\
	else\
	{\
		GET_PRCNT_COMPLETE(obj, TMP_DBL)\
		GET_ROW(obj, TMP_INT)\
		CALC_Y_OFFSET(TMP_INT, TMP_INT)\
		obj.y = TMP_INT + floor(map_tile_height * TMP_DBL);\
		obj.y += game_object_offset_y[obj.PROP_OBJECT_ID];\
		if(TMP_DBL >= 1.0)\
		{\
			obj.PROP_OBJECT_ROW += 1;\
			SET_STATE(obj, STATE_DEFAULT)\
		}\
	}\

//  MOVE_RIGHT
//  Takes the object and move it right one tile
//  This is an iterative function that will need to be called
//  many times to complete. While in progress, the status will
//  be set to STATE_MOVE_RIGHT. When complete it will be set
//  back to STATE_DEFAULT.
#define MOVE_RIGHT(obj)\
	if(obj.PROP_OBJECT_STATE != STATE_MOVE_RIGHT)\
	{\
                SET_STATE(obj, STATE_MOVE_RIGHT)\
	}\
	else\
	{\
		GET_PRCNT_COMPLETE(obj, TMP_DBL)\
		GET_COLUMN(obj, TMP_INT)\
		CALC_X_OFFSET(TMP_INT, TMP_INT)\
		obj.x = TMP_INT + floor(map_tile_width * TMP_DBL);\
		obj.x += game_object_offset_x[obj.PROP_OBJECT_ID];\
		if(TMP_DBL >= 1.0)\
		{\
			obj.PROP_OBJECT_COL += 1;\
			SET_STATE(obj, STATE_DEFAULT)\
		}\
	}\

#define MOVE_LEFT(obj)\
	if(obj.PROP_OBJECT_STATE != STATE_MOVE_LEFT)\
	{\
                SET_STATE(obj, STATE_MOVE_LEFT)\
	}\
	else\
	{\
		GET_PRCNT_COMPLETE(obj, TMP_DBL)\
		GET_COLUMN(obj, TMP_INT)\
		CALC_X_OFFSET(TMP_INT, TMP_INT)\
		obj.x = TMP_INT - floor(map_tile_width * TMP_DBL);\
		obj.x += game_object_offset_x[obj.PROP_OBJECT_ID];\
		if(TMP_DBL >= 1.0)\
		{\
			obj.PROP_OBJECT_COL -= 1;\
			SET_STATE(obj, STATE_DEFAULT)\
		}\
	}\


#define MOVE_DOWN(obj)\
	if(obj.PROP_OBJECT_STATE != STATE_MOVE_DOWN)\
	{\
                SET_STATE(obj, STATE_MOVE_DOWN)\
	}\
	else\
	{\
		GET_PRCNT_COMPLETE(obj, TMP_DBL)\
		GET_ROW(obj, TMP_INT)\
		CALC_Y_OFFSET(TMP_INT, TMP_INT)\
		obj.y = TMP_INT - floor(map_tile_height * TMP_DBL);\
		obj.y += game_object_offset_y[obj.PROP_OBJECT_ID];\
		if(TMP_DBL >= 1.0)\
		{\
			obj.PROP_OBJECT_ROW -= 1;\
			SET_STATE(obj, STATE_DEFAULT)\
		}\
	}\

#define SET_POSITION(obj, col, row)\
	obj.PROP_OBJECT_COL = col;\
	obj.PROP_OBJECT_ROW = row;\
	CALC_X_OFFSET(obj.PROP_OBJECT_COL, obj.x)\
	CALC_Y_OFFSET(obj.PROP_OBJECT_ROW, obj.y)\
	obj.x += game_object_offset_x[obj.PROP_OBJECT_ID];\
	obj.y += game_object_offset_y[obj.PROP_OBJECT_ID];

#endif
