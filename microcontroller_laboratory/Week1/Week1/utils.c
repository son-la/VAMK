#include "utils.h"
#include "lcd.h"

void led_game_init(unsigned char *dir, unsigned char *led_data)
{
	*dir = MIN_MAX;
	*led_data = 0xFE;
}

void lcd_game_init(unsigned char *dir, int *p1_y, int *p2_y, int *ball_x, int *ball_y, int *ball_x_val, int *ball_y_val)
{
	*dir = MAX_MIN;
	*p1_y = 0;
	*p2_y = 0;
	*ball_x_val = 6;
	*ball_y_val= 0;	
	*ball_x = 6;
	*ball_y = 0;
}

int tik_tok = 1;
void gameover(void)
{
	
	DisplayString(LCD_LINE2, "        ");	
	
	if (tik_tok)
	{
		DisplayString(LCD_LINE1, "GAMEOVER");	
	}
	else
	{
		DisplayString(LCD_LINE1, "        ");	
	}
	tik_tok = !tik_tok;
}

void lcd_game_display(enum lcd_game_modes lcd_game_mode, int p1_y, int p2_y, int ball_x, int ball_y)
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