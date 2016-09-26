#include "sfr62p.h"
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "utils.h"

/* Mode operation */
#define SINEWAVE 0
#define PWM 1


#define RESOLUTION 20 // Stored samples
#define BOUNCING_CNT 3 // 3 x 100 = 300ms between each press


#pragma INTERRUPT timer_a0_interrupt // PWM/DAC sinewave
#pragma INTERRUPT timer_a1_interrupt // ADC, update values , display, bouncing handler
#pragma INTERRUPT timer_a2_interrupt // Blinking

#pragma INTERRUPT sw1; // Change mode
#pragma INTERRUPT sw2; // Increase
#pragma INTERRUPT sw3; // Decrease

void update_copyarray(void);
void sine_config(void);
void PWM_config(void);
void DAC_config(void);
void ADC_config(void);
void update_dc_fq(void);

enum MODES mode = NORMAL; // Steady display
int task = PWM;


int sineData[RESOLUTION] = {51,67,81,92,100,102,100,92,81,67,51,35,21,10,2,0,2,10,21,35};
int copySineData[RESOLUTION] = {128,168,203,230,250,255,250,230,203,168,128,88,53,25,5,0,5,25,53,88};

int fq_range_min[] = {10, 100, 1000}; // range_max = range_min x 10;
int dc_range_min[] = {0, 25, 50, 75}; // range_max = range_min + 25;

int fq_choice = 0, dc_choice = 2; // fq:0->2, dc:0->3
int fq_val = 0, dc_val = 0;
float ampl = 2.5; // 0 to 2.5
long n, m;
float ratio = 0;
int old_knob_val = -1;
int init = 1;

int sw_pressed = 0; // 0: pressable
void sw1(void)
{
	if (sw_pressed == 0)
	{
		sw_pressed = BOUNCING_CNT;
		
		// Switch mode
		if (++mode > 2)
		{
			mode = NORMAL;
		}
	
		if (mode > 0)
			ta2s = 1; // During setting mode, Turn-on auto update display
		else
		{
			ta2s = 0; // else turn off
			lcd_display_function(mode, dc_val, fq_choice);
		}
	}
}

void sw2(void)
{
	if (sw_pressed == 0)
	{
		sw_pressed = BOUNCING_CNT;
		// Increase
		switch (mode)
		{
			case SET_FQ:
			{
				if (++fq_choice > 2)
					fq_choice = 0;
				old_knob_val = -1; // Reset DQ,FQ val and reconfigure output
				break;
			}
			case SET_DC:
			{
				if (++dc_choice > 3)
					dc_choice = 0;
				old_knob_val = -1; // Reset DQ,FQ val and reconfigure output
				break;			
			}
			case NORMAL:
			{
				if (ampl == 2.5)
					return;
				ampl += 0.1;
				update_copyarray();
				break;
			}
		}
		
		
		/*
		if (task == PWM)
			PWM_config();
		else
			sine_config();
		*/
	}
}

void sw3(void)
{
	if (sw_pressed == 0)
	{
		// Decrease
		switch (mode)
		{
			case SET_FQ:
			{
				if (--fq_choice < 0)
					fq_choice = 2;
				old_knob_val = -1; // Reset DQ,FQ val and reconfigure output
				break;
			}
			case SET_DC:
			{
				if (--dc_choice < 0)
					dc_choice = 3;
				old_knob_val = -1; // Reset DQ,FQ val and reconfigure output
				break;			
			}
			case NORMAL:
			{
				if (ampl == 0)
					return;
				ampl -= 0.1;
				update_copyarray();
				break;
			}
		}
		
		/*
		if (task == PWM)
			PWM_config();
		else
			sine_config();
		*/
	}
}


/* LCD blinking when set FQ and DC */
int timer_a2_interrupt_cnt = 0;
int tiktik = 1;
void timer_a2_interrupt(void) 
{
	if (++timer_a2_interrupt_cnt > 50)
	{
		if (tiktik == 1)
			p4 = 0;
		else
			p4 = 0xff;
		tiktik = -tiktik;
		
		timer_a2_interrupt_cnt = 0;
		lcd_display_function(mode, dc_val, fq_choice);
	}	
}


/* ADC */
int timer_a1_interrupt_cnt = 0;
void timer_a1_interrupt(void) 
{
	if (++timer_a1_interrupt_cnt > 10)
	{
		int knob_val;
		int *reg; 	
	
		timer_a1_interrupt_cnt = 0;
		
		// Debouncing
		if (sw_pressed > 0)
			--sw_pressed;
		
		
		reg = (int *)&ad0;
		knob_val = *reg;
	
		
		
		if (knob_val != old_knob_val)
		{
			old_knob_val = knob_val;
			
			ratio = (float)knob_val / 255;
			
			update_dc_fq();
			
			// Frequency change -> reconfigure PWM or ta0
			
			if (task == PWM)
				PWM_config();
			else
				sine_config();
			
			
			lcd_display_function(mode, dc_val, fq_choice);
		}
	}
}


