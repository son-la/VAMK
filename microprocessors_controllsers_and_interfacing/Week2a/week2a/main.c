/***********************************************************************************
FILE NAME  	:	main.c
DESCRIPTION :	Empty proejct set up for RSKM16C62P					


Copyright   : 2005 Renesas Technology Europe Ltd.
Copyright   : 2005 Renesas Technology Corporation.
All Rights Reserved

***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
21.01.2006 RTA-MGF First Release
***********************************************************************************/

/**********************************************************************************
System Includes
***********************************************************************************/

/**********************************************************************************
User Includes
***********************************************************************************/
#include "sfr62p.h"
/* rskM16C62p_def.h defines some common definitions */
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "lcd.h"
#include "alarm.h"
#include "time_set.h"
#include "calendar.h"
#include "set_operation.h"


#pragma INTERRUPT sw1 // int0
#pragma INTERRUPT sw2 // int1
#pragma INTERRUPT sw3 // int2

#pragma INTERRUPT timer_a0_interrupt // Timer a0
#pragma INTERRUPT timer_a1_interrupt // Timer a1
#pragma INTERRUPT timer_a2_interrupt // Timer a2

// Set default
enum operation_modes mode = CALENDAR; // default calendar mode
unsigned int year = 2015; 
unsigned int month = 9;
unsigned int day = 25;
unsigned int day_of_week = 5;

unsigned int hour = 0;
unsigned int minute = 0;
unsigned int second = 0;
unsigned int msecond = 0;
unsigned int msec = 0;

char days_of_week[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
char months_of_year[12][4] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};

int alarm_min 	= 0; // SET ALARM MINUTE
int alarm_hour = 0; // SET ALARM HOUR
int check_alarm_trigger = 0;

int set_mode_choice = 0; // 0 back - calendar, 1 set time, 2 alarm
enum time_setting set_time_choice = SET_HOUR;
int alarm_mode	= 0; // 0 - SET HOUR, 1 - SET MINUTE, 2 - BACK

int sw_press = 1;

void sw1_func(void)
{
switch (mode)
	{
		case CALENDAR: // calendar mode -> mode_choosing mode
		{
			// Open Modes choosing
			set_mode_choice = CALENDAR;
			mode = MODE_CHOOSING;		
			break;	
		}
		case MODE_CHOOSING: // mode_choosing mode -> chosen mode
		{
			mode = set_mode_choice;
			break;	
		}
		case ALARM: // During alarm mode 
		{
			// Check alarm mode procedure	
			switch(alarm_mode){
				case 0: // SET HOUR -> SET MINUTE
				{	
					++alarm_mode;
					break;
				}
				case 1: // SET MINUTE -> CALENDAR
				{
					++alarm_mode ;
					//mode = CALENDAR;
					break;	
				}
				case 2: // CONFIRM ALARM SET
				{
					alarm_mode = 0;
					mode = CALENDAR;
					check_alarm_trigger = 1;
					break;
				}
				default:
					break;
			}	
			break;		
		}
		case SET_TIME: // During time setting mode
		{
			// Check time setting procedure
			if (set_time_choice == SET_DAY)
			{
				set_time_choice = SET_HOUR;
				set_mode_choice = 0;
				mode = CALENDAR;
			}
			p4 = 0;
			set_time_choice++;
			break;
		}
	}	
}

void sw2_func(void)
{
	switch (mode)
	{
		case MODE_CHOOSING:
		{
			if (++set_mode_choice == 3)
				set_mode_choice = 0;
			break;	
		}
		case SET_TIME:
        {
            switch (set_time_choice)
            {
                case SET_HOUR:
                {                    
                    if(++hour == 24)
                    {
                        hour = 0;
                    }
                    break;
                }
                case SET_MINUTE:
                {
                    if(++minute == 60)
                    {
                        minute = 0;
                    }
                    break;
                }
                case SET_SECOND:
                {
                    if(++second == 60)
                    {
                        second = 0;
                    }
                    break;
                }
				case SET_YEAR:
                {
                    if(++year == MAX_YEAR)
                    {
                        year = MIN_YEAR;
                    }
                    break;
                }
                case SET_MONTH:
                {
                    if(++month == 12 )
                    {
                        month = 0;
                    }
                    break;
                }
                case SET_DAY:
                {
                    if (
                        (  ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12)) && (day == 31)   ) ||
                        (  ((month == 4) ||	(month == 6) ||	(month == 9) || (month == 11)) && (day == 30)  ) ||
                        (  (month == 2) && (day == 29)  ) ||
                        (  (month == 2) && (day == 28) && (year % 4 != 0)  )
                        )
						
                    {
                        day = 1;
                        return;	
                    }
					else
					{
                        ++day;
                        return;	
                    }
                    break;
                }
                    
            }
        }	
		case ALARM:
		{
			switch( alarm_mode ){
				case 0: // SET HOUR
				{
					if( ++alarm_hour == 24 )
						alarm_hour = 0;
					break;	
				}
				case 1: // SET MINUTE
				{
					if( ++alarm_min == 60 )
						alarm_min = 0;
					break;	
				}
			}
			break;
		}	
		///// Other cases
	}
}

