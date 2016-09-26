/*
Sw1: Change mode: NORMAL -> SINE -> TRIANGLE -> RECORD -> PLAYBACK
Sw2: 
	- RECORD: start recording
	- else: Increase amplitude
Sw3: Change frequency range


Oscilopscope -> J1-4
Function generator -> J4-15
*/

#include "task2.h"
#include "sfr62p.h"
#include "adc_stuff.h"
#include "dac_stuff.h"
#include "timer_stuff.h"
#include "lcd_utils.h"


int task2_sineData_copy[TASK2_RESOLUTION] = {128,168,203,230,250,255,250,230,203,168,128,88,53,25,5,0,5,25,53,88};
int task2_triangleData_copy[TASK2_RESOLUTION] = {128,153,178,205,230,255,230,205,178,153,128,103,78,50,25,0,25,50,78,103};
int task2_realData_copy[TASK2_RESOLUTION] = {0};

int task2_sineData[TASK2_RESOLUTION] = {128,168,203,230,250,255,250,230,203,168,128,88,53,25,5,0,5,25,53,88};
int task2_triangleData[TASK2_RESOLUTION] = {128,153,178,205,230,255,230,205,178,153,128,103,78,50,25,0,25,50,78,103};
int task2_realData[TASK2_RESOLUTION] = {0};

int task2_freq_range[3] = {10,100,1000};;
int task2_freq_choice;
float task2_freq; 

float task2_ampl; // 0 to 2.5 . Not work at the moment
float task2_percent; // 0 to 100
float task2_voltage; // 0 to 5
int task2_old_knob_value; // Compare to new_ratio, if difference update display (to reduce delay of display)
int task2_data_pos;

int task2_max_adc_mode;// 1023 or 255 - 10 bit or 8 bit, depends on ADC

enum TASK2_MODES task2_mode = NORMAL;

int fixed_ta1_mode = OFF;

int task2_timer_a0_interrupt_cnt;
int task2_timer_a1_interrupt_cnt;

void task2_initialize(void)
{
	/* Parameters Initialization */
	task2_freq_choice = 0;
	task2_freq = task2_freq_range[task2_freq_choice];
	task2_ampl = 2.5;
	task2_percent = 0;
	task2_voltage = 0;
	task2_old_knob_value = -1;
	task2_data_pos = -1;
	task2_max_adc_mode = 1023;
	task2_mode = NORMAL;
	
	task2_timer_a0_interrupt_cnt = 0;
	task2_timer_a1_interrupt_cnt = 0;
	
	fixed_ta1_mode = OFF;
	
	/* Configure ADC and DAC */
	adc_configuration();
	dac_configuration();
	
	/* Configure timer and start game */
	timer_configuration();
}



void task2_timer_a0_interrupt(void)
{
	if (fixed_ta1_mode == ON)
	{
		if (++task2_timer_a0_interrupt_cnt > 2) // 2 x 100ms
		{
			int task2_knob_value;
			int *reg; 
		
			task2_timer_a0_interrupt_cnt = 0;

			/* Read ad0 value, display % and Volt to LCD, turn on LED */	
	
			// Get ad0 value		
			reg = (int *)&ad0; // 0 -> 255 in 8b mode or 0 -> 1023 in 16b mode
			task2_knob_value = *reg;
	

			if (task2_knob_value != task2_old_knob_value)
			{
				float task2_ratio;
				task2_old_knob_value = task2_knob_value;
				task2_ratio = (float)task2_knob_value / task2_max_adc_mode;
			
					
				/* Calculate percentage, voltage and frequency*/
				task2_percent =  task2_ratio * 100;	// 0-100%
				task2_voltage = task2_ratio * 5;		// 0-5V
		
				task2_freq = task2_freq_range[task2_freq_choice] + task2_ratio * (9*task2_freq_range[task2_freq_choice]); // min_freq + (max_freq - min_freq) x ratio		
		
		
		
				/* Update ta1 */
			
				/* pre-config, 20 sample case */
				if (task2_freq < 18750)
				{
					ta1 = 37500/task2_freq - 1; // 24,000,000 / 32 / TASK2_RESOLUTION = 37500
					ta1mr = 0x80;
				}
			
				else if (task2_freq < 75000)
				{
					ta1 = 150000/task2_freq - 1;
					ta1mr = 0x40;
				}
			
				/* Update display */
				task2_updateDisplay();
			
				/* Turn on LED */
				if (task2_percent < 25)
					p4 = 0xfe;
				else if (task2_percent < 50)
					p4 = 0xfc;
				else if (task2_percent < 75)
					p4 = 0xf8;
				else
					p4 = 0xf0;
			}
				
		}
	}
}

