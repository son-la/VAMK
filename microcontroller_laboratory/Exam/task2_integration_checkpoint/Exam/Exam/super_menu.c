#include "super_menu.h"
#include "main.h"
#include "sfr62p.h"
#include "adc_stuff.h"
#include "timer_stuff.h"
#include "task_init.h"
#include "lcd_utils.h"


int superMenu_old_knob_value;
int superMenu_timer_a0_interrupt_cnt;

void superMenu_initialize(void)
{
	/* Parameter Initlaization */
	superMenu_old_knob_value = -1;
	super_menu_choice = TASK1;
	adc_configuration();
	timer_configuration();
	
	superMenu_timer_a0_interrupt_cnt = 0;
}

void superMenu_timer_a0_interrupt(void)
{
	if (++superMenu_timer_a0_interrupt_cnt > 3)
	{
		// Interval is already 100ms (timer_stuff)
		int superMenu_knob_val = 0;
		int *reg; 
		
		superMenu_timer_a0_interrupt_cnt = 0;
		
		// Get ad0 value		
		reg = (int *)&ad0; // 0 -> 255 in 8b mode
		superMenu_knob_val = *reg;

		if (superMenu_knob_val != superMenu_old_knob_value)
		{
			superMenu_old_knob_value = superMenu_knob_val;
		
			/* Scroll position */
			if (superMenu_knob_val < 51)
				super_menu_choice = TASK1;
			else if (superMenu_knob_val < 102)
				super_menu_choice = TASK2;
			else if (superMenu_knob_val < 153)
				super_menu_choice = TASK3;
			else if (superMenu_knob_val < 204)
				super_menu_choice = TASK4;
			else 
				super_menu_choice = TASK5;
		}			
	
	
		// Update display, can put inside function for efficiency
		superMenu_lcdDisplay(); 
	}
}


void superMenu_sw1(void)
{
	// Scroll down
	if (++super_menu_choice > TASK5)
		super_menu_choice = TASK1;
}

void superMenu_sw2(void)
{
	// Scroll up
	if (--super_menu_choice < TASK1)
		super_menu_choice = TASK5;
}

void superMenu_sw3(void)
{
	// Select task
	super_menu = super_menu_choice;
	taskInitialize();
}