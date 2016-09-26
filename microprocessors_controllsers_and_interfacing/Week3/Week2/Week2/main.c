#include "sfr62p.h"
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "lcd.h"
#include "utils.h"

#define RESOLUTION 100 // Samples per period

/* Interrupts */
#pragma INTERRUPT timer_a0_interrupt
#pragma INTERRUPT timer_a1_interrupt

#pragma INTERRUPT sw1;
#pragma INTERRUPT sw2;
#pragma INTERRUPT sw3;


void timer_a0_interrupt(void); // ADC, sample potential meter, LED display
void timer_a1_interrupt(void); // DAC - SINE_WAVE, TRIANGLE_WAVE, REAL_SIGNAL || ADC - REAL_SIGNAL

void adc_configuration(void); // Analog-Digital converter configuration
void dac_configuration(void); // Digital-Analog converter configuration

void update_display(void); // LCD display

/* Preconfigured data */
int sineData[RESOLUTION] = {51,67,81,92,100,102,100,92,81,67,51,35,21,10,2,0,2,10,21,35};
int triangleData[RESOLUTION] = {51,61,71,82,92,102,92,82,71,61,51,41,31,20,10,0,10,20,31,41};
int realData[RESOLUTION] = {0};

double freq_range[3] = {10,100,1000};
int freq_choice = 0;
double freq;

double ampl = 2.5; // 0 to 2.5
double percent = 0; // 0 to 100
double voltage = 0; // 0 to 5
double old_ratio = -1;
int data_pos = -1;

int max_adc_mode = 1023; // 1023 or 255 - 10 bit or 8 bit

enum MODES {NORMAL, SINE_WAVE, TRIANGLE_WAVE, RECORD, PLAYBACK};
enum MODES mode = RECORD;

/*********** SW stuff */
void sw1(void)
{		
	if (++mode > 4)  // Change mode
		mode = NORMAL;
		
	if (mode > 2) //  Reconfigure ADC when in sweep-repeat/repeat mode
	{
		adc_configuration();
	}
	
	data_pos = -1; // Reset buffer
	update_display();
}

void sw2(void) 
{
	if (mode == RECORD || mode == PLAYBACK) // Start recording / playing back
		data_pos = 0;
	else if (mode == SINE_WAVE || mode == TRIANGLE_WAVE) // Increase amplitude
	{
		if (++ampl > 2.5)
			ampl = 0;
	}
}

void sw3(void)
{
	if (mode == SINE_WAVE || mode == TRIANGLE_WAVE) // Change frequency range
	{
		if (++freq_choice > 2)
			freq_choice = 0;
	} 	
}


/*********** ADC stuff */
int timer_a0_interrupt_cnt = 0;
void timer_a0_interrupt(void)
{
	/* Sample potential meter */
	if (++timer_a0_interrupt_cnt > 10) // 10 x 10ms
	{
		double ratio;
		int knob_val;
		int *reg; 
		
		timer_a0_interrupt_cnt = 0;
		
		/* Read ad0 value, display % and Volt to LCD, turn on LED */	
	
		// Get ad0 value		
		reg = (int *)&ad0; // 0 -> 255 in 8b mode or 0 -> 1023 in 16b mode
		knob_val = *reg;
	
		// Calculate percentage, voltage and frequency
		
		ratio = (double)knob_val / max_adc_mode;
		
		if (ratio != old_ratio)
		{
			unsigned int temp;
			old_ratio = ratio;
			
			/* Calculate percentage, voltage and frequency*/
			percent =  ratio * 100;	// 0-100%
			voltage = ratio * 5;		// 0-5V
		
			freq = freq_range[freq_choice] + ratio * (9*freq_range[freq_choice]); // min_freq + (max_freq - min_freq) x ratio		
		
		
		
			/* Update ta1 */
			
			/* pre-config, 20 sample case */
			if (freq < 18750)
			{
				ta1 = 37500/freq - 1; // 24,000,000 / 32 / RESOLUTION = 37500
				ta1mr = 0x80;
			}
			
			else if (freq < 75000)
			{
				ta1 = 150000/freq - 1;
				ta1mr = 0x40;
			}
			
			/* Update display */
			update_display();
			
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
}


/*********** DAC - SINE_WAVE, TRIANGLE_WAVE, REAL_SIGNAL || ADC - REAL_SIGNAL*/ 
void timer_a1_interrupt(void)
{	
	
	/* Sample real signal, sampling frequency = showing frequency */
		
	if (mode == SINE_WAVE || mode == TRIANGLE_WAVE || mode == PLAYBACK) // DAC stuff
	{
		int data = 0;
		if (mode == SINE_WAVE)
		{
			data = sineData[data_pos] * ampl;			
		}
		else if (mode == TRIANGLE_WAVE)
		{
			data = triangleData[data_pos] * ampl;
		}
		else
		{
			data = realData[data_pos]/max_adc_mode*255; // convert adc_mode to dac mode (10/8 bit to 8bit)
		}
		
		if (++data_pos >= RESOLUTION)
				data_pos = 0;
		
		// Rounding and output
		//data += (data - (int)data >= 0.5) ? 1:0;			
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
			//update_display();
		}
	}
}


/********** Display sutff */
void update_display()
{	
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
	/* Repeate mode */
	else
	{
		adst = 0; // Turn off ADC
	
		adcon0 = 0x09; // Repeat sweep mode0, SW trigger
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
	
	/* Initlize frequency range */
	freq = freq_range[freq_choice];
	
	/* ADC and DAC configuration */
	adc_configuration();
	dac_configuration();
	
	
	/* Configure timer a0 - 10ms*/
	ta0mr = 0x40; // f8
	ta0 = 29999; // 10ms
	
	/* Configure timer a1*/
	ta1mr = 0x40;
	ta1 = 29;
	
	
	/* Configure priority level */
	DISABLE_IRQ			
	ta0ic = 0x04; 
	ta1ic = 0x02;
	
	int0ic = 0x06;
	int1ic = 0x06;
	int2ic = 0x06;
	ENABLE_IRQ
	
	/* Start timer */
	ta0s = 1;
	ta1s = 1;
	while(1){asm("NOP");};
}         