void task2_timer_a1_interrupt(void)
{
	/* Sample real signal, sampling frequency = showing frequency */	
	if (task2_mode == SINE_WAVE || task2_mode == TRIANGLE_WAVE || task2_mode == PLAYBACK) // DAC stuff
	{
		int task2_data = 0;
		
		if (task2_mode == SINE_WAVE)
		{
			task2_data = task2_sineData_copy[task2_data_pos];			
		}
		else if (task2_mode == TRIANGLE_WAVE)
		{
			task2_data = task2_triangleData_copy[task2_data_pos];
		}
		else
		{
			task2_data = task2_realData_copy[task2_data_pos]; // convert adc_mode to dac mode (10/8 bit to 8bit)
		}
		
		if (++task2_data_pos >= TASK2_RESOLUTION)
				task2_data_pos = 0;
		
		da0 = task2_data;
	}
	else if (task2_mode == RECORD) // ADC stuff
	{
		if (task2_data_pos > -1 && task2_data_pos < TASK2_RESOLUTION) // buffer available
		{
			int *reg;			
			// Get ad1 value

			reg = (int *)&ad1; // 0 -> 255 in 8b mode or 0 -> 1023 in 16b mode
			task2_realData[task2_data_pos] = *reg;
			++task2_data_pos;
				
			if(task2_data_pos == TASK2_RESOLUTION)
			{
				// Processing data after recording
				int i = 0;
				task2_updateDisplay();
				for (i = 0; i < TASK2_RESOLUTION; i++)
				{
					task2_realData[i] = task2_realData[i]/4;
					task2_realData_copy[i] = task2_realData[i]*(task2_ampl/(float)2.5);
				}
			}
		}
	}
}






void task2_sw1(void)
{
	if (++task2_mode > 4)  // Change mode
		task2_mode = NORMAL;
		
	if (task2_mode > 2) //  Reconfigure ADC when in AD0 and AD1
	{
		adc_configuration();
	}
	
	if (task2_mode == RECORD || task2_mode == PLAYBACK)
	{
		fixed_ta1_mode = ON;
		// Use fixed ta1
		ta1s = 0;
		ta1mr = 0x40; // f8
		// ta1 = 24,000,000/8 x 1/([samples per period] x [original frequency]) - 1
		ta1 = 149; 
	
		ta1s = 1;
	}
	else
	{
		// Turn on ADC, auto adjust ta1
		fixed_ta1_mode = OFF;
	}
	
	task2_data_pos = -1; // Reset buffer
	task2_updateDisplay();
}



void task2_sw2(void)
{
	if (task2_mode == RECORD) // Start recording / playing back
		task2_data_pos = 0;
	else if (task2_mode == SINE_WAVE || task2_mode == TRIANGLE_WAVE || task2_mode == PLAYBACK) // Increase amplitude
	{
		int i;
		if (++task2_ampl > 2.5)
			task2_ampl = 0;
			
		for (i = 0; i < TASK2_RESOLUTION; i++)
		{
			task2_sineData_copy[i] = task2_sineData[i]*(task2_ampl/(float)2.5);
			task2_triangleData_copy[i] = task2_triangleData[i]*(task2_ampl/(float)2.5);
			task2_realData_copy[i] = task2_realData[i]*(task2_ampl/(float)2.5);
		}
	}
}
void task2_sw3(void)
{
	if (task2_mode == SINE_WAVE || task2_mode == TRIANGLE_WAVE) // Change frequency range
	{
		if (++task2_freq_choice > 2)
			task2_freq_choice = 0;
	
		task2_old_knob_value = -1;
	} 
}