/***********************************************************************************
FILE NAME  	:	main_pm.c 
DESCRIPTION	:	main routine for Timer - Pulse Period/Width Measurement									

				Timer B0 is configured to measure pulse width in TB0in  
	          	while timer B1 is used to measures the period TB1in. To see  
	           	pulse measurments a 250Hz square wave with a 0.5msec pulse width 
				(high)from a function generator should be connected to both 
				timer inputs (JA6-11 and Ja6-12).  

				1. Download this code & run the program 
				2. Stop the execution & observe values of variables "width_low" & "width_high".
                   				        					
Copyright   : 2005 Renesas Technology Europe Ltd.
Copyright   : 2005 Renesas Technology Corporation.
All Rights Reserved

***********************************************************************************/
/* Ultilize count source, Mhz display*/
/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
11.08.2005 RTA-MGF First Release
***********************************************************************************/


#include "sfr62p.h"
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "lcd.h"

/**********************************************************************************
User Defines
***********************************************************************************/
#define LCD_MAX 16// Two lines
#define STRING_LENGTH 60


/*
f1	24000000	2.7ms - 0.04us	 	370Hz -> 25 MHz
f8: 3000000 	21ms - 0.3us 		47.6Hz -> 3.3MHz
f32 750000		87 ms - 1.3us		11.5Hz -> 750kHz
*/

unsigned short usPeriod = 0, usWidth_low = 0, usWidth_high = 0;
void lcd_display_function(void);

int freq_range[3] = {1,8,32};
int freq_choice = 2;
void sw0(void)
{
	if (++freq_choice > 2)
		freq_choice = 0;
		
	if (freq_choice == 1)
	{
		p4 = 0xfc; // 1100
	}
	else if (freq_choice == 2)
	{
		
		p4 = 0xf8; // 1000
	}
	else
	{
		p4 = 0xfe; // 1110
	}
		
	timer_init();
}

/**********************************************************************************
Function Name: 	Main
Description:	Initializes timers B0 & B1. Then the variable 'period' is constantly
             	updated with the period count in timer TB1. This is to illustrate
			 	that the period measurement is free running. Note that the first
			 	few times TB1 is read, the data may not be valid.
Parameters: 	none
Return value: 	none
***********************************************************************************/
void main (void)
{ 
	InitialiseDisplay();
  	/* Sets up timer B0 for pulse width measurement and
	 timer B1 for pulse period measurement */
	timer_init();
	
	/* period measured in polled mode */
  	while (1) {}
}
/**********************************************************************************
End of function main
***********************************************************************************/   

/**********************************************************************************
Function Name: 	tb0_irq
Description:	Timer B0 Interrupt Service Routine. The overflow flag is check 
				to determine if the TB0 register contains valid data. If so, the 
				input is tested to determine if the value in the TB0 register
				is the high pulse width or low width which is then stored in the 
				appropriate	variable. 
Parameters: 	none
Return value: 	none
***********************************************************************************/
void tb0_irq(void)
{ 

	/* check for timer overflow */
	
  	if (mr3_tb0mr == 1)
	{	
		/* if so clear flag and data invalid, so exit routine */
  	 	if (freq_choice == 0)
		{
			tb0mr = 0x0a;
		}
		else if (freq_choice == 1)
		{
			tb0mr = 0x4a;					
		}
		else 
		{
			tb0mr = 0x8a;					
		}  
		usWidth_low = 0;
		usWidth_high = 0;
		usPeriod = 0;
    	return;					
  	}
	
	usPeriod = tb1 ;
	
	/* check if high or low pulse width was measured if input is now hi, 
		low width measured if input is now low, high width measured */
 	if (p9_0 == 1)
		/* input is now hi, low width measured */
	{
		usWidth_low = tb0;       
	}
 	else
		/* input is now low, high width measured */
	{
		usWidth_high = tb0;	
	}
	
}
/**********************************************************************************
End of function tb0_irq
***********************************************************************************/   

