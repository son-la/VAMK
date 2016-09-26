#include "calendar.h"
#include "lcd.h"
#include "main.h"

unsigned char tik_tok = 0;

void update_calendar(void)
{
	//Manually update day of week for performance reason
	if (++day_of_week == 7) day_of_week = 0; // Saturday - Sunday case
	
	
	
	// Update day, month, year				
	// Next month
	if (
		(  ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10)) && (day == 31)  ) ||
		(  ((month == 4) ||	(month == 6) ||	(month == 9) || (month == 11)) && (day == 30)  ) ||
		(  (month == 2) && (day == 29)  ) ||
		(  (month == 2) && (day == 28) && (year % 4 != 0)  )
	)
	{
		day = 1;				
		month++;
		return;	
	}
	
	// Next year
	if ((month == 12) && (day == 31))
	{
		day = 1;
		month = 1;
		
		if (++year == MAX_YEAR) 
		{
			year = MIN_YEAR;
			day_of_week = DAY_OF_WEEK0;	
		}
			
		return;
	}
	
	// Next day
	day++;
}

int string_pos = 0;
void LCD_display_calendar(unsigned int pyear,unsigned int pmonth,unsigned int pday, unsigned int pday_of_week, unsigned int phour,unsigned int pminute,unsigned int psecond)
{
	int i;
	int char_pos = string_pos;
	char line1[8];
	char line2[8];
	char string[CAL_STRING_LENGTH];
	char temp[3];
	
	/* First line */
	line1[0] = phour / 10 + 48;
	line1[1] = phour % 10 + 48;
	
	line1[3] = pminute / 10 + 48;
	line1[4] = pminute % 10 + 48;
	
	line1[6] = psecond / 10 + 48;
	line1[7] = psecond % 10 + 48;
	
	if (tik_tok)
	{
		line1[2] = ':';
		line1[5] = ':';
	}
	else
	{
		line1[2] = ' ';
		line1[5] = ' ';
	}
	tik_tok = !tik_tok;	
	
	DisplayString(LCD_LINE1, line1);
	
	
	/* Second line */
	// THU, JAN 01, 2015
	
	
	// Day of week
	string[0] = days_of_week[pday_of_week][0];
	string[1] = days_of_week[pday_of_week][1];
	string[2] = days_of_week[pday_of_week][2];
	 
	string[3] = ',';
	string[4] = ' ';
	 
	// Month of year
	string[5] = months_of_year[pmonth][0];
	string[6] = months_of_year[pmonth][1];
	string[7] = months_of_year[pmonth][2];
	 
	string[8] = ' ';
	 
	// Day of month
	string[9] = pday / 10 + 48;
	string[10] = pday % 10 + 48;
	 
	
	string[11] = ',';
	string[12] = ' ';
	
	// Year
	string[13] = pyear / 1000 + 48;
	string[14] = pyear / 100 % 10 + 48;
	string[15] = pyear / 10 % 10 + 48;
	string[16] = pyear % 10 + 48;
	
	string[17] = ' ';
	string[18] = ' ';
	
	LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L2));
		
	for (i = 0; i < 8; i++)
	{
		LCD_write(DATA_WR, string[char_pos]);
		if (++char_pos == CAL_STRING_LENGTH)
			char_pos = 0;
	}
	if (++string_pos == CAL_STRING_LENGTH)
		string_pos = 0;
}

/*
	Sunday: 0
	Monday: 1
	... 
	Saturday: 6

*/
unsigned int get_day_of_week(unsigned int pyear, unsigned int pmonth, unsigned int pday)
{
	// 1-1-2015 = 4 - Thursday
	unsigned int total_days = 0;
	int i;
	for (i = MIN_YEAR; i < pyear; i++)
		if (i % 4 == 0)
			total_days += 366;
		else
			total_days += 365;
			
	for (i = 1; i <= pmonth; i++)
	{
		if ((i == 1) || (i == 3) || (i == 5) || (i == 7) || (i == 8) || (i == 10) || (i == 12))
			total_days += 31;
		else if ((i == 4) || (i == 6) || (i == 9) || (i == 11))
			total_days += 30;
		else
		{
			if (pyear % 4 == 0)
				total_days += 29;
			else
				total_days += 28;	
		}
	}
	
	total_days += pday - 1;
	
	
	total_days %= 7;
	return (DAY_OF_WEEK0 + total_days) % 7;
}