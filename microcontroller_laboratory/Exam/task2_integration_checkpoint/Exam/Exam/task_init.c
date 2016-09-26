#include "task_init.h"
#include "main.h"
#include "sfr62p.h"
/* Clear Screen */
#include "lcd.h"
/* Switch initialization */
#include "switch_function.h"
/* TASK initlaization and implementation */
#include "super_menu.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "task5.h"

void onceInitialize(void)
{
	/* Variables initlaization */	
	super_menu = TOP_MENU;
	super_menu_choice = TASK1;
	switch_initialize();
	InitialiseDisplay();
	
	/* Configure sw3 as input */
	pd8_4 = 0;
	
	/* Knob as input */
	pd10 = 0x00;
}

void taskInitialize(void)
{
	/* STOP all timers*/
	ta0s = 0;
	ta1s = 0;
	ta2s = 0;
	
	/* STOP DAC */
	da0e = 0;
	
	
	/* STOP ADC*/
	adst = 0;
	
	
	/* Clear screen */
	LCD_write(CTRL_WR,LCD_CLEAR);

	/* Clear switch debounce */
	debounce_swPress = 0;
	
	/* Initlaize with specific task */
	switch (super_menu)
	{
		case TOP_MENU:	
		{
			superMenu_initialize();
			break;	
		}
		case TASK1:
		{
			task1_initialize();
			break;	
		}
		case TASK2:
		{
			task2_initialize();
			break;	
		}
		case TASK3:
		{
			task3_initialize();
			break;	
		}
		case TASK4:
		{
			task4_initialize();
			break;	
		}
		case TASK5:
		{
			task5_initialize();
			break;	
		}
	}	
}