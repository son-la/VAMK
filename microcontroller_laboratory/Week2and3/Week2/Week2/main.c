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
/* Problems */
/*
 * How to use two ADC at the same time?
 *
 */

/**********************************************************************************
User Includes
***********************************************************************************/
#include "sfr62p.h"
/* rskM16C62p_def.h defines some common definitions */
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "lcd.h"
#include "utils.h"

#define RESOLUTION 20 // Sample per period


#pragma INTERRUPT timer_a0_interrupt
#pragma INTERRUPT timer_a1_interrupt
#pragma INTERRUPT timer_a2_interrupt

#pragma INTERRUPT sw1;
#pragma INTERRUPT sw2;
#pragma INTERRUPT sw3;



void timer_a0_interrupt(void); // ADC, sample potential meter, LED display and 
void timer_a1_interrupt(void); // DAC - SINE_WAVE, TRIANGLE_WAVE, REAL_SIGNAL || ADC - REAL_SIGNAL
void timer_a2_interrupt(void); // Display

void adc_configuration(void); // Analog-Digital converter configuration
void dac_configuration(void); // Digital-Analog converter configuration

void getSineWaveData(void);
void getTriangleWaveData(void);
long getLimit(void); // Use frequency and timer interval to return value

double sineData[RESOLUTION] = {0, 0.3090, 0.5878, 0.8090, 0.9511, 1.0000, 0.9511, 0.8090, 0.5878, 0.3090, 0.0000, -0.3090, -0.5878, -0.8090, -0.9511, -1.0000, -0.9511, -0.8090, -0.5878, -0.3090};
double triangleData[RESOLUTION] = {0, 0.2000, 0.4000, 0.6000, 0.8000, 0.9949, 0.8000, 0.6000, 0.4000, 0.2000, -0.0000, -0.2000, -0.4000, -0.6000, -0.8000, -0.9949, -0.8000, -0.6000, -0.4000, -0.2000};
int realData[RESOLUTION];

double max_freq = 500;
double min_freq = 0;
double freq;

double ampl = 2.5; // 0 to 2.5
double percent = 0;
double voltage = 0;
int data_pos = -1;

int max_adc_mode = 1023; // 1023 or 255 - 10 bit or 8 bit

enum MODES {NORMAL, SINE_WAVE, TRIANGLE_WAVE, RECORD, PLAYBACK};
enum MODES mode = NORMAL;

/*********** SW stuff */
void sw1(void)
{		
	if (++mode > 4)  // Change mode
		mode = NORMAL;
		
	if (mode > 2) //  Reconfigure ADC when in sweep-repeat/repeat mode
	{
		adc_configuration();
		data_pos = -1; // Reset buffer
	}
	else if (mode == NORMAL) // Reset buffer in normal mode (No need for output wave)
		data_pos = -1;
}

void sw2(void) 
{
	if (mode == RECORD || mode == PLAYBACK) // Start recording / playing back
		data_pos = 0;
	else if (mode == SINE_WAVE || mode == TRIANGLE_WAVE) // Increase amplitude
	{
		if (ampl >= 2.5)
			return;
		ampl += 0.1;
	}
}

void sw3(void)
{
	if (mode == SINE_WAVE || mode == TRIANGLE_WAVE) // Decrease amplitude
	{
		if (ampl <= 0)
			return;
		ampl -= 0.1;
	} 	
	else if (mode == NORMAL) // Change ADC mode 8 bit or 10 bit
	{
		if (max_adc_mode == 255)
			max_adc_mode = 1023;
		else 
			max_adc_mode = 255;
		
		adc_configuration();
	}
}


/*********** ADC stuff */
int timer_a0_interrupt_cnt_knob = 0;
int timer_a0_interrupt_cnt_record = 0;
void timer_a0_interrupt(void)
{
	/* Sample potential meter*/
	if (timer_a0_interrupt_cnt_knob++ >= 10) // 10 x 10ms
	{
		int knob_val;
		int *reg; 
		
		timer_a0_interrupt_cnt_knob = 0;
		
		/* Read ad0 value, display % and Volt to LCD, turn on LED */	
	
		// Get ad0 value		
		reg = (int *)&ad0; // 0 -> 255 in 8b mode or 0 -> 1023 in 16b mode
		knob_val = *reg;
	
		// Calculate percentage, voltage and frequency
		percent = (double)knob_val / max_adc_mode * 100;	// 0-100%
		voltage = (double)knob_val / max_adc_mode * 5;		// 0-5V
		freq = min_freq + percent*(double)(max_freq-min_freq)/100 ; // min_freq - max_freq
		

		
		/* Turn on LED */
		if (percent < 25)
			p4 = 0xfe;
		else if (percent < 50)
			p4 = 0xfc;
		else if (percent < 75)
			p4 = 0xf8;
		else
			p4 = 0xf0;
	}
}


