#include "time_set.h"
#include "lcd.h"

char line1[8];

void LCD_display_time_setting(enum time_setting pchoice)
{
	// Display time setting operation (blinking, ...)	
	switch(pchoice)
	{
		case SET_HOUR:
		{
			hour_adjust();
			break;
		}	
		case SET_MINUTE:
		{
			minute_adjust();
			break;	
		}
		case SET_SECOND:
		{
			second_adjust();
			break;	
		}
		case SET_YEAR:
		{
			year_adjust();
			break;	
		}
		case SET_MONTH:
		{
			month_adjust();
			break;	
		}
		case SET_DAY:
		{
			day_adjust();
			break;	
		}
	}
}

void hour_adjust(void)
{
    DisplayString(LCD_LINE1, "+-HOUR ");
    line1[0] = ' ';
    line1[1] = ' ';
    line1[2] = ' ';
	line1[3] = hour/10 + 48;
    line1[4] = hour%10 + 48;
    line1[5] = ' ';
    line1[6] = ' ';
    line1[7] = ' ';
    DisplayString(LCD_LINE2, line1);
}

void minute_adjust(void)
{
    DisplayString(LCD_LINE1, "+-MINUTE");
    line1[0] = ' ';
    line1[1] = ' ';
	line1[2] = ' ';
    line1[3] = minute/10 + 48;
    line1[4] = minute%10 + 48;
    line1[5] = ' ';
    line1[6] = ' ';
    line1[7] = ' ';
    DisplayString(LCD_LINE2, line1);
}

void second_adjust(void)
{
    DisplayString(LCD_LINE1, "+-SECOND ");
    line1[0] = ' ';
    line1[1] = ' ';
	line1[2] = ' ';
    line1[3] = second/10 + 48;
    line1[4] = second%10 + 48;
    line1[5] = ' ';
    line1[6] = ' ';
    line1[7] = ' ';
    DisplayString(LCD_LINE2, line1);
}

void year_adjust(void)
{
    DisplayString(LCD_LINE1, "+-YEAR  ");
    line1[0] = ' ';
    line1[1] = ' ';
	line1[2] = ' ';
    line1[3] = year / 1000 + 48;
    line1[4] = year / 100 % 10 + 48;
    line1[5] = year / 10 % 10 + 48;
    line1[6] = year % 10 + 48;
    line1[7] = ' ';
    DisplayString(LCD_LINE2, line1);
}

void month_adjust(void)
{
    DisplayString(LCD_LINE1, "+-MONTH ");
    line1[0] = ' ';
    line1[1] = ' ';
    line1[2] = months_of_year[month][0];
    line1[3] = months_of_year[month][1];
    line1[4] = months_of_year[month][2];
    line1[5] = ' ';
    line1[6] = ' ';
    line1[7] = ' ';
    DisplayString(LCD_LINE2, line1);
}

void day_adjust(void)
{
    DisplayString(LCD_LINE1, "+-DAY   ");
    line1[0] = ' ';
    line1[1] = ' ';
    line1[2] = day/10 + 48;
    line1[3] = day%10 + 48;
    line1[4] = ' ';
    line1[5] = ' ';
    line1[6] = ' ';
    line1[7] = ' ';
    DisplayString(LCD_LINE2, line1);
}
