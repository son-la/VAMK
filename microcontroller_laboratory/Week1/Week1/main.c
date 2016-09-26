/***********************************************************************************
FILE NAME  	:	main.c
DESCRIPTION :	Empty proejct set up for RSKM16C62P					


Copyright   : 2005 Renesas Technology Europe Ltd.
Copyright   : 2005 Renesas Technology Corporation.
All Rights Reserved

***********************************************************************************/


#include "sfr62p.h"
/* rskM16C62p_def.h defines some common definitions */
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "stdlib.h"
#include "lcd.h"
#include "utils.h"

#pragma INTERRUPT timer_a0_interrupt
#pragma INTERRUPT timer_a1_interrupt
#pragma INTERRUPT timer_a2_interrupt

#pragma INTERRUPT sw1
#pragma INTERRUPT sw2
#pragma INTERRUPT sw3


/* Initialization */
enum game_states game_state = STOP; // Game state: START/STOP
enum game_modes game_mode = LED_GAME; // Game mode: LCD_GAME/LED_GAME
enum lcd_game_modes lcd_game_mode = TWO_PLAYER; // LCD game mode: ONE_PLAYER/TWO_PLAYER

int game_speed[3] = {200, 100, 50}; // Different game speed: 1s, 500 ms, 200 ms
int game_speed_choice = 0; // game speed choice: 0, 1, 2

unsigned char led_data = 0xFE;//1111 1101
unsigned char dir = MIN_MAX; // MIN_MAX for left to right, MAX_MIN for right to left

int lose_blinking = 0; // Gameover blinking toggle

int p1_y, p2_y,ball_x, ball_y; // LCD game data
int ball_x_val, ball_y_val; // pre-value

/* Switch functions*/
int sw_press = 1;
void sw1_func(void)
{
	if (game_state == START)
	{
		if (game_mode == LED_GAME)
		{
			/* MIN hits */
			led_data = ~led_data; // Inverse for processing
			if (led_data == MAX)
			{
				dir = MAX_MIN;
				// led_data = 0x04;	
				// New state: 0010 <--
			}
			led_data = ~led_data; // Inverse back
		}
		else
		{
			p1_y = !p1_y;
			// Update display
			lcd_game_display(lcd_game_mode, p1_y, p2_y, ball_x, ball_y);	
		}
	}
	else
	{
		/* Start game */
		game_state = START;
	}
	
}

void sw2_func(void)
{
	if (game_state == START)
	{
		/* LED GAME */
		if (game_mode == LED_GAME)
		{
			/* MAX hits */
			led_data = ~led_data; // Inverse for processing
			if (led_data == MIN)
			{
				dir = MIN_MAX;
				// led_data = 0x02;	
				// New state: 0100 -->
			}
			led_data = ~led_data; // Inverse back
		}
		else /* LCD GAME */
		{
			if (lcd_game_mode == TWO_PLAYER)
			{
				p2_y = !p2_y;
				// Update display
				lcd_game_display(lcd_game_mode, p1_y, p2_y, ball_x, ball_y);	
			}
		}
	}
}

void sw3_func(void) // Change game speed
{
	int i;
	if (++game_speed_choice == 3)
		game_speed_choice = 0;
}


/* One-short configuration START */
void sw1(void)
{
	/*
	if (sw_press == 1)
	{
		sw_press = 0;
		sw1_func();
		ta2os = 1;
	}
	*/
		sw1_func();
}

void sw2(void)
{
	/*
	if (sw_press == 1)
	{
		sw_press = 0;
		sw2_func();
		ta2os = 1;
	}
	*/
	sw2_func();
}

void sw3(void)
{
	
	if (sw_press == 1)
	{
		sw_press = 0;
		sw3_func();
		ta2os = 1;
	}
	
	//sw3_func();
	
}


int timer_a2_interrupt_cnt = 0;
void timer_a2_interrupt(void) // One-shot
{
  	if (++timer_a2_interrupt_cnt == 3) // 30 ms
	{
   		sw_press = 1;
		timer_a2_interrupt_cnt = 0;
		return;
	}
	ta2os = 1; // renew timer	
}

/* One-short configuration END */


/* Timer configuration */
int timer_a1_interrupt_cnt = 0;
void timer_a1_interrupt(void) // LCD_display, just for lose blinking - NEED UPGRADE !!
{
	if (++timer_a1_interrupt_cnt == 10)
	{
		timer_a1_interrupt_cnt = 0;
		
		if (lose_blinking == 1)
		{
			gameover();	
		}
	}	
}

