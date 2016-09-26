/***********************************************************************************
FILE NAME  	:	main_event_cntr.c 
DESCRIPTION :	Main routine for Timer - Event Counter Mode
               	Timer TA2 to event counter mode. An interrupt is generated after it counts
				the number of events specified by ta2 (i.e. 100).  LED2 will blink each time
				the	interrupt service routine is executed    
			 	The event that ta2 is counting is underflows on ta3.  The ta3 waveform can be 
				observed on P7_6 which is pin JA5-5 on the RSKM16C62P board

Copyright   : 2005 Renesas Technology Europe Ltd.
Copyright   : 2005 Renesas Technology Corporation.
All Rights Reserved

***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
11.08.2005 RTA-MGF First Release
***********************************************************************************/

/**********************************************************************************
System Includes
***********************************************************************************/

/**********************************************************************************
User Includes
***********************************************************************************/
/* sfr62p.h provides common defines for widely used items. */
#include "sfr62p.h"
/* rskM16C62p_def.h defines some common definitions */
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "lcd.h"
/**********************************************************************************
Global variables
***********************************************************************************/

/**********************************************************************************
Prototype declarations
***********************************************************************************/
void normal_display(void);
void floating_point_display(void);
/**********************************************************************************
User Program Code
***********************************************************************************/

/**********************************************************************************
Function Name: 	Main
Description:	Initializes variables and LED port and then waits for
            	the TA2 interrupts.   
Parameters: 	none
Return value: 	none
***********************************************************************************/
void main (void)
{
	/* Initlaise the display*/
	InitialiseDisplay();	
	
	
	/* Input pin init*/
	pd7_5 = 0; //ta2in
				
			
	/*  Initialise the timers */
	timer_init();
  	while (1);
}
/**********************************************************************************
End of function main
***********************************************************************************/   

/**********************************************************************************
Function Name: 	timer_init
Description:	Initializes Timer TA2 to event counter mode. An interrupt is
				generated after it has counted the number of events specified on 
				ta2 (i.e. 100).  In this module TA2 is counting underflows on TA3
				This init also starts TA3 in timer mode with a 5 mSec period  
Parameters: 	none
Return value: 	none
***********************************************************************************/
void timer_init()
{
	/*ta2 mode register */
	ta2mr = 0x41; // free run type, falling edge 
	/*ta1 mode register */
	ta1mr = 0x40; //f8 
	
	ta1 = 29999;  // 10ms
	ta2 = 65535;
	
	
	/* Connect ta2in to input pin */
	//ta2in = p7_5; Not necessary
	
    DISABLE_IRQ			
	ta1ic = 0x01; // Timer a1 interrupt
   	ta2ic = 0x03; // Timer a2 interrupt
    ENABLE_IRQ			

	

	/* start timers */
	ta1s = 1;
   	//ta2s = 1;
	tabsr |= (2 << 1);
}
/**********************************************************************************
End of function timer_init
***********************************************************************************/   

/**********************************************************************************
Function Name: 	ta2_irq
Description:	Timer A2 Interrupt Service Routine. pulse_count
Parameters: 	none
Return value: 	none
***********************************************************************************/
void ta2_irq(void)
{ 
	p4 = 0;
} 
/**********************************************************************************
End of function ta2_irq
***********************************************************************************/   


/**********************************************************************************
Function Name: 	ta1_irq
Description:	Timer A1 Interrupt Service Routine. Display count value on LCD
				display
Parameters: 	none
Return value: 	none
***********************************************************************************/
int ta1_user_counter = 0;
void ta1_irq(void)
{
	if (++ta1_user_counter == 50)
	{
		ta1_user_counter = 0;
		normal_display(); // Count - Line1
		floating_point_display(); // Frequency - Line2
	}
}
/**********************************************************************************
End of function ta2_irq
***********************************************************************************/   

void normal_display(void)
{
	/* Normal display */
	long pulse_count = 65535 - ta2;
	
	
	char line2[8];		
	int cnt;
	for (cnt = 0; cnt < 7; cnt++)
	{
		int i;
		long base10 = 10;
		for (i = 0; i < (6 - cnt); i++)
			base10 *= 10;
		
		line2[cnt] = pulse_count/base10%10 + 48;
	}
	
	line2[7] = pulse_count%10 + 48;
	DisplayString(LCD_LINE1, line2);

}

long old_val = 0;
int floating_point_timer = 0;
void floating_point_display(void)
{
	int real_part = 5;
	int i,j;
	int sec = 2 * 2; // 2 because 500ms trigger, 5 for 5 second 
	double ffreq;
	long ifreq;
	char line2[8];
	long pulse_count = 65535 - ta2;
	
	/* Floating point */
	if (++floating_point_timer == sec) // 1 second
	{
		floating_point_timer = 0;
		
		// Calculate change in pulses, then convert to freq
		if (old_val <= pulse_count)
			ffreq = (double)(pulse_count - old_val)/sec*2 ;
		else
			ffreq = (double)(pulse_count + 65535-old_val)/sec*2;
		
		old_val = pulse_count;		
		
		if (ffreq < 100)
			real_part = 2;
		else if (ffreq < 1000)
			real_part = 3;
		else if (ffreq < 10000)
			real_part = 4;
		else if (ffreq < 100000)
			real_part = 5;
		
		// 123.4567 -> 1234567
		for (i = 0; i < 8 - real_part; i++)
			ffreq *= (double)10;
		
		if (ffreq - (long)ffreq > 0)
			ffreq += 1;	
		
		ifreq = (long)ffreq;
		
		// Display integer part
		for (i = 0; i < 8; i++)
		{
			long divider = 1;
			int j;
		
			if (i == real_part)
			{
				line2[real_part] = '.';
				continue;
			}	
		
			for (j = 0; j < 7 - i; j++)
				divider *= 10;
			if (i > real_part)
				divider *= 10;
			
			line2[i] = ifreq/divider % 10 + 48;
		}
		//line2[7] = ifreq % 10 + 48;
		
		DisplayString(LCD_LINE2, line2);
	}	
}
