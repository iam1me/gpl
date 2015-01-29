//=================================
//  ROOM VARIABLES
//=================================
int current_room = 0;
int last_room = 0;
int room_count = 1;
//int transition_north[room_count];
//int transition_east[room_count];
//int transition_west[room_count];
//int transition_south[room_count];

int tile_object_id[map_columns*map_rows];
int tile_id[map_columns*map_rows];
string tile_fileset[map_columns*map_rows];
int tile_walkable[map_columns*map_rows];

// Tile Animation(s)
forward animation TileAnimation(pixmap tile_obj);