/* DAC */
int data_pos = 0;
void timer_a0_interrupt(void)
{
	if (task == SINEWAVE)
	{
		da0 = copySineData[data_pos];			
		if (++data_pos >= RESOLUTION)
			data_pos = 0;
	}

}






/**********************************************************************************
Function Name: 	Main
Description:	Main function
Parameters: 	none
Return value: 	none
***********************************************************************************/
void main(void)				
{
	/* Display */
	InitialiseDisplay();
	
	/* ADC configuration*/
	ADC_config(); 
		
	
	/* Configure timers*/
	
	// For ADC
	ta1mr = 0x40;
	ta1 = 29999; // 10ms
	
	// For Blinking 
	ta2mr = 0x40;
	ta2 = 29999; // 10ms 
	
	
	/* Set priority */
	DISABLE_IRQ	
	ta2ic = 0x01;
	ta1ic = 0x01;
	ta0ic = 0x01;
	
	int0ic = 0x02;
	int1ic = 0x02;
	int2ic = 0x02;
	ENABLE_IRQ
	
	
	
	// Start ADC to get ratio value 
	ta1s = 1;
	
	old_knob_val = -1;
	
	/* One of those, config timera0 */
	// DAC or PWM
	if (task == PWM)
		PWM_config(); // Config PWM in case square_wave
	else
	{
		DAC_config(); // Config DAC in case sine_wave
		sine_config();
	}
	// Start generating output
	ta0s = 1;
	
	
	
	
	update_dc_fq();
	lcd_display_function(mode, dc_val, fq_choice);
	
	init = 0;
	while(1);
}         



/************************ Duty cycle and FQ update *************************/
void update_dc_fq(void)
{
	/* DC */
	if (mode == SET_DC)
		dc_val = dc_range_min[dc_choice] + (float)25*ratio;
	else if (mode == SET_FQ)
		/* FQ */
		fq_val = fq_range_min[fq_choice] + (float)fq_range_min[fq_choice] * 9.0 * ratio; // fq_range_min + (fq_range_max - fq_range_min) x ratio
	if (init == 1)
	{
		dc_val = dc_range_min[dc_choice] + (float)25*ratio;
		fq_val = fq_range_min[fq_choice] + (float)fq_range_min[fq_choice] * 9.0 * ratio; // fq_range_min + (fq_range_max - fq_range_min) x ratio
	}
}


/**************** ADC and DAC configuration **********************/
void DAC_config(void)
{
	da0e = 0; // Turn off DAC
	prc2 = 1; // Turn off protect bit
	pd9_3 = 0; // P9_3 as input
	da0 = 0; 
	da0e = 1; // Enable DAC
}

void ADC_config(void)
{
	adst = 0; // Turn off ADC
	adcon0 = 0x08; // AD0, Sw trigger
	adcon1 = 0x20; // Vref connected, 8bit mode
	adcon2 = 0x00; // No sample and hold
	adst = 1; // Enable ADC	
}





/**************** PWM configuration and helpers ******************/

void update_n(void) // As its name
{
	float fn = dc_val/(float)100.0 * (float)255.0;	
	fn += (fn - (long)fn >= 0.5)?1:0;
	n = (long)fn;
}

int update_m(void) // As its name
{
	// Update m according to fq_val, return f_j choice: 1,8,32 or 0 if cannot find m
	int f_j = 32; // Frequency config value
	float fm  = ((float)24000000/fq_val)/255 -1; //94117.7/(float)fq_val - 1;  //((float)24000000/fq_val)/255 -1;
	if (fm > 255)
	{
		fm  = ((float)24000000/fq_val/8)/255 -1;//11764.7/(float)fq_val - 1; //((float)24000000/fq_val/8)/255 -1;
		if (fm > 255)
		{
			fm  = ((float)24000000/fq_val/32)/255 -1;//2491.2/(float)fq_val -1; //((float)24000000/fq_val/32)/255 -1;
			if (fm > 255)
				fm = n; // No output
			f_j = 32; // f32
		}
		else
			f_j = 8; // f8
	}
	else
		f_j = 1; // f1
	
	fm += (fm - (int)fm >= 0.5)?1:0;
	m = (int)fm;
	
	return f_j;	
}


void PWM_config(void)
{
	int f_j;
	ta0s = 0;
	update_n();
	f_j = update_m();
	//n = 127;
	//m = 57;
	//f_j = 8;
	
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
		case 32:
		{
			ta0mr = 0xa7; // f32
			break;	
		}
	}
	
	ta0 = 256*n + m;
	ta0s = 1;	
}


/********************** Sine wave configuration *****************/

void sine_config(void)
{
	ta0s = 0;
	// TimerA1 starts firstt then when this function call, fq_val is available
	if (fq_val < 18750)
	{
		ta0mr = 0x80;
		ta0 = 37500/fq_val - 1; // 24,000,000 / 32 / RESOLUTION = 37500
	}
	else 
	{
		ta0mr = 0x40;
		ta0 = 150000/fq_val - 1;
	}	
	ta0s = 1;
}

void update_copyarray(void)
{
	int i;
	for (i = 0; i < RESOLUTION; i++)
	{
		copySineData[i] = sineData[i] * ampl;
	}	
}