void sw3_func(void)
{
	switch (mode)
	{
		case MODE_CHOOSING:
		{
			if (--set_mode_choice == -1)
				set_mode_choice = 2;
			break;	
		}	
		case SET_TIME:
        {
            switch (set_time_choice)
            {
                case SET_HOUR:
                {                   
                    if(hour == 0)
                        hour = 23;
                    else
						--hour;
                    break;
                }
                case SET_MINUTE:
                {
                    if(minute == 0)
                        minute = 59;
					else
						--minute;
						
					break;
                }
                case SET_SECOND:
                {
                    if(second == 0)
                        second = 59;
					else
						--second;
					break;
                }
				case SET_YEAR:
                {
                    if(--year == (MIN_YEAR - 1))
                    {
                        year = MAX_YEAR;
                    }
                    break;
                }
                case SET_MONTH:
                {
                    if(--month == -1 )
                    {
                        month = 11;
                    }
                    break;
                }
                case SET_DAY:
                {
                    if(--day == 0)
                    {
                        if((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12))
                        {
                            day = 31;
                            break;
                        }
                        else if ((month == 4) ||	(month == 6) ||	(month == 9) || (month == 11))
                        {
                            day = 30;
                            break;
                        }
                        else if((month == 2) && (year % 4 == 0))
                        {
                            day = 29;
                            break;
                        }
                        else if((month == 2) && (year % 4 != 0))
                        {
                            day = 28;
                            break;
                        }
                    }
				}
			 
            }
        }
		case ALARM:
		{
			switch( alarm_mode ){
				case 0: // SET HOUR
				{
					if( --alarm_hour == -1 )
						alarm_hour = 23;
					break;	
				}
				case 1: // SET MINUTE
				{
					if( --alarm_min == -1 )
						alarm_min = 59;
					break;	
				}
			}
		}
		///// Other cases
	}
}

void sw1(void)
{
	if (sw_press == 1)
	{
		sw_press = 0;
		sw1_func();
		ta2os = 1;
	}
	
}

void sw2(void)
{
	if (sw_press == 1)
	{
		sw_press = 0;
		sw2_func();
		ta2os = 1;
	}
}

void sw3(void)
{
	if (sw_press == 1)
	{
		sw_press = 0;
		sw3_func();
		ta2os = 1;
	}
}

int alarm_trigger = 0; 	// Triger = 1 if led on, otherwise trigger  = 0
int led_on = 0;			// LED time

void reset_alarm( void )
{
	check_alarm_trigger = 0;
	alarm_trigger = 0;
}

void timer_a0_interrupt(void) // Main clock
{
	
	
	if (mode == CALENDAR || mode == ALARM || mode == MODE_CHOOSING) // While setting alarm and calendar (normal) modes -> clock is still working
	{
		if (++ msecond== 100){
			msecond = 0;
			// Check alarm
			if( check_alarm_trigger && alarm_trigger == 0)
			{
				alarm_trigger = check_alarm(alarm_hour, alarm_min, hour, minute);
			}
			
			// If alarm on, then set LED on for 2 minute (120sec)
			if ( alarm_trigger == 1 )
			{
				led_on = 10;
				alarm_trigger = 0;
				check_alarm_trigger = 0;
			}	
			// If led_on > 0, turn on LED, otherwise turn it off
			if( --led_on > 0 )
				p4 = 0x00;
			else
				p4 = 0xff;
						
			if (++second == 60) 
			{					
				second = 0;
				if (++minute == 60)
				{
					minute = 0;

					if (++hour == 24)
					{
						hour = 0;
						
						// Update calendar
						update_calendar();
					}
				}
			}
		}
	}
}

void timer_a1_interrupt(void) // Display function
{
	if(++msec == 100){
		
		switch (mode)
		{
			case CALENDAR: 
			{
				LCD_display_calendar(year, month, day, day_of_week, hour, minute, second);	
				break;	
			}
			case MODE_CHOOSING:
			{
				LCD_display_show_options(set_mode_choice);	
				break;
			}
			case SET_TIME:
			{
				LCD_display_time_setting(set_time_choice);
				break;
			}
			case ALARM:
			{
				LCD_display_alarm(alarm_mode,alarm_hour,alarm_min);	
				break;
			}
		}
		
		msec = 0;
	}
}

// One-shot
int ta2_user_cnt = 0;
void timer_a2_interrupt(void)
{
  	if (++ta2_user_cnt == 3)
	{
   		sw_press = 1;
		ta2_user_cnt = 0;
		return;
	}
	ta2os = 1; // renew timer
	
}

/**********************************************************************************
Function Name: 	Main
Description:	Main function
Parameters: 	none
Return value: 	none
***********************************************************************************/
void main(void)				
{
	InitialiseDisplay();
	
	
	ta0mr = 0x40; //f8
	ta1mr = 0x40; //f8
	
	ta0 = 29999; 
	ta1 = 29999; 
	
	// One shot
	ta2 = (unsigned int)((f1_CLK_SPEED/32)*0.05)-1; // 50ms pulse
	ta2mr = 0x86 ;
	ta2ic = 0x03;
	

	asm("FSET I");
	
	ta0ic = 0x01; // Set priority
	ta1ic = 0x01; // Set priority
	
	int0ic = 0x02; // Set priority
	int1ic = 0x02; // Set priority
	int2ic = 0x02; // Set priority
	
	tabsr |= (1 << 1); // Start timer a1
	tabsr |= (1 << 0); // Start timer a0
	ta2s = 1;  // Start timer a2
	
	while(1){}	
}         
/**********************************************************************************
End of function main
***********************************************************************************/   

