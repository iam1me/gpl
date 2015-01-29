# In order to simplify game development, I am taking advantage of 
# the preprocessor and using Macros. This will take the test1.gpl
# file, run it through the preprocessor, and stick the output in
# out.gpl.

out.gpl: test1.cpp map_vars.h map_init.cpp map_anim.cpp room_vars.h room_init.cpp room_anim.cpp room_macros.cpp player_vars.h player_init.cpp player_anim.cpp player_macros.cpp game_vars.h game_init.cpp game_anim.cpp game_macros.cpp slenderman_vars.h slenderman_init.cpp slenderman_anim.cpp slenderman_macros.cpp
	g++ -E -P test1.cpp -o out.gpl


clean:
	rm out.gpl