/**********************************************************************************
Function Name: 	tmr_init
Description:	Setups timer B0 for pulse width measurement and timer B1 for 
             	pulse period measurement.   
Parameters: 	none
Return value: 	none
***********************************************************************************/
 void timer_init()
 {
	
	
   	/* turn off interrupts before modifying Interrupt Priority Level */
	DISABLE_IRQ	        
  
	/* Set TB0in & TB1in pins as inputs */
	prc2=1;
	pd9=0xfc;
	prc2 =0;
	if (freq_choice == 0)
	{
		tb0mr = 0x0a;
		tb1mr = 0x06;		
	}
	else if (freq_choice == 1)
	{
		tb0mr = 0x4a;
		tb1mr = 0x46;			
	}
	else 
	{
		tb0mr = 0x8a;
		tb1mr = 0x86;			
	}
	
   	//tb0mr = 0x4a; 
  			/*  01001010   
      		b1:b0	TMOD1,TMOD0		PULSE MEASUREMENT MODE
     		b3:b2	MR1,MR0			PULSE WIDTH MEASUREMENT   
      		b4		MR2				0 FOR PULSE MEASUREMENT
      		b5		MR3		        OVERFLOW FLAG 
     		b7:b6	TCK1,TCK0		F DIVIDED BY 8 SELECTED */

	 /* set the interrupt priority level */
    tb0ic = 0x03;  

	//tb1mr = 0x46; 
  			/*  01000110   
      		b1:b0	TMOD1,TMOD0		PULSE MEASUREMENT MODE
     		b3:b2	MR1,MR0			PULSE PERIOD MEASUREMENT  
      		b4		MR2				0 FOR PULSE MEASUREMENT
      		b5		MR3		        OVERFLOW FLAG 
     		b7:b6	TCK1,TCK0		F DIVIDED BY 8 SELECTED */ 

   	/* turn on interrupts */
	tb1ic = 0x03;
	int0ic = 0x04;
	ta0mr = 0x40;
	ta0 = 29999;
	ta0ic = 0x02;
	ENABLE_IRQ				

	/* start timers counting */  
	ta0s = 1;
   	tb0s = 1;    
   	tb1s = 1;    
}
/**********************************************************************************
End of function tmr_init
***********************************************************************************/   
int timer=0;
void ta0_irq(void)
{
	if (++timer >= 50)
	{
		timer = 0;
		
		if (p9_0 == 1)
		{
			lcd_display_function();
		}
	 	else
		{
			lcd_display_function();
		}	
		
	}
}
int first_char_left = 0;  //First character in first lcd position

