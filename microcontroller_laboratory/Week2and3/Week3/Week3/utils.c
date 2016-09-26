#include "utils.h"
#include "lcd.h"

int tik = 1;
void lcd_display_function(enum MODES mode, double dc_val, int fq_choice)
{
	char line1[8], line2[8];
	long dc;
	
	line1[0] = 'F';
	line1[1] = 'Q';
	
	
	if (fq_choice == 0)
	{
		line1[2] = '1';
		line1[3] = '0';	
		line1[4] = '-';
		line1[5] = '1';
		line1[6] = '0';
		line1[7] = '0';
	}
	else if (fq_choice == 1)
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
	
	dc_val *= 1000;
	dc_val += (dc_val - (long)dc_val >= 0.5) ? 1:0;
	dc = (long)dc_val;
	
	line2[0] = 'D';
	line2[1] = 'C';
	line2[2] = dc/1000 + 48;
	line2[3] = dc/100%10+48;
	line2[4] = dc/10%10+48;
	line2[5] = '.';
	line2[6] = dc%10 + 48;
	line2[7] = '%';
	
	switch (mode)
	{
		case NORMAL:
		{
			DisplayString(LCD_LINE1, line1);
			DisplayString(LCD_LINE1, line2);
			break;			
		}
		case SET_FQ:
		{
			if (tik)
				DisplayString(LCD_LINE1, line1);
			else
				DisplayString(LCD_LINE1, "        ");
				
			DisplayString(LCD_LINE1, line2);
			break;
		}
		case SET_DC:
		{
			DisplayString(LCD_LINE1, line1);
			if (tik)
				DisplayString(LCD_LINE2, line2);
			else
				DisplayString(LCD_LINE2, "        ");
			break;			
		}
	}
	tik = -tik;
}
	