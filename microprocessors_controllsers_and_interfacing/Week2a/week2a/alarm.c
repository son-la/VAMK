#include "alarm.h"
#include "main.h"
#include "lcd.h"

unsigned char tik_tak = 0;

void LCD_display_alarm( int alarm_mode, int alarm_hour, int alarm_min )
{

	switch( alarm_mode ){
		case 0:	//SET HOUR
		{
			LCD_write(CTRL_WR,LCD_CLEAR);
			DisplayString( LCD_LINE1, "SET HH:");
		
			LCD_display_alarm_clock(alarm_hour, alarm_min);
		
			break;
		}
		case 1: // SET MINUTE
		{
			LCD_write(CTRL_WR,LCD_CLEAR);
			DisplayString( LCD_LINE1, "SET MM:");
		
			LCD_display_alarm_clock(alarm_hour, alarm_min);
			
			break;
		}
		case 2: // CONFIRMATION
		{			
			LCD_write(CTRL_WR,LCD_CLEAR);
			DisplayString( LCD_LINE1, "ALARM:");
			
			LCD_display_alarm_clock(alarm_hour, alarm_min);
			break;
		}
		default:
			break;
	}
	// Set hour, minute, and alarm_mode = 1 when alarm is set	
}

void LCD_display_alarm_clock( int alarm_hour, int alarm_min )
{
	char alarm_clock[8];
	
	// Show hours
	alarm_clock[0]	= alarm_hour/10 + 48;
	alarm_clock[1]	= alarm_hour%10 + 48;
	
	// Show minutes
	alarm_clock[3]	= alarm_min/10 + 48;
	alarm_clock[4]	= alarm_min%10 + 48;
	
	// Show :
	if (tik_tak)
	{
		alarm_clock[2] = ':';
	}
	else
	{
		alarm_clock[2] = ' ';
	}
	
	alarm_clock[5] = ' ';
	alarm_clock[6] = ' ';
	alarm_clock[7] = ' ';
	
	tik_tak = !tik_tak;	
	
	DisplayString(LCD_LINE2, alarm_clock);
	
}

int check_alarm(int alarm_hour, int alarm_minute, unsigned int clock_hour, unsigned int clock_minute )
{
	if( (clock_hour == alarm_hour)&&(clock_minute == alarm_minute) )
		return 1;
	return 0;
}
