/***********************************************************************************
FILE NAME  	:	main_oneshot.c 
DESCRIPTION :	Initializes Timer TA2 in one-shot mode
				The timer is started using software start and LED2 is turned on
				LED2 will be turned of when the one-shot expires in 50 milliseconds
			    JA3-39.

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
User Defines
***********************************************************************************/

/**********************************************************************************
Global variables
***********************************************************************************/

enum light_mode mode = MODE0;

/**********************************************************************************
Prototype declarations
***********************************************************************************/

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
	
	// Configure timer a1 - Update display
	ta1mr = 0x40; //f8 
	ta1 = 29999;  // 10ms
	ta1ic = 0x01; // Set priority
	
	// COnfigure timer a3 - Count LED on time
	ta3mr = 0x40;
	ta3 = 29999;
	ta3ic = 0x01;
	
	// Configure two buttons
	int0ic = 0x02; // Set priority
	int1ic = 0x02; // Set priority
	
	/* Initialise the timers a0 */
	timer_init();

	/* start one-shot bit */
	//ta2os = 1;            

	tabsr |= (1 << 1); // Start timer a1
	tabsr |= (1 << 3); // Start timer a3

	/* program is now free running */
	while(1);
}
/**********************************************************************************
End of function main
***********************************************************************************/   

/**********************************************************************************
Function Name: 	tmr_init
Description:	Setup Timer A2 setup for 100ms in one-shot mode.  
Parameters: 	none
Return value: 	none
***********************************************************************************/
 void timer_init(void)
{
	/* Disable interrupts while configuring the registers */
	DISABLE_IRQ;

	/* timer A2 set for 50 mSec pulse, clock is f1/32*/			
	ta2 = (unsigned int)((f1_CLK_SPEED/32)*0.05)-1;

	/* Timer mode register setting */
    ta2mr = 0x86 ;	
		/*  10000110
        b1:b0		TMOD0,TMOD1		ONE-SHOT MODE SELECTED
        b2			MR0		    	PULSE OUTPUT   
        b3			MR1 		   	SELECT TRIGGER EDGE 
        b4			MR2				SELECTED ONE SHOT FLAG AS TRIGGER 
		b5			MR3		        SET TO 0 IN ONE SHOT MODE     
        b7:b6		TCK0,TCK1		F DIVIDED BY 32 SELECTED 
		*/

	/* Timer interrupt priority */
   	ta2ic = 0x03;


	/* enable interrupts macro defined in rskm16c29defs.h */   
    ENABLE_IRQ;

	/* must be set to 1, the the ta2os bit will start count */			
  	ta2s = 1;             
}
/**********************************************************************************
End of function tmr_init
***********************************************************************************/   


/**********************************************************************************
Function Name: 	ta3_irq
Description:	Timer A3 interrupt service routine
Parameters: 	none
Return value: 	none
***********************************************************************************/

int oneshot_count = 0;
int ta3_user_cnt = 0;
void ta3_irq(void)
{
	if (LED2 == LED_ON)
	{
		oneshot_count++;
		if (oneshot_count == 1300) // Avoid overfloat
			oneshot_count = 0;	
	}
}


/**********************************************************************************
Function Name: 	ta2_irq
Description:	Timer A2 interrupt service routine.
Parameters: 	none
Return value: 	none
***********************************************************************************/

int ta2_user_cnt = 0;
void ta2_irq(void)
{
	
  /* turn on Yellow LED */
  	if (++ta2_user_cnt == 20)
	{
   		LED2 = LED_OFF;	
		ta2_user_cnt = 0;	
		return ; // No renew timer
	}
	ta2os = 1; // renew timer
	
}
/**********************************************************************************
End of function ta2_irq
***********************************************************************************/   


/**********************************************************************************
Function Name: 	ta1_irq
Description:	Timer A1 interrupt service routine.
Parameters: 	none
Return value: 	none
***********************************************************************************/
int ta1_user_var = 0;
int counter = 0;
void ta1_irq(void)
{
	int tmp;
	if (++counter == 100)
	{
		char line1[8];
		char line2[8];
	
		// Display selected mode and time elapse
		if (++ta1_user_var == 50) // 500 ms
		{
			ta1_user_var = 0;	
		}
	
	
		line1[0] = 'M';
		line1[1] = 'o';
		line1[2] = 'd';
		line1[3] = 'e'; 
		line1[4] = ':';
		line1[5] = mode + 48;
		line1[6] = ' ';
		line1[7] = ' ';
		
		tmp = oneshot_count * 10;
		
		
		line2[0] = tmp / 10000 + 48 ;
		line2[1] = tmp / 1000 % 10 + 48;
		line2[2] = tmp / 100 % 10 + 48;
		line2[3] = tmp / 10 % 10 + 48;
		line2[4] = tmp % 10 + 48;
		line2[5] = ' ';
		line2[6] = 'm';
		line2[7] = 's';
		DisplayString(LCD_LINE1, line1);
		DisplayString(LCD_LINE2, line2);
		
		
		counter = 0;
	}
}
/**********************************************************************************
End of function ta1_irq
***********************************************************************************/   

// Change mode
void sw0_irq(void)
{
	// Check mode
	if (mode == MODE0)
		mode = MODE1;
	else
		mode = MODE0;
		
	ta2_user_cnt = 0;	
}


// Trigger LED
void sw1_irq(void)
{

	// Reset or not!
	if (mode == MODE1) // Renew duration
	{
		ta2_user_cnt = 0;  // Reset count varable
		ta2os = 0; // Stop timer
		ta2os = 1; // Start timer
	}
	else if (ta2_user_cnt == 0) // in mode 1 and led not ON
	{
		ta2os = 1;
	}
	
		
	LED2 = LED_ON;	
}