/*********** DAC - SINE_WAVE, TRIANGLE_WAVE, REAL_SIGNAL || ADC - REAL_SIGNAL*/ 
long timer_a1_interrupt_cnt = 0;
void timer_a1_interrupt(void)
{
	long iLimit;
	
	if (freq == 0)
		da0 = 0;
	else
	{
		/* Sample real signal, sampling frequency = showing frequency */
		iLimit = getLimit();
		if (++timer_a1_interrupt_cnt >= iLimit) 
		{
			timer_a1_interrupt_cnt = 0;
			if (mode == SINE_WAVE || mode == TRIANGLE_WAVE || mode == PLAYBACK) // DAC stuff
			{
				double data = 0;
				
				if (mode == SINE_WAVE)
				{
					data = (sineData[data_pos] + 1)*ampl*(double)255/5;			
					if (++data_pos >= RESOLUTION)
						data_pos = 0;
				}
				else if (mode == TRIANGLE_WAVE)
				{
					data = (triangleData[data_pos] + 1)*ampl*(double)255/5;
					if (++data_pos >= RESOLUTION)
						data_pos = 0;
				}
				else
				{
					if (data_pos > -1 && data_pos < RESOLUTION)
					{
						data = realData[data_pos]/(double)max_adc_mode*255; // convert adc_mode to dac mode (10/8 bit to 8bit)
						// max_adc_mode + 1, check later
						++data_pos;
					}
				}
				
				// Rounding and output
				data += (data - (int)data >= 0.5) ? 1:0;			
				da0 = (int)data;
			}
			else if (mode == RECORD) // ADC stuff
			{
				if (data_pos > -1 && data_pos < RESOLUTION) // buffer available
				{
					int *reg;			
					// Get ad1 value
		
					reg = (int *)&ad1; // 0 -> 255 in 8b mode or 0 -> 1023 in 16b mode
					realData[data_pos] = *reg;
					++data_pos;
				}
			}
		}
	}	
}


/********** Display sutff */
int timer_a2_interrupt_cnt = 0;
void timer_a2_interrupt(void)
{
	if (++timer_a2_interrupt_cnt >= 10) // 10 x 10ms
	{
		timer_a2_interrupt_cnt = 0;	
		
		// Check mode and display
		if (mode == NORMAL)
		{
			// Display voltage
			lcd_display_floating_point(voltage, 1, 'V', 1);
			// Display percentage
			lcd_display_floating_point(percent, 3, '%', 2);
		}
		else if (mode == SINE_WAVE || mode == TRIANGLE_WAVE)
		{
			// Display frequency
			if (freq < 1000)
				lcd_display_floating_point(freq, 3, 'H', 1);
			else
				lcd_display_floating_point(freq/1000, 2, 'K', 1);
			
			// Display percentage
			lcd_display_floating_point(percent, 3, '%', 2);
		}
		else if (mode == RECORD)
		{
			// Show percentage record
			lcd_display_recording(data_pos, RESOLUTION);	
		}
		else if (mode == PLAYBACK)
		{
			// Show percentage playback
			lcd_display_playback(data_pos, RESOLUTION);
		}
	}
}

void adc_configuration()
{
	/* Repeat mode */
	if (mode != RECORD)
	{
		adst = 0; // Turn off ADC
	
		adcon0 = 0x08; // AD0, Sw trigger
		if (max_adc_mode == 255)
		{
			adcon1 = 0x20; // Vref connected, 8bit mode
		}
		else
		{
			adcon1 = 0x28;	// Vref connected, 10bit mode
		}
		adcon2 = 0x00; // No sample and hold
		adst = 1; // Enable ADC
	}
	/* Sweep repeate mode */
	else
	{
		adst = 0; // Turn off ADC
	
		adcon0 = 0x18; // Repeat sweep mode0, SW trigger
		if (max_adc_mode == 255)
		{
			adcon1 = 0x20;
		}
		else
		{
			adcon1 = 0x28;
		}
		adcon2 = 0x00;
		
		DISABLE_IRQ	
		adic = 0x01;
		ENABLE_IRQ
		
		adst = 1;
	}
}

void dac_configuration(void)
{
	da0e = 0; // Turn off DAC
	prc2 = 1; // Turn off protect bit
	pd9_3 = 0; // P9_3 as input
	da0 = 0; 
	da0e = 1; // Enable DAC
}

void main(void)				
{
	InitialiseDisplay();
	
	freq = max_freq;
	
	/* ADC and DAC configuration */
	adc_configuration();
	dac_configuration();
	//getSineWaveData(); // Prepare sine wave data
	//getTriangleWaveData(); // Prepare triangle wave data
	
	
	/* Configure timer a0 - 10ms*/
	ta0mr = 0x40; // f8
	ta0 = 29999; // 10ms
	
	/* Configure timer a1 - 10us*/
	ta1mr = 0x40;
	ta1 = 29;
	
	/* Configure timer a2 - 10ms*/ 
	ta2mr = 0x40;
	ta2 = 29999;
	
	/* Configure priority level */
	DISABLE_IRQ			
	ta0ic = 0x04; 
	ta1ic = 0x02;
	ta2ic = 0x03;
	int0ic = 0x06;
	int1ic = 0x06;
	int2ic = 0x06;
	ENABLE_IRQ
	
	/* Start timer */
	ta0s = 1;
	ta1s = 1;
	ta2s = 1;
	while(1);
}         



void getSineWaveData(void)
{
	int i;
	
	for (i = 0; i < RESOLUTION/2; i++)	
	{
		sineData[i] = getSine(0 + (double)i*2*PI/RESOLUTION); // 0 -> PI
	}
}

void getTriangleWaveData(void)
{
	int i;
	for (i = 0; i < RESOLUTION; i++)	
	{
		triangleData[i] = getTriangle(0 + (double)i*2/RESOLUTION); // 0 -> 2
	}
}

long getLimit(void) 
{
	double fLimit = 0;
	
	// Timer a1 interval = 10us
	fLimit = (double)100000/(freq*(double)RESOLUTION); // 1/(f x RESOLUTION) = Duration for one sample (in second) |||||| x 10e5 = number in 10us
	fLimit += (fLimit - (long)fLimit) >= 0.5 ? 1:0;	
	return (long)fLimit;
}