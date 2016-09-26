#include "task3.h"
#include "lcd_utils.h"



/* Mode operation */
#define TASK3_SINEWAVE 0
#define TASK3_PWM 1


#define TASK3_RESOLUTION 20 // Stored samples

enum TASK3_MODES task3_mode; // Steady display
int task3_task;

int task3_sineData[TASK3_RESOLUTION] = {128,168,203,230,250,255,250,230,203,168,128,88,53,25,5,0,5,25,53,88};
//int task3_sineData[TASK3_RESOLUTION] = {51,67,81,92,100,102,100,92,81,67,51,35,21,10,2,0,2,10,21,35};
int task3_copySineData[TASK3_RESOLUTION] = {128,168,203,230,250,255,250,230,203,168,128,88,53,25,5,0,5,25,53,88};


int task3_fq_range_min[] = {10, 100, 1000}; // range_max = range_min x 10;
int task3_dc_range_min[] = {0, 25, 50, 75}; // range_max = range_min + 25;

int task3_fq_choice, task3_dc_choice; // fq:0->2, dc:0->3
int task3_fq_val, task3_dc_val;
float task3_ampl;
long task3_n, task3_m;
float task3_ratio;
int task3_old_knob_val;
int task3_init; // Initializing

int task3_data_pos;
int task3_timer_a0_interrupt_cnt;
int task3_timer_a2_interrupt_cnt;

void task3_initialize(void)
{
	/* Parameters Initlaization */
	task3_mode = TASK3_NORMAL;
	task3_task = TASK3_PWM;
	task3_fq_choice = 0; task3_dc_choice = 2;
	task3_fq_val = 0; task3_dc_val = 0;
	task3_ampl = 2.5;
	task3_old_knob_val = -1;
	task3_init = 1;
	
	task3_data_pos = 0;
	
	task3_timer_a0_interrupt_cnt = 0;
	task3_timer_a2_interrupt_cnt = 0;
	
	/* ADC configuration */
	adc_configuration();
	
	/* Configure timers */
	// For ADC
	ta0mr = 0x40;
	ta0 = 29999; // 10ms
	
	// For Blinking 
	ta2mr = 0x40;
	ta2 = 29999; // 10ms 
	
	
	/* Set priority */
	_asm(" FCLR I");
	ta2ic = 0x01;
	ta1ic = 0x01;
	ta0ic = 0x01;
	_asm("FSET I");
	
	
	
	// Start ADC to get ratio value 
	ta0s = 1;
	
	task3_old_knob_val = -1;
	
	/* One of those, config timera0 */
	// DAC or PWM
	if (task3_task == TASK3_PWM)
		task3_PWMConfig(); // Config PWM in case square_wave
	else
	{
		dac_configuration(); // Config DAC in case sine_wave
		task3_sineConfig();
	}
	
	
	// Start generating output
	ta1s = 1;
	
	
	
	// Manually update DC and FQ, then print out
	task3_update_dc_fq();
	task3_LCDDisplayFunction(task3_mode, task3_dc_val, task3_fq_choice);
	
	task3_init = 0;
}

void task3_sw1(void)
{
	// Switch mode
	if (++task3_mode > 2)
	{
		task3_mode = TASK3_NORMAL;
	}

	if (task3_mode > 0)
		ta2s = 1; // During setting mode, Turn-on auto update display
	else
	{
		ta2s = 0; // else turn off
		task3_LCDDisplayFunction(task3_mode, task3_dc_val, task3_fq_choice);
	}
}


void task3_sw2(void)
{
	// Increase
	switch (task3_mode)
	{
		case TASK3_SET_FQ:
		{
			if (++task3_fq_choice > 2)
				task3_fq_choice = 0;
			task3_old_knob_val = -1; // Reset DQ,FQ val and reconfigure output
			break;
		}
		case TASK3_SET_DC:
		{
			if (++task3_dc_choice > 3)
				task3_dc_choice = 0;
			task3_old_knob_val = -1; // Reset DQ,FQ val and reconfigure output
			break;			
		}
		case TASK3_NORMAL:
		{
			if (task3_ampl == 2.5)
				return;
			task3_ampl += 0.1;
			task3_updateCopyarray();
			break;
		}
	}
}


void task3_sw3(void)
{
	// Decrease
	switch (task3_mode)
	{
		case TASK3_SET_FQ:
		{
			if (--task3_fq_choice < 0)
				task3_fq_choice = 2;
			task3_old_knob_val = -1; // Reset DQ,FQ val and reconfigure output
			break;
		}
		case TASK3_SET_DC:
		{
			if (--task3_dc_choice < 0)
				task3_dc_choice = 3;
			task3_old_knob_val = -1; // Reset DQ,FQ val and reconfigure output
			break;			
		}
		case TASK3_NORMAL:
		{
			if (task3_ampl == 0)
				return;
			task3_ampl -= 0.1;
			task3_updateCopyarray();
			break;
		}
	}
}


void task3_timer_a1_interrupt(void)
{
	if (task3_task == TASK3_SINEWAVE)
	{
		da0 = task3_copySineData[task3_data_pos];			
		if (++task3_data_pos >= TASK3_RESOLUTION)
			task3_data_pos = 0;
	}

}



