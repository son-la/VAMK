#include "lcd_utils.h"
#include "lcd.h"




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
	int string_length = 23;
	char string[23];
	string[0] = '2'; string[1] = '.'; string[2] = 'F'; string[3] = 'u'; string[4] = 'n'; 
	string[5] = 'c'; string[6] = 't'; string[7] = 'i'; string[8] = 'o'; string[9] = 'n';
	string[10] = ' '; string[11] = 'G'; string[12] = 'e'; string[13] = 'n'; string[14] = 'e';
	string[15] = 'r'; string[16] = 'a'; string[17] = 't'; string[18] = 'o'; string[19] = 'r';
	string[20] = ' '; string[21] = ' '; string[22] = ' ';
	
		
	
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



/************************************************/
/*********************TASK2**********************/
/************************************************/



void task2_updateDisplay(void)
{
	// Check mode and display
	if (task2_mode == NORMAL)
	{
		// Display voltage
		task2_lcdDisplayFloatingPoint(task2_voltage, 1, 'V', 1);
		// Display percentage
		task2_lcdDisplayFloatingPoint(task2_percent, 3, '%', 2);
	}
	else if (task2_mode == SINE_WAVE || task2_mode == TRIANGLE_WAVE)
	{
		// Display frequency
		if (task2_freq < 1000)
			task2_lcdDisplayFloatingPoint(task2_freq, 3, 'H', 1);
		else
			task2_lcdDisplayFloatingPoint(task2_freq/1000, 2, 'K', 1);
		
		// Display percentage
		task2_lcdDisplayFloatingPoint(task2_percent, 3, '%', 2);
	}
	else if (task2_mode == RECORD)
	{
		// Show percentage record
		task2_lcdDisplayRecording(task2_data_pos, TASK2_RESOLUTION);	
	}
	else if (task2_mode == PLAYBACK)
	{
		// Show percentage playback
		task2_lcdDisplayPlayBack(task2_data_pos, TASK2_RESOLUTION);
	}
}


void task2_lcdDisplayFloatingPoint(double value, int real_part, char unit, int line_num)
{
	int i;
	long round_val;
	char line[8];
	
	for (i = 0; i < 7 - real_part; i++)
		value *= 10;
	
	// Rounding
	if (value - (long)value > 0)
		value += 1;	
	
	round_val = (long)value;
	
	// Get digit
	for (i = 0; i < 7; i++)
	{
		long divider = 1;
		int j;
		
		if (i == real_part)
		{
			line[i] = '.';	
			continue;
		}
		
		for (j = 0; j < 6 - i; j++)
			divider *= 10;
		if (i > real_part)
			divider *= 10;
			
		line[i] = round_val/divider % 10 + 48;
	}
	
	// Append unit
	line[7] = unit;
	
	// Print
	if (line_num == 1)
		DisplayString(LCD_LINE1, line);
	else
		DisplayString(LCD_LINE2, line);
}


void task2_lcdDisplayRecording(int buffer, int resolution)
{
	char line1[8];
	char line2[8];
	double fPercent;
	int iPercent;
	
	line1[0] = 'R';
	line1[1] = 'E';
	line1[2] = 'C';
	line1[3] = 'O';
	line1[4] = 'R';
	line1[5] = 'D';
	line1[6] = ':';
	line1[7] = ' ';
	
	if (buffer < 0)
		iPercent = 0;
	else if (buffer > resolution)
		iPercent = 100;
	else
	{
		fPercent = (double)(buffer)/resolution * 100;
		fPercent += fPercent - (int)fPercent >= 0.5 ? 1:0;
		iPercent = (int)fPercent;
	}
	
	line2[0] = iPercent/100 + 48;
	line2[1] = iPercent/10%10 + 48;
	line2[2] = iPercent%10 + 48;
	line2[3] = '%';
	line2[4] = ' ';
	line2[5] = ' ';
	line2[6] = ' ';
	line2[7] = ' ';
	
	DisplayString(LCD_LINE1, line1);
	DisplayString(LCD_LINE2, line2);
}

