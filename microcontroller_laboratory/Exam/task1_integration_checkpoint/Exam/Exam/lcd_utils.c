#include "lcd_utils.h"


void superMenu_lcdDisplay(void)
{
	DisplayString(LCD_LINE1, "TOP_MENU");
	switch (super_menu_choice)
	{
		case TASK1:
		{
			DisplayString(LCD_LINE2, "1.GAME  ");
			break;	
		}
		case TASK2:
		{
			superMenu_scrollDisplay();
			break;	
		}
		case TASK3:
		{
			DisplayString(LCD_LINE2, "3.ADCDAC");
			break;	
		}
		case TASK4:
		{
			DisplayString(LCD_LINE2, "4.EMPTY ");
			break;	
		}
		case TASK5:
		{
			DisplayString(LCD_LINE2, "5.EMPTY ");
			break;	
		}
	}
}

int first_char_left = 0;  //First character in first lcd position
void superMenu_scrollDisplay(void)
{
	int cnt; // counter
	int char_pos; // current position in string
	int string_length = 20;
	char string[20] = "2.Function Generator";
	
	/***********/
	/* Rolling */
	/***********/

	/* Move pointer to line 2 */ 
	LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L2) );

	char_pos = first_char_left; // First char to print 

	// Loop through string, print out
	for (cnt = 0; cnt < 8; cnt++)
	{
		LCD_write(DATA_WR, string[char_pos]);
		if (++char_pos == string_length)
			char_pos = 0;
	}

	// shift character, new pattern
	if (++first_char_left == string_length)
		first_char_left = 0;
}


/************************************************/
/*********************TASK1**********************/
/************************************************/

void task1_LEDGameInit(unsigned char *dir, unsigned char *led_data)
{
	*dir = MIN_MAX;
	*led_data = 0xFE;
}

void task1_LCDGameInit(unsigned char *dir, int *p1_y, int *p2_y, int *ball_x, int *ball_y, int *ball_x_val, int *ball_y_val)
{
	*dir = MAX_MIN;
	*p1_y = 0;
	*p2_y = 0;
	*ball_x_val = 6;
	*ball_y_val= 0;	
	*ball_x = 6;
	*ball_y = 0;
}

int task1_tikTok = 1;
void task1_gameOver(void)
{
	
	DisplayString(LCD_LINE2, "        ");	
	
	if (task1_tikTok)
	{
		DisplayString(LCD_LINE1, "GAMEOVER");	
	}
	else
	{
		DisplayString(LCD_LINE1, "        ");	
	}
	task1_tikTok = !task1_tikTok;
}


void task1_LCDGameDisplay(enum task1_lcd_game_modes lcd_game_mode, int p1_y, int p2_y, int ball_x, int ball_y);	
void task1_LCDGameDisplay(enum task1_lcd_game_modes lcd_game_mode, int p1_y, int p2_y, int ball_x, int ball_y)
{
	char line1[8];
	char line2[8];
	int i;
	
		
	/* Position of player 1 */
	if (p1_y == 0)
	{
		line1[7] = ']';	
		line2[7] = ' ';
	}
	else
	{
		line1[7] = ' ';	
		line2[7] = ']';
	}
	
	/* Position of player 2 */
	if (lcd_game_mode == TWO_PLAYER)
	{
		if (p2_y == 0)
		{
			line1[0] = '[';	
			line2[0] = ' ';
		}
		else
		{
			line1[0] = ' ';	
			line2[0] = '[';
		}
	}
	
	
	/* Position of ball */
	for (i = 0; i < 7; i++)
	{
		if (lcd_game_mode == TWO_PLAYER && i == 0)
			continue;
		
		if (ball_x == i)
		{
			if (ball_y == 0)
			{
				line1[i] = 'o';
				line2[i] = ' ';
			}
			else
			{
				line1[i] = ' ';
				line2[i] = 'o';
			}
		}
		else
		{
			line1[i] = ' ';
			line2[i] = ' ';	
		}
	}
	
	DisplayString(LCD_LINE1, line1);
	DisplayString(LCD_LINE2, line2);
}