//=============================
//  PLAYER VARIABLES
//=============================
int player_id = -1;
int player_total_hp = 3;
int player_hp = player_total_hp;
int heart_id[player_total_hp];

forward animation PlayerAnimation(GAMEOBJECT player_obj);
forward animation HeartAnimation(GAMEOBJECT heart_obj);
