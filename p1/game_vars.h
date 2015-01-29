//====================================
// Game Control Objects
//====================================
rectangle game_obj(); // Controls the main loop of the game

int max_object_count = 1000;
int game_object_count = 0;
GAMEOBJECT game_objects[max_object_count];

int free_object_count = max_object_count;
int free_object_id[max_object_count];

int game_object_max_hp[max_object_count];
int game_object_hp[max_object_count]; // Target Object
int game_object_vulnerable[max_object_count];
int game_object_offset_x[max_object_count]; 
int game_object_offset_y[max_object_count];
int game_object_dest_col[max_object_count]; // Destination Column
int game_object_dest_row[max_object_count]; // Destination Row
int game_object_start_tick[max_object_count]; // First tick of the action
int game_object_end_tick[max_object_count]; // Last tick of the action
int game_object_duration[max_object_count]; // Duration of the action
int game_object_frame[max_object_count]; // Which frame of the animation
int game_object_frame_rate[max_object_count]; //The rate at which the frames change

int game_object_frame_count[max_object_count]; // The # of frames for the current action
int game_object_move_frame_count[max_object_count];
int game_object_attack_frame_count[max_object_count];
int game_object_hit_frame_count[max_object_count];
int game_object_die_frame_count[max_object_count];

int game_object_direction[max_object_count]; // The orientation of the object

// An object whose various user fields shall be used
// as temporary variables in the game_macro.cpp
GAMEOBJECT game_object_macro_temp();

//  DEBUG MODE OBJECTS
int enable_debug_mode = 1;
textbox txt_curr_anim();

//  Various Game Flags
int flag_uparrow = 0;
int flag_downarrow = 0;
int flag_leftarrow = 0;
int flag_rightarrow = 0;
int flag_spacebar = 0;
int flag_gameover = 0;

int spawn_rate = 50;
int last_spawn_tick = -1;

//  ANIMATION DECLARATIONS
forward animation NullAnimation(GAMEOBJECT null_obj);
forward animation GameAnimation(rectangle game_obj_param);
forward animation InitGameAnimation(rectangle game_init_obj);
