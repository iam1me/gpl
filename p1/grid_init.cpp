//==============================
//  INIT GRID
//==============================
initialization
{
	for(grid_column_iterator = 0; grid_column_iterator < grid_vertical_line_count; grid_column_iterator += 1)
	{
		grid_vertical_lines[grid_column_iterator].x = (grid_column_iterator + 1) * map_tile_width;
		grid_vertical_lines[grid_column_iterator].y = 0;
		grid_vertical_lines[grid_column_iterator].w = 1;
		grid_vertical_lines[grid_column_iterator].h = window_height;
		grid_vertical_lines[grid_column_iterator].red = 0;
		grid_vertical_lines[grid_column_iterator].green = 0;
		grid_vertical_lines[grid_column_iterator].blue = 0;
		grid_vertical_lines[grid_column_iterator].drawing_order = 1;
		grid_vertical_lines[grid_column_iterator].visible = enable_grid_mode;
	}

	for(grid_row_iterator = 0; grid_row_iterator < grid_horizontal_line_count; grid_row_iterator += 1)
	{
		grid_horizontal_lines[grid_row_iterator].x = 0;
		grid_horizontal_lines[grid_row_iterator].y = (grid_row_iterator + 1) * map_tile_height;
		grid_horizontal_lines[grid_row_iterator].w = window_width;
		grid_horizontal_lines[grid_row_iterator].h = 1;
		grid_horizontal_lines[grid_row_iterator].red = 0;
		grid_horizontal_lines[grid_row_iterator].green = 0;
		grid_horizontal_lines[grid_row_iterator].blue = 0;
		grid_horizontal_lines[grid_row_iterator].drawing_order = 1;
		grid_horizontal_lines[grid_row_iterator].visible = enable_grid_mode;
	}
}

