#include "task_init.h"
#include "main.h"
#include "switch_function.h"
#include "lcd.h"
#include "sfr62p.h"
#include "timer_stuff.h"

/* TASK initlaization and implementation */
#include "super_menu.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "task5.h"

void onceInitialize(void)
{
	/* Variables initlaizeation */
	super_menu = TOP_MENU;
	super_menu_choice = TASK1;
	switch_initialize();
	InitialiseDisplay();
}

void taskInitialize(void)
{
	/* STOP all timers */
	ta0s = 0;
	ta1s = 0;
	ta2s = 0;
	
	/* Clear screen */
	LCD_write(CTRL_WR,LCD_CLEAR);
	
	/* STOP ADC and restart ratio variable*/
	old_knob_val = -1;
	adst = 0;
	

	
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