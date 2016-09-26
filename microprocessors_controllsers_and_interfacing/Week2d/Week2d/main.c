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
#include "lcd_display.h"


#pragma INTERRUPT sw1 // SET
#pragma INTERRUPT sw2 // +
#pragma INTERRUPT sw3 // -

#pragma INTERRUPT timer_a0_interrupt // Timer a0 - Mainclock
#pragma INTERRUPT timer_a1_interrupt // Timer a1 - Display
#pragma INTERRUPT timer_a2_interrupt // Timer a2 - Oneshot

// Set default
enum operation_modes mode = NORM; // NORM - SET DC - SET TH

int dc_val = 50;
int th_val = 40;
double tp_val = 80;
int pwm_mode = 8;
long n = 0;
long m = 0;
long freq = 0;

int sw_press = 1;

double update_tp()
{
	return (double)th_val/((double)dc_val/100);
}

void sw1_func(void) // Change modes
{
	if (mode == SET_TH)
	{
		mode = NORM;
		// After change value, update output signal
		PWM_config(); 
	}
	else
		++mode;
}

void sw2_func(void) // Increase
{
	switch (mode)
	{
		case SET_DC:
		{
			//Increase DC - max 99%
			if (++dc_val > 99)
					dc_val = 1;
			
			tp_val = update_tp();
			break;
		}
		case SET_TH:
        {
			// Increase th - ms
			++th_val;
			/*
			while (update_tp() == th_val) // Avoid tp == th
			{
				++th_val;
			}
			*/
			tp_val = update_tp();
            break;
        }	
	}
}

void sw3_func(void) // Decrease
{
	switch (mode)
	{
		case NORM:
		{
			// Switch PWM mode
			if (pwm_mode == 16)
				pwm_mode = 8;
			else
				pwm_mode = 16;
			PWM_config();
			break;	
		}
		case SET_DC:
		{
			// Decrease DC min 1%
			if (--dc_val < 1)
				dc_val = 99;
				
			tp_val = update_tp();
			
			break;
		}
		case SET_TH:
        {
			// Decrease th - ms
			int old_th_val = th_val;
			if (th_val > 1)
				--th_val;
			/*
			while (update_tp() == th_val)  // Avoid tp == th
			{
				if (th_val > 1)
					--th_val;
				else
					th_val = old_th_val;
					break;
			}
			*/
			tp_val = update_tp();
            break;
        }	
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



void timer_a0_interrupt(void) // Main clock
{
		
}

int msec = 0;
void timer_a1_interrupt(void) // Display function
{
	if(++msec == 50){
		msec = 0;
		switch (mode)
		{
			case NORM:
			{
				LCD_display_normal(pwm_mode, n, m, freq);	
				break;	
			}
			case SET_DC:
			{
				LCD_dispaly_set_dc(dc_val);
				break;
			}
			case SET_TH:
			{
				LCD_display_set_th(th_val);
				break;
			}
		}
	}
}

int ta2_user_cnt = 0;
void timer_a2_interrupt(void) // One-shot
{
  	if (++ta2_user_cnt == 3)
	{
   		sw_press = 1;
		ta2_user_cnt = 0;
		return;
	}
	ta2os = 1; // renew timer	
}

void PWM_config()
{
	ta0s = 0;
	switch (pwm_mode)
	{
		case 8:
		{
			double fn = 255*(double)th_val/tp_val;
			long f = 24000; // tp, th are already in ms
			double fm = (double)tp_val * (double)f/255 - 1; // f1
			double fta0;
			long my_ta0;
			
			// Get n
			fn += (fn - (long)fn >= 0.5) ? 1:0;
			n = (long)fn;
			
			// Get m
			if (fm >= 255)
			{
					fm = (double)tp_val * (double)f/8/255 - 1; // f8
					if (fm >= 255)
					{
						fm = (double)tp_val * (double)f/32/255 - 1; // f32
						if (fm < 255)
						{
							p4 = 0;
							ta0mr = 0xa7; // 8 bit - f32
							freq = f*1000/32;
						}
					}
						
					else
					{
						ta0mr = 0x67; // 8 bit - f8
						freq = f*1000/8;	
					}
			}
			else
			{
				ta0mr = 0x27;  // 8 bit - f1
				freq = f*1000;	
			}
			
			
			fm += (fm - (long)fm >= 0.5) ? 1:0;				
			m = (long)fm;
	
			
			ta0ic = 0x1;
			fta0 = 256 * fn + fm;
			
			
			
			ta0 = n*256+m;
			

			
			break;
		}	
		case 16:
		{
			
			double fn = (double)65535 * (double)dc_val / 100; // float n
			
			fn += (fn - (long)fn >= 0.5)?1:0;
			n = (long)fn;
			
				
			ta0mr = 0x07; // 16 bit
			
			ta0ic = 0x1;
			ta0 = n;
			break;	
		}	
	}
	ta0s = 1;
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
	
	// PWM timer
	PWM_config();
	
	// Display
	ta1mr = 0x40; //f8
	ta1 = 29999; 
	
	// One shot
	ta2 = (unsigned int)((f1_CLK_SPEED/32)*0.05)-1; // 50ms pulse
	ta2mr = 0x86 ;
	ta2ic = 0x03;
	

	ta1ic = 0x01; // Set priority
	ta0ic = 0x01;
	
	int0ic = 0x02; // Set priority
	int1ic = 0x02; // Set priority
	int2ic = 0x02; // Set priority
	
	tabsr |= (1 << 1); // Start timer a1	
	
	asm("FSET I");
	
	ta2s = 1;  // Start timer a2
	ta0s = 1;
	ta1s = 1;
	
	while(1){}	
}         
/**********************************************************************************
End of function main
***********************************************************************************/   

