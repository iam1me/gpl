//=================================================
//	CLOCK TICK ANIMATION
//=================================================
animation ClockTickAnimation(rectangle clock_obj)
{
	tick_count += 1;
	if(tick_count > max_tick_count) tick_count = 0;
}

