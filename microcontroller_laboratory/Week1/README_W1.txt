#LCD and LED games

files:
game_policy.c, game_policy.h: display and initialization functions
main.h: enums
main.c 
	- swithches operations
	- timer opeartions
	- registers configuration
	
---------------------------------------------------------------------
sw1_func():
	# Check game_state 
	if game_state is START:
		if LED_GAME:
			# Hit ball back if led_data is 0001
			# LED is showing 0010, direction MAX_MIN and After hit show 0010, direction MIN_MAX
		if LCD_GAME
			# move player 1 ']' up/down
			# Update display
	else:
		#Start game
		
sw2_func():
	# Check game_state 
	if game_state is START:
		if LED_GAME:
			# Hit ball back if led_data is 1000
			# LED is showing 0100, direction MIN_MAX and After hit show 0100, direction MAX_MIN
		if LCD_GAME
			# Check lcd_game_mode
			if TWO_PLAYER
				# move player 2 '[' up/down
				# Update display
				
sw3_func():
	#Increase game_speed
	
	
timer_a2_interrupt: One-shot timer

timer_a1_interrupt: Gameover blinking 500 ms

timer_a0_interrupt: Game opeartion clock
	# Check sw3 is pressed:
	if YES:
		# Count 3 seconds
			# game_state = STOP
			# clear screen
			# change game mode snf init value
			# update display if new game is LCD_GAME
			# Turn off gameover blinking
	if NO:
		# Clear 3-second counter
		
	# Check game_state, if START
	if LED_GAME:
		# Set LED on according to led_data
		# Move ball:
			if game_state is START
				# Check end game:
				if (led_Data == MIN as 0001) or (led_Data == MAX as 1000)
					# game_state = STOP
					# turn on GAMEOVER blinking
				else
					# Shift ball
					
				# prepare new led_data for next consideration
	
	if LCD_GAME:
		
		# Check end game:
		if ball_x_val == 7 in both modes and p1_y ']' != ball_y_val -> Miss
			AND
		if game_mode is TWO_PLAYER and ball_x_val == 0 and p2_y '[' != ball_y_val -> Miss
			-> 	# END GAME
				# game_state = STOP
				# turn on GAMEOVER blinking
				
		# Game not end
		
			# if ball_x_val == 0 -> ball_x_val = 1, dir = MIN_MAX (0 -> 7) as hit back
			# if ball_x_val == 7 -> ball_x_val = 6, dir = MAX_MIN (7 -> 0) as hit back
			
			# Update new position
			# Update ball_x = ball_x_val
			# Update ball_y = ball_y_val as random fashion
			
			# Update display
			
			# Prepare new location for next consideration					
				# New x position of ball
				if dir == MIN_MAX as 0 -> 7 - approaching first player
					++ball_x_val
				else as 7 -> 0 
					--ball_x_val
					if lcd_game_mode == ONE_PLAYER and ball_x_val == 0 -> bounce back automatically in ONE_PLAYER case
					# This does not affect TWO_PLAYER case because new position is 0 - lost position
				# New y position of ball
					# Generate random number 0 to 1, checking if < 0.5 -> reverse position
						
						
main()
	# Initialize display
	# Set sw3 as input
	# Configure timers
	# Start timer
						
						
******************** utils.c

led_game_init()				
	# Set direction MIN_MAX (0001 -> 1000)
	# Set led_data (11111110)

lcd_game_init()
	# Set direction MAX_MIN ------o] -> o------]
	# Set players positions
	# Set ball positions 	[------o]
							 --------
				
gameover()
	# Blinking GAMEOVER

lcd_game_display()
	# Position of player 1
	# Position of player 2
	# Position of ball
		
		
		
		
		