void task3_timer_a0_interrupt(void)
{
	if (++task3_timer_a0_interrupt_cnt > 2) // 100ms x 2 = 200ms
	{
		int task3_knob_val;
		int *task3_reg; 	
	
		task3_timer_a0_interrupt_cnt = 0;
		
		
		task3_reg = (int *)&ad0;
		task3_knob_val = *task3_reg;
	
		
		
		if (task3_knob_val != task3_old_knob_val)
		{
			task3_old_knob_val = task3_knob_val;
			
			task3_ratio = (float)task3_knob_val / (float)255.0;
			
			task3_update_dc_fq();
			
			// Frequency change -> reconfigure PWM or ta0
			
			if (task3_task == TASK3_PWM)
				task3_PWMConfig();
			else
				task3_sineConfig();
			
			
			task3_LCDDisplayFunction(task3_mode, task3_dc_val, task3_fq_choice);
		}
	}
}



void task3_timer_a2_interrupt(void) 
{
	if (++task3_timer_a2_interrupt_cnt > 50)
	{		
		task3_timer_a2_interrupt_cnt = 0;
		task3_LCDDisplayFunction(task3_mode, task3_dc_val, task3_fq_choice);
	}	
}

/************************ Duty cycle and FQ update *************************/
void task3_update_dc_fq(void)
{
	/* DC */
	if (task3_mode == TASK3_SET_DC)
		task3_dc_val = task3_dc_range_min[task3_dc_choice] + (float)25*task3_ratio;
	else if (task3_mode == TASK3_SET_FQ)
		/* FQ */
		task3_fq_val = task3_fq_range_min[task3_fq_choice] + (float)task3_fq_range_min[task3_fq_choice] * 9.0 * task3_ratio; // fq_range_min + (fq_range_max - fq_range_min) x ratio
		
	if (task3_init == 1)
	{
		task3_dc_val = task3_dc_range_min[task3_dc_choice] + (float)25*task3_ratio;
		task3_fq_val = task3_fq_range_min[task3_fq_choice] + (float)task3_fq_range_min[task3_fq_choice] * 9.0 * task3_ratio; // fq_range_min + (fq_range_max - fq_range_min) x ratio
	}
}




/* PWM configuration */

void task3_update_n(void) // As its name
{
	float task3_fn = task3_dc_val/(float)100.0 * (float)255.0;	
	task3_fn += (task3_fn - (long)task3_fn >= 0.5)?1:0;
	task3_n = (long)task3_fn;
}

int task3_update_m(void) // As its name
{
	// Update m according to fq_val, return f_j choice: 1,8,32 or 0 if cannot find m
	int task3_f_j = 32; // Frequency config value
	float task3_fm  = ((float)24000000/task3_fq_val)/255 -1; //94117.7/(float)fq_val - 1;  //((float)24000000/fq_val)/255 -1;
	if (task3_fm > 255)
	{
		task3_fm  = ((float)24000000/task3_fq_val/8)/255 -1;//11764.7/(float)fq_val - 1; //((float)24000000/fq_val/8)/255 -1;
		if (task3_fm > 255)
		{
			task3_fm  = ((float)24000000/task3_fq_val/32)/255 -1;//2491.2/(float)fq_val -1; //((float)24000000/fq_val/32)/255 -1;
			if (task3_fm > 255)
				task3_fm = task3_n; // No output
			task3_f_j = 32; // f32
		}
		else
			task3_f_j = 8; // f8
	}
	else
		task3_f_j = 1; // f1
	
	task3_fm += (task3_fm - (int)task3_fm >= 0.5)?1:0;
	task3_m = (int)task3_fm;
	
	return task3_f_j;	
}

void task3_PWMConfig(void)
{
	int task3_f_j;
	ta1s = 0;
	task3_update_n();
	task3_f_j = task3_update_m();
	
	switch (task3_f_j)
	{
		case 1:
		{
			ta1mr = 0x27; // f1
			break;
		}
		case 8:
		{
			ta1mr = 0x67; // f8
			break;
		}
		case 32:
		{
			ta1mr = 0xa7; // f32
			break;	
		}
	}
	
	ta1 = 256*task3_n + task3_m;
	ta1s = 1;	
}

/********************** Sine wave configuration *****************/

void task3_sineConfig(void)
{
	ta1s = 0;
	// TimerA1 starts firstt then when this function call, fq_val is available
	if (task3_fq_val < 18750)
	{
		ta1mr = 0x80;
		ta1 = 37500/task3_fq_val - 1; // 24,000,000 / 32 / RESOLUTION = 37500
	}
	
	else 
	{
		ta1mr = 0x40;
		ta1 = 150000/task3_fq_val - 1;
	}	
	ta1s = 1;
}

void task3_updateCopyarray(void)
{
	int i;
	for (i = 0; i < TASK3_RESOLUTION; i++)
	{
		task3_copySineData[i] = task3_sineData[i] * task3_ampl;
	}	
}