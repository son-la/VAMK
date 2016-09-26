#include "sfr62p.h"
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "lcd_utils.h"

#define SW_DEBOUNCE 8 // 20ms x 8 = 160ms
#define MIN 0x01 // 0001
#define MAX 0x08 // 1000

#define MIN_MAX 1 // LED0 to LED3
#define MAX_MIN 0 // LED3 to LED0


#pragma INTERRUPT timer_a0_interrupt
#pragma INTERRUPT sw1
#pragma INTERRUPT sw2
#pragma INTERRUPT sw3

// Task 1 parameters
unsigned char preset_data = 0xFE;// 1110 - LED0 on
int dir = MIN_MAX; // LED0 to LED3

// Task2 parameters
float th[3] = {1.0, 3.0, 13.0};
float tp[3] = {5.0, 10.0, 17.0};
int pwm_choice = 0;

void PWM_config(void);
void update_n(void);
int update_m(void);


int sw_press = 0;
void sw1(void)
{
	if (sw_press == 0)
	{
		// Change led direction
		if (dir == MIN_MAX)
			dir = MAX_MIN;
		else
			dir = MIN_MAX;
			
		sw_press = SW_DEBOUNCE;	
	}	
}

void sw2(void)
{
	if (sw_press == 0)
	{
		if (++pwm_choice > 2)
			pwm_choice = 0;
		
		PWM_config();
		sw_press = SW_DEBOUNCE;	
	}	
}

void sw3(void)
{
	if (sw_press == 0)
	{
		if (--pwm_choice < 0)
			pwm_choice = 2;
		
		PWM_config();
		sw_press = SW_DEBOUNCE;	
	}	
}

int timer_a0_interrupt_cnt = 0;
void timer_a0_interrupt(void)
{
	if (++timer_a0_interrupt_cnt > 100) // 100ms
	{
		//if (sw_press > 0) --sw_press;
		
		timer_a0_interrupt_cnt = 0;	
		
		
		p4 = preset_data;
		
		
		// Inverse for processing 
		preset_data = ~preset_data;
		
		
		if (dir == MAX_MIN && preset_data == MIN) 
		{
			preset_data = ~MAX;	
			return;	
		}
		else if (dir == MIN_MAX && preset_data == MAX)
		{
			preset_data = ~MIN;		
			return;	
		}
		else
		{
			if (dir == MIN_MAX)
				preset_data = preset_data << 1;
			else 
				preset_data = preset_data >> 1;
	
			preset_data = ~preset_data;	
		}
	}
	
	if (sw_press > 0 && timer_a0_interrupt_cnt%2 > 0) --sw_press;
}

void main(void)				
{
	InitialiseDisplay();
	
	/* TimerA0 - LED - SW debouncing */
	ta0mr = 0x40;
	ta0 = 29999;
	
	/* Priority level */
	DISABLE_IRQ	
	ta0ic = 0x01;
	ta4ic = 0x02;
	
	int0ic = 0x03;
	int1ic = 0x03;
	int2ic = 0x03;
	ENABLE_IRQ
	
	ta0s = 1;
	
	/* PWM config */
	PWM_config();
	
	
	while(1);
}         


/**************** PWM configuration and helpers ******************/
int n,m;

void update_n(void) // As its name
{
	float float_n = th[pwm_choice]/tp[pwm_choice] * (float)255.0;	
	float_n += (float_n - (int)float_n >= 0.5)?1:0;
	n = (int)float_n;
}

int update_m(void) // As its name
{
	// Update m according to fq_val, return f_j choice: 1,8,32 or 0 if cannot find m
	int f_j = 32; // Frequency config value
	float fm  = ((float)24000000*tp[pwm_choice]/1000)/255 -1;
	if (fm > 255)
	{
		fm  = ((float)24000000*tp[pwm_choice]/1000/8)/255 -1;
		if (fm > 255)
		{
			fm  = ((float)24000000*tp[pwm_choice]/1000/32)/255 -1;
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
	
	update_n();
	f_j = update_m();
	
	switch (f_j)
	{
		case 1:
		{
			updateLCD(n,m, 24000000);	
			ta4mr = 0x27; // f1
			break;
		}
		case 8:
		{
			updateLCD(n,m, 3000000);	
			ta4mr = 0x67; // f8
			break;
		}
		case 32:
		{
			updateLCD(n,m, 750000);	
			ta4mr = 0xa7; // f32
			break;	
		}
	}
	
	ta4 = 256*n + m;
	ta4s = 1;
	
}

