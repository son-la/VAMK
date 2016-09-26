#include "sfr62p.h"
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "lcd.h"
#include "utils.h"

#define RESOLUTION 20 // Samples per period

/* Interrupts */
#pragma INTERRUPT timer_a1_interrupt
#pragma INTERRUPT timer_a0_interrupt

#pragma INTERRUPT sw1;
#pragma INTERRUPT sw2;
#pragma INTERRUPT sw3;


void timer_a0_interrupt(void);
void timer_a1_interrupt(void); // DAC playback, ADC record

void adc_configuration(void); // Analog-Digital converter configuration
void dac_configuration(void); // Digital-Analog converter configuration

void update_display(void); // LCD display

/* Preconfigured data */

int freq_range[3] = {10,100,1000};
int freq_choice = 1;
float freq;
float ampl = 2.5;

int realData[RESOLUTION] = {0};

int data_pos = -1;

float old_knob_val = -1;
int max_adc_mode = 1023; // 1023 or 255 - 10 bit or 8 bit

enum MODES {RECORD, PLAYBACK};
enum MODES mode = PLAYBACK;

int sw_press = 0;
int tik = 1;
/*********** SW stuff */
void sw1(void)
{		
	if (sw_press == 0)
	{
		sw_press = 3;
		
		
		
		if (mode == PLAYBACK)  // Change mode
		{
			mode = RECORD;
			ta1mr = 0x40;
			ta1 = 149;
		}
		else
		{
			
			mode = PLAYBACK;
			old_knob_val = -1;
		}
	
		data_pos = -1; // Reset buffer
		update_display();
		adc_configuration();
	}
}

void sw2(void) 
{
	if (sw_press == 0)
	{
		sw_press = 3;
		if (mode == RECORD)
		{
			data_pos = 0;
		}
	}
}

void sw3(void)
{
	if (sw_press == 0)
	{
		old_knob_val = -1;	
	}
	
}

int timer_a0_interrupt_cnt = 0;
void timer_a0_interrupt(void)
{
	if (++timer_a0_interrupt_cnt > 10)
	{
		timer_a0_interrupt_cnt = 0;
		if (sw_press > 0)
			--sw_press;
		if (mode == PLAYBACK)
		{
			float ratio;
			int knob_val;
			int *reg; 
			
			knob_val = *reg;
			
			if (knob_val != old_knob_val)
			{
				int i;
				knob_val = 100;
				ratio = (float)knob_val/255;

				freq = freq_range[freq_choice]*( (float)1 + (float)9.0 *ratio); // min_freq + (max_freq - min_freq) x ratio					 
				
				ta1 = 150000/freq - 1;
				
				ta1mr = 0x40;
				
				
				
				
				old_knob_val = knob_val;
			}
		}
		
	}
}

/*********** DAC - SINE_WAVE, TRIANGLE_WAVE, REAL_SIGNAL || ADC - REAL_SIGNAL*/ 
void timer_a1_interrupt(void)
{	
	
	/* Sample real signal, sampling frequency = showing frequency */
		
	if (mode == PLAYBACK) // DAC stuff
	{
		int data = 0;
	
		data = realData[data_pos]; // convert adc_mode to dac mode (10/8 bit to 8bit)
		
		if (++data_pos >= RESOLUTION)
				data_pos = 0;
		
		// Rounding and output		
		da0 = data;
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
			
			if (data_pos == RESOLUTION)
			{
				int i;
				for (i = 0; i < RESOLUTION; i++)
				{
					realData[i] = realData[i]/4;	
				}	
				update_display();
			}
		}
	}
}


/********** Display sutff */
void update_display()
{	
	
	if (mode == RECORD)
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

void adc_configuration()
{
	if (mode == RECORD)
	{
		adst = 0; // Turn off ADC

		adcon0 = 0x89; // Repeat sweep mode0, SW trigger
		adcon1 = 0x28; // 10 bit	
		adcon2 = 0x00;
	
		DISABLE_IRQ	
		adic = 0x01;
		ENABLE_IRQ
	
		adst = 1;
	}
	else
	{
		adst = 0; // Turn off ADC
	
		adcon0 = 0x08; // AD0, Sw trigger
		adcon1 = 0x20;	// Vref connected, 10bit mode
		adcon2 = 0x00; // No sample and hold
		
		
		DISABLE_IRQ	
		adic = 0x01;
		ENABLE_IRQ
		
		adst = 1; // Enable ADC
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
	
	
	/* ADC and DAC configuration */
	adc_configuration();
	dac_configuration();
	
	/* Configure timer a0*/
	ta0mr =0x40;
	ta0 = 29999;
	
	/* Configure timer a1*/
	ta1mr = 0x40;
	ta1 = 149;
	
	
	/* Configure priority level */
	DISABLE_IRQ			
	ta0ic = 0x03; 
	ta1ic = 0x02;
	
	int0ic = 0x06;
	int1ic = 0x06;
	int2ic = 0x06;
	ENABLE_IRQ
	
	update_display();
	
	/* Start timer */
	ta1s = 1;
	ta0s = 1;
	
	while(1);
}         