int timer_a0_interrupt_cnt = 0;
int sw3_pressed = 0;
void timer_a0_interrupt(void) // GAME timer
{
	if (++timer_a0_interrupt_cnt >= game_speed[game_speed_choice]){
		timer_a0_interrupt_cnt = 0;
		
		// Press sw3 to change game mode
		if (p8_4 == 0)
		{
			if (++sw3_pressed >= (300 / game_speed[game_speed_choice])) 
			// 300 = 10ms x 300 = 3 seconds
			// E.g.: game_speed_choice = 0 -> game_speed = 100 -> perform ++sw3_pressed 3 times.
			{
				// Stop game
				game_state = STOP;
				// Clear screen
				LCD_write(CTRL_WR,LCD_CLEAR);
				
				// Change game mode and init 
				// ** Game hasn't started yet
				if (game_mode == LED_GAME)
				{
					game_mode = LCD_GAME;
					lcd_game_mode = TWO_PLAYER;
					// Reset gameplay parameter
					lcd_game_init(&dir, &p1_y, &p2_y, &ball_x, &ball_y, &ball_x_val, &ball_y_val);
					// Update lcd
					lcd_game_display(lcd_game_mode, p1_y, p2_y, ball_x, ball_y);	
				}
				else
				{
					game_mode = LED_GAME;
					led_game_init(&dir, &led_data);
				}
				lose_blinking = 0;
			}
		}
		else
			sw3_pressed = 0; // sw3 release
		
		
		/* Move ball */
		
		/* LED GAME */
		if (game_state == START) 
		{		
			if (game_mode == LED_GAME)
			{
				// Assign data to port LED
				p4 = led_data; 

				led_data = ~led_data; // Inverse for processing 
				
				
				// Shifting
				if (dir == MAX_MIN)
				{
					if (led_data == MIN) //0001 or 1000
					{
						game_state = STOP;
						lose_blinking = 1;
					}
					else
						led_data = led_data >> 1;
					
						
				}
				else 
				{
					if (led_data == MAX) //0001 or 1000
					{
						game_state = STOP;
						lose_blinking = 1;
					}
					else
						led_data = led_data << 1;
				}
					
				led_data = ~led_data; // Inverse back
			}//endif LED_GAME
			else // LCD_GAME
			{	
				// Check lose
				if (ball_x_val == 7) // Right edge
				{
					if (p1_y != ball_y_val)
					{
						game_state = STOP;
						lose_blinking = 1;	
						return ;
					}		
				}
				else if  (lcd_game_mode == TWO_PLAYER && ball_x_val == 0) // Left edge in TWO-PLAYER mode
				{
					if (p2_y != ball_y_val)
					{
						game_state = STOP;
						lose_blinking = 1;
						return ;
					}
				}
			/* GAME NOT END */
				/* HIT BACKS */
				if (ball_x_val == 0)
				{
					ball_x_val = 1;
					dir = MIN_MAX;	
				}
				else if (ball_x_val == 7)
				{
					ball_x_val = 6;	
					dir = MAX_MIN;
				}
				
								
				// Update new position
				ball_x = ball_x_val;
				ball_y = ball_y_val;		
								
				
				// Update display
				lcd_game_display(lcd_game_mode, p1_y, p2_y, ball_x, ball_y);	
				
				
				
				// Prepare new position
				if (dir == MIN_MAX)
				{
					++ball_x_val;
				}
				else
				{
					--ball_x_val;
					/* ONE-PLAYER case to bounce back automatically */
					if (lcd_game_mode == ONE_PLAYER && ball_x_val == 0)
						dir = MIN_MAX;
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



void main(void)				
{
	InitialiseDisplay();
	if (game_mode == LCD_GAME)
	{
		lcd_game_init(&dir, &p1_y, &p2_y, &ball_x, &ball_y, &ball_x_val, &ball_y_val);
		lcd_game_display(lcd_game_mode, p1_y, p2_y, ball_x, ball_y);
	}
	else
	{
		led_game_init(&dir, &led_data);
		p4 = led_data; 	
	}
		
	
	/* Configure sw3 as input */
	pd8_3 = 0;
	
	/* Configure timer a0 */
	ta0mr = 0x40; // f8
	ta0 = 29999; // 10ms
	
	/* Configure timer a1 */
	ta1mr = 0x80; // f32
	ta1 = 37499; // 50ms
	
	/* Configure timer a2 */
	ta2mr = 0x86; // f32 - rising edge as input signal - pulse is output
	ta2 = (unsigned int)((f1_CLK_SPEED/32)*0.01)-1; // 10ms pulse
	
	/* Configure priority level */
	DISABLE_IRQ			
	ta2ic = 0x02; // One-shot timer
	ta1ic = 0x01; // Display timer
	ta0ic = 0x02; // Game timer
	int0ic = 0x06; // sw0
	int1ic = 0x06; // sw1
	int2ic = 0x06; 
	ENABLE_IRQ
	
	/* Start timers */
	ta0s = 1;
	ta1s = 1;
	ta0s = 1;
	
	while(1);
}         