void lcd_display_function(void)
{
		unsigned short pulse_high, pulse_low, period; // us
		unsigned long long_pulse_high, long_pulse_low, long_period, long_freq, long_duty_cycle;
		double duty_cycle; // $
		double freq; // Hz
		char string[STRING_LENGTH];
		/* For scrolling */
		int cnt; // counter
		int char_pos; // current position in string
		
		timer = 0;

		if (freq_choice == 1)
		{
			period = usPeriod/3;
			pulse_high = usWidth_high/3;
			pulse_low = usWidth_low/3;
		}
		else if (freq_choice == 0)
		{
			period = usPeriod/24;
			pulse_high = usWidth_high/24;
			pulse_low = usWidth_low/24;
		}
		else
		{
			period = usPeriod/0.75;
			pulse_high = usWidth_high/0.75;
			pulse_low = usWidth_low/0.75;
		}
		//period = pulse_high+pulse_low;
		
		if (period != 0)		
		{
			freq = (double)1/(double)period *(double)1000000; 
			duty_cycle = (double)pulse_high/(double)period*(double)100;
		}
		else
		{
			freq = 0;
			duty_cycle= 0;	
		}
	
	
	
		/*******************************/
		/*******String preparation******/
		/*******************************/
	
		/* Time high */
		string[0] = ' ';
		string[1] = 'T';
		string[2] = 'h';
		string[3] = '=';
		if (pulse_high > 999)
		{
			string[10] = 'm';
			long_pulse_high = pulse_high/10;
		}
		else
		{
			string[10] = 'u';
			long_pulse_high = pulse_high*100;	
		}
		string[4] = long_pulse_high/10000%10 + 48;
		string[5] = long_pulse_high/1000%10 + 48;
		string[6] = long_pulse_high/100%10 + 48;
		string[7] = '.';
		string[8] = long_pulse_high/10%10 + 48;
		string[9] = long_pulse_high%10 + 48;
	
		string[11] = 's';
	
		/* Time low */
		string[12] = ' ';
		string[13] = 'T';
		string[14] = 'l';
		string[15] = '=';
		if (pulse_low > 999)
		{
			string[22] = 'm';
			long_pulse_low = pulse_low/10;
		}
		else
		{
			string[22] = 'u';
			long_pulse_low = pulse_low*100;	
		}
		string[16] = long_pulse_low/10000%10 + 48;
		string[17] = long_pulse_low/1000%10 + 48;
		string[18] = long_pulse_low/100%10 + 48;
		string[19] = '.';
		string[20] = long_pulse_low/10%10 + 48;
		string[21] = long_pulse_low%10 + 48;
	
		string[23] = 's';

		/* Total Period*/
		string[24] = ' ';
		string[25] = 'T';
		string[26] = 'p';
		string[27] = '=';
		if (pulse_low > 999)
		{
			string[34] = 'm';
			long_period = period/10;
		}
		else
		{
			string[34] = 'u';
			long_period = period*100;	
		}
		string[28] = long_period/10000%10 + 48;
		string[29] = long_period/1000%10 + 48;
		string[30] = long_period/100%10 + 48;
		string[31] = '.';
		string[32] = long_period/10%10 + 48;
		string[33] = long_period%10 + 48;
	
		string[35] = 's';
	
		/* Frequency */
		string[36] = ' ';
		string[37] = 'F';
		string[38] = '=';
		if (freq > 999999)
		{
			string[45] = 'M';
			string[46] = 'H';
			long_freq = (long)(freq/10000);
		}
		else if (freq > 999)
		{
			string[45] = 'k';
			string[46] = 'H';
			long_freq = (long)(freq/10);
		}
		else
		{
			string[45] = 'H';
			string[46] = 'z';
			long_freq = (long)(freq*100);	
		}
		string[39] = long_freq/10000%10 + 48;
		string[40] = long_freq/1000%10 + 48;
		string[41] = long_freq/100%10 + 48;
		string[42] = '.';
		string[43] = long_freq/10%10 + 48;
		string[44] = long_freq%10 + 48;
	
		/* Duty Cycle */
		duty_cycle *= 100;
		long_duty_cycle = (long)(duty_cycle += (duty_cycle - (long)duty_cycle >= 0.5 ? 1 : 0));
		string[47] = ' ';
		string[48] = 'D';
		string[49] = 'C';
		string[50] = '=';
		string[51] = long_duty_cycle/10000%10 + 48;
		string[52] = long_duty_cycle/1000%10 + 48;
		string[53] = long_duty_cycle/100%10 + 48;
		string[54] = '.';
		string[55] = long_duty_cycle/10%10 + 48;
		string[56] = long_duty_cycle%10 + 48;
		string[57] = '%';
		string[58] = ' ';
		string[59] = ' ';
	
	
	
		/***********/
		/* Rolling */
		/***********/
	
		/* Move pointer to line 1 */ 
		LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L1) );
	
		char_pos = first_char_left; // First char to print 
	
		// Loop through string, print out
		for (cnt = 0; cnt < LCD_MAX; cnt++)
		{
			LCD_write(DATA_WR, string[char_pos]);
			if (++char_pos == STRING_LENGTH)
				char_pos = 0;
			// Auto newline || Remove if only oneline
			if (cnt == 7)
				LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L2) );
		}
	
		// shift character, new pattern
		if (++first_char_left == STRING_LENGTH)
			first_char_left = 0;
	
}