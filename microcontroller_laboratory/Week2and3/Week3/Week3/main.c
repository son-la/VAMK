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
#include "sfr62p.h"
/* rskM16C62p_def.h defines some common definitions */
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "utils.h"

#define RESOLUTION 20 // How many sample in array

#pragma INTERRUPT timer_a0_interrupt // DAC sine wave
#pragma INTERRUPT timer_a1_interrupt // ADC, Display
#pragma INTERRUPT timer_a2_interrupt // Oneshot

#pragma INTERRUPT sw1;
#pragma INTERRUPT sw2;
#pragma INTERRUPT sw3;

void PWM_config(void);
void ADC_config(void);
long getLimit(void); // Use frequency and timer interval to return value
void update_dc_fq(enum MODES update_mode);

enum MODES mode = NORMAL; // Steady display

double sineData[RESOLUTION] = {0, 0.3090, 0.5878, 0.8090, 0.9511, 1.0000, 0.9511, 0.8090, 0.5878, 0.3090, 0.0000, -0.3090, -0.5878, -0.8090, -0.9511, -1.0000, -0.9511, -0.8090, -0.5878, -0.3090};
int fq_range_min[] = {10, 100, 1000}; // range_max = range_min x 10;
int dc_range_min[] = {0, 25, 50, 75}; // range_max = range_min + 25;

int fq_choice = 0, dc_choice = 0; // fq:0->2, dc:0->3
int fq_val, dc_val;
double ampl = 2.5; // 0 to 2.5
long n, m;

int sw_press = 1;
void sw1_func(void)
{
	// Switch mode
	if (++mode > 2)
	{
		mode = NORMAL;
	}
	
	// Update DC and FQ
	if (mode == NORMAL)
		update_dc_fq(SET_DC);
	else if (mode == SET_DC)
		update_dc_fq(SET_FQ);
}

void sw2_func(void)
{
	// Increase
	switch (mode)
	{
		case SET_FQ:
		{
			if (++fq_choice > 2)
				fq_choice = 0;
			break;
		}
		case SET_DC:
		{
			if (++dc_choice > 3)
				dc_choice = 0;
			break;			
		}
		case NORMAL:
		{
			if (ampl == 2.5)
				return;
			ampl += 0.1;
			break;
		}
	}
}

