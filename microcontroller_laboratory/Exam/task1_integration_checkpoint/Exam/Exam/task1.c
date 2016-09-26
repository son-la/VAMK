#include "task1.h"
#include "sfr62p.h"
#include "lcd_utils.h"
#include "timer_stuff.h"
#include "stdlib.h"

enum task1_game_states task1_game_state; // Game state: START/STOP
enum task1_game_modes task1_game_mode; // Game mode: LCD_GAME/LED_GAME
enum task1_lcd_game_modes task1_lcd_game_mode; // LCD game mode: ONE_PLAYER/TWO_PLAYER

int task1_game_speed[3] = {200, 100, 50}; // Different game speed: 1s, 500 ms, 200 ms
int task1_game_speed_choice; // game speed choice: 0, 1, 2

unsigned char task1_led_data;//1111 1101
unsigned char task1_dir; // MIN_MAX for left to right, MAX_MIN for right to left

int task1_lose_blinking; // Gameover blinking toggle

int p1_y, p2_y,ball_x, ball_y; // LCD game data
int ball_x_val, ball_y_val; // pre-value


void task1_initialize(void)
{
	/* Parameters Initialization */
	task1_game_state = STOP;
	task1_game_mode = LED_GAME;
	task1_lcd_game_mode = TWO_PLAYER;
	task1_game_speed_choice = 0;
	task1_led_data = 0xFE;
	task1_dir = MIN_MAX;
	task1_lose_blinking = 0;
	
	if (task1_game_mode == LCD_GAME)
	{
		task1_LCDGameInit(&task1_dir, &p1_y, &p2_y, &ball_x, &ball_y, &ball_x_val, &ball_y_val);
		task1_LCDGameDisplay(task1_lcd_game_mode, p1_y, p2_y, ball_x, ball_y);
	}
	else
	{
		task1_LEDGameInit(&task1_dir, &task1_led_data);
		p4 = task1_led_data; 	
	}
	
	/* Configure timer and start game */
	timer_configuration();
}
void task1_changeGameMode(void)
{
	task1_game_state = STOP;
	if (task1_game_mode == LED_GAME)
	{
		task1_game_mode = LCD_GAME;
		task1_lcd_game_mode = TWO_PLAYER;
		// Reset gameplay parameter
		task1_LCDGameInit(&task1_dir, &p1_y, &p2_y, &ball_x, &ball_y, &ball_x_val, &ball_y_val);
		// Update lcd
		task1_LCDGameDisplay(task1_lcd_game_mode, p1_y, p2_y, ball_x, ball_y);	
	}
	else
	{
		task1_game_mode = LED_GAME;
		task1_LEDGameInit(&task1_dir, &task1_led_data);
	}
	task1_lose_blinking = 0;
}

int task1_checkGameOver_cnt = 0;
void task1_checkGameOver(void)
{
	if (++task1_checkGameOver_cnt > 10)
	{
		task1_checkGameOver_cnt = 0;
		if (task1_lose_blinking == 1)
		{
			// LED blinking
			task1_gameOver();	
		}
	}
}

int task1_playGame_cnt = 0;
void task1_playGame(void)
{
	if (++task1_playGame_cnt >= task1_game_speed[task1_game_speed_choice]){
		task1_playGame_cnt = 0;
		
		/* LED GAME */
		if (task1_game_state == START) 
		{		
			if (task1_game_mode == LED_GAME)
			{
				// Assign data to port LED
				p4 = task1_led_data; 

				task1_led_data = ~task1_led_data; // Inverse for processing 
				
				
				// Shifting
				if (task1_dir == MAX_MIN)
				{
					if (task1_led_data == MIN) //0001 or 1000
					{
						task1_game_state = STOP;
						task1_lose_blinking = 1;
					}
					else
						task1_led_data = task1_led_data >> 1;
					
						
				}
				else 
				{
					if (task1_led_data == MAX) //0001 or 1000
					{
						task1_game_state = STOP;
						task1_lose_blinking = 1;
					}
					else
						task1_led_data = task1_led_data << 1;
				}
					
				task1_led_data = ~task1_led_data; // Inverse back
			}//endif LED_GAME
			else // LCD_GAME
			{	
				// Check lose
				if (ball_x_val == 7) // Right edge
				{
					if (p1_y != ball_y_val)
					{
						task1_game_state = STOP;
						task1_lose_blinking = 1;	
						return ;
					}		
				}
				else if  (task1_lcd_game_mode == TWO_PLAYER && ball_x_val == 0) // Left edge in TWO-PLAYER mode
				{
					if (p2_y != ball_y_val)
					{
						task1_game_state = STOP;
						task1_lose_blinking = 1;
						return ;
					}
				}
			/* GAME NOT END */
				/* HIT BACKS */
				if (ball_x_val == 0)
				{
					ball_x_val = 1;
					task1_dir = MIN_MAX;	
				}
				else if (ball_x_val == 7)
				{
					ball_x_val = 6;	
					task1_dir = MAX_MIN;
				}
				
								
				// Update new position
				ball_x = ball_x_val;
				ball_y = ball_y_val;		
								
				
				// Update display
				task1_LCDGameDisplay(task1_lcd_game_mode, p1_y, p2_y, ball_x, ball_y);	
				
				
				
				// Prepare new position
				if (task1_dir == MIN_MAX)
				{
					++ball_x_val;
				}
				else
				{
					--ball_x_val;
					/* ONE-PLAYER case to bounce back automatically */
					if (task1_lcd_game_mode == ONE_PLAYER && ball_x_val == 0)
						task1_dir = MIN_MAX;
				}
				
				if (ball_x_val != 0 && ball_x_val != 7)
				{
					// Reset seed if need
					float r = (float)rand()/RAND_MAX;	
					if (r < 0.5)
						ball_y_val = !ball_y_val;
				}
				
			}// End game_mode LCD_GAME
		}// End game_state START
	}// End timer interval
}


void task1_changeGameSpeed(void)
{
	if (++task1_game_speed_choice == 3)
		task1_game_speed_choice = 0;
}


void task1_sw1(void)
{
	if (task1_game_state == START)
	{
		if (task1_game_mode == LED_GAME)
		{
			/* MIN hits */
			task1_led_data = ~task1_led_data; // Inverse for processing
			if (task1_led_data == MAX)
			{
				task1_dir = MAX_MIN;
				// led_data = 0x04;	
				// New state: 0010 <--
			}
			task1_led_data = ~task1_led_data; // Inverse back
		}
		else
		{
			p1_y = !p1_y;
			// Update display
			task1_LCDGameDisplay(task1_lcd_game_mode, p1_y, p2_y, ball_x, ball_y);	
		}
	}
	else
	{
		/* Start game */
		task1_game_state = START;
	}
}

void task1_sw2(void)
{
	if (task1_game_state == START)
	{
		/* LED GAME */
		if (task1_game_mode == LED_GAME)
		{
			/* MAX hits */
			task1_led_data = ~task1_led_data; // Inverse for processing
			if (task1_led_data == MIN)
			{
				task1_dir = MIN_MAX;
				// led_data = 0x02;	
				// New state: 0100 -->
			}
			task1_led_data = ~task1_led_data; // Inverse back
		}
		else /* LCD GAME */
		{
			if (task1_lcd_game_mode == TWO_PLAYER)
			{
				p2_y = !p2_y;
				// Update display
				task1_LCDGameDisplay(task1_lcd_game_mode, p1_y, p2_y, ball_x, ball_y);	
			}
		}
	}
}