void task2_lcdDisplayPlayBack(int buffer, int resolution)
{
	char line1[8];
	char line2[8];
	double fPercent;
	int iPercent;
	
	line1[0] = 'P';
	line1[1] = 'L';
	line1[2] = 'A';
	line1[3] = 'Y';
	line1[4] = ':';
	line1[5] = ' ';
	line1[6] = ' ';
	line1[7] = ' ';
	
	if (buffer < 0)
		iPercent = 0;
	else if (buffer > resolution)
		iPercent = 100;
	else
	{
		fPercent = (double)(buffer)/resolution * 100;
		fPercent += fPercent - (int)fPercent >= 0.5 ? 1:0;
		iPercent = (int)fPercent;
	}
	
	line2[0] = iPercent/100 + 48;
	line2[1] = iPercent/10%10 + 48;
	line2[2] = iPercent%10 + 48;
	line2[3] = '%';
	line2[4] = ' ';
	line2[5] = ' ';
	line2[6] = ' ';
	line2[7] = ' ';
	
	DisplayString(LCD_LINE1, line1);
	DisplayString(LCD_LINE2, line2);
}






/************************************************/
/*********************TASK3**********************/
/************************************************/
int task3_tik = 1;
void task3_LCDDisplayFunction(enum TASK3_MODES task3_mode, double task3_dc_val, int task3_fq_choice)
{
	char line1[8], line2[8];
	long dc;
	
	line1[0] = 'F';
	line1[1] = 'Q';
	
	DisplayString(LCD_LINE1, "        ");
	DisplayString(LCD_LINE2, "        ");
	
	if (task3_fq_choice == 0)
	{
		line1[2] = '1';
		line1[3] = '0';	
		line1[4] = '-';
		line1[5] = '1';
		line1[6] = '0';
		line1[7] = '0';
	}
	else if (task3_fq_choice == 1)
	{
		line1[2] = '1';
		line1[3] = '0';	
		line1[4] = '0';
		line1[5] = '-';
		line1[6] = '1';
		line1[7] = 'K';
	}
	else
	{
		line1[2] = '1';
		line1[3] = 'K';	
		line1[4] = '-';
		line1[5] = '1';
		line1[6] = '0';
		line1[7] = 'K';
	}
	
	task3_dc_val *= 10;
	task3_dc_val += (task3_dc_val - (long)task3_dc_val >= 0.5) ? 1:0;
	dc = (long)task3_dc_val;
	
	line2[0] = 'D';
	line2[1] = 'C';
	line2[2] = dc/1000 + 48;
	line2[3] = dc/100%10+48;
	line2[4] = dc/10%10+48;
	line2[5] = '.';
	line2[6] = dc%10 + 48;
	line2[7] = '%';
	
	switch (task3_mode)
	{
		case TASK3_NORMAL:
		{
			DisplayString(LCD_LINE1, line1);
			DisplayString(LCD_LINE2, line2);
			break;			
		}
		case TASK3_SET_FQ:
		{
			if (task3_tik == 1)
				DisplayString(LCD_LINE1, "        ");
			else
				DisplayString(LCD_LINE1, line1);
				
			DisplayString(LCD_LINE2, line2);
			break;
		}
		case TASK3_SET_DC:
		{
			DisplayString(LCD_LINE1, line1);
			if (task3_tik == 1)
				DisplayString(LCD_LINE2, "        ");
			else
				DisplayString(LCD_LINE2, line2);
			break;			
		}
	}
	task3_tik = -task3_tik;
}

/************************************************/
/*********************EMPTY**********************/
/************************************************/


void emptyProjectDisplay(void)
{
	DisplayString(LCD_LINE1,"EMPTY   ");	
	DisplayString(LCD_LINE2,"PROJECT ");	
	
}