void sw3_func(void)
{
	// Decrease
	switch (mode)
	{
		case SET_FQ:
		{
			if (--fq_choice < 0)
				fq_choice = 2;
			break;
		}
		case SET_DC:
		{
			if (--dc_choice < 0)
				dc_choice = 3;
			break;			
		}
		case NORMAL:
		{
			if (ampl == 0)
				return;
			ampl -= 0.1;
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



void update_n(void) // As its name
{
	double fn = dc_val/100 * 255;	
	fn += (fn - (long)fn >= 0.5)?1:0;
	n = (long)fn;
}

int update_m(void) // As its name
{
	// Update m according to fq_val, return f_j choice: 1,8,32 or 0 if cannot find m
	double f_j = 24000000; // 24MHz
	int ret = 0; // Frequency config value
	double fm  = ((double)1/fq_val * f_j)/255 -1;
	if (fm > 255)
	{
		fm  = ((double)1/fq_val * f_j/8)/255 -1;
		if (fm > 255)
		{
			fm  = ((double)1/fq_val * f_j/32)/255 -1;
			if (fm < 255)
				ret = 32; // f32
			else
				fm = n; // No output
		}
		else
			ret = 8; // f8
	}
	else
		ret = 1; // f1
	
	fm += (fm - (long)fm >= 0.5)?1:0;
	m = (long)fm;
	
	return ret;	
}

void PWM_config(void)
{
	int f_j;
	ta0s = 0;
	
	update_n();
	f_j = update_m();
	
	switch (f_j)
	{
		case 1:
		{
			ta0mr = 0x27; // f1
			break;
		}
		case 8:
		{
			ta0mr = 0x67; // f8
			break;
		}
		case 0:
		case 32:
		{
			ta0mr = 0xa7; // f32
			break;	
		}
	}
	
	ta0 = 256*n + m;
	ta0s = 1;	
}

void ADC_config(void)
{
	adst = 0; // Turn off ADC
	adcon0 = 0x08; // AD0, Sw trigger
	adcon1 = 0x20; // Vref connected, 8bit mode
	adcon2 = 0x00; // No sample and hold
	adst = 1; // Enable ADC	
}

void update_dc_fq(enum MODES update_mode)
{
	int knob_val;
	int *reg; 	
	double ratio;
	
	
	reg = (int *)&ad0; // 0 -> 255 in 8b mode
	knob_val = *reg;
	
	ratio = (double)knob_val / 255;	// Could be 256, check later
	
	switch (update_mode)
	{
		case SET_DC:
		{
			dc_val = dc_range_min[dc_choice] + (double)25*ratio;
			break;	
		}
		case SET_FQ:
		{
			fq_val = fq_range_min[fq_choice] + (double)fq_range_min[fq_choice] * 9 * ratio; // fq_range_min + (fq_range_max - fq_range_min) x ratio
			break;	
		}
	}
}




int timer_a1_interrupt_cnt = 0;
void timer_a1_interrupt(void) 
{
	// Display work
	if (++timer_a1_interrupt_cnt == 10) // 100ms
	{
		timer_a1_interrupt_cnt = 0;
		lcd_display_function(mode, dc_val, fq_choice);
	}	
}


int timer_a0_interrupt_cnt = 0;
int data_pos = 0;
void timer_a0_interrupt(void)
{
	long iLimit;
	
	
	iLimit = getLimit();
	if (++timer_a1_interrupt_cnt >= iLimit) 
	{
		double data = 0;
	
		timer_a1_interrupt_cnt = 0;
	
		
		data = (sineData[data_pos] + 1)*ampl*(double)255/(double)5;			
		if (++data_pos >= RESOLUTION)
			data_pos = 0;
		
		
		
		data += (data - (int)data >= 0.5) ? 1:0;			
		da0 = (int)data;
	}
		
}

void DAC_config(void)
{
	da0e = 0; // Turn off DAC
	prc2 = 1; // Turn off protect bit
	pd9_3 = 0; // P9_3 as input
	da0 = 0; 
	da0e = 1; // Enable DAC
}




/**********************************************************************************
Function Name: 	Main
Description:	Main function
Parameters: 	none
Return value: 	none
***********************************************************************************/
void main(void)				
{
	ADC_config(); // Get knob value
	update_dc_fq(SET_DC); // Get dc_val
	update_dc_fq(SET_FQ); // Get fq_val
	
	PWM_config(); // Config PWM in case square_wave
	DAC_config(); // Config DAC in case sine_wave
		
		
	
	/* Configure timer a1 for timer - 10ms*/
	ta1mr = 0x40;
	ta1 = 29999;
	
	/* Configure timer a2 for one-shot */
	ta2 = (unsigned int)((f1_CLK_SPEED/32)*0.05)-1; // 50ms pulse
	ta2mr = 0x86 ;
	
	/* Configure timer a0 for DAC - 10us */
	ta1mr = 0x40;
	ta1 = 29;
	
	
	
	/* Set priority */
	ta2ic = 0x01; 
	ta1ic = 0x01;
	
	int0ic = 0x02;
	int1ic = 0x02;
	int2ic = 0x02;
	
	while(1);
}         
/**********************************************************************************
End of function main
***********************************************************************************/   

long getLimit(void) 
{
	double fLimit = 0;
	
	// Timer a1 interval = 10us
	fLimit = (double)100000/(fq_val*(double)RESOLUTION); // 1/(f x RESOLUTION) = Duration for one sample (in second) |||||| x 10e5 = number in 10us
	fLimit += (fLimit - (long)fLimit) >= 0.5 ? 1:0;	
	return (long)fLimit;
}
