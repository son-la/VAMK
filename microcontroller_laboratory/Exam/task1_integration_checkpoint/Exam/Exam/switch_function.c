#include "switch_function.h"
#include "sfr62p.h"
#include "task_init.h"
#include "main.h"

#include "task1.h"

#pragma INTERRUPT sw1
#pragma INTERRUPT sw2
#pragma INTERRUPT sw3

int sw_press = 0;

void switch_initialize(void)
{
	_asm(" FCLR I");
	int0ic = 0x02; // Set priority
	int1ic = 0x02; // Set priority
	int2ic = 0x02; // Set priority
	_asm("FSET I");
}

void sw1(void)
{
	if (sw_press == 0)
	{
		sw_press = 2;	
	
		switch (super_menu)
		{
			case TOP_MENU:
			{
				if (++super_menu_choice > TASK5)
					super_menu_choice = TASK1;
				break;
			}
			case TASK1:
			{
				task1_sw1();
				break;	
			}
		}
	}
}

void sw2(void)
{
	if (sw_press == 0)
	{
		sw_press = 2;
		
		switch (super_menu)
		{
			case TOP_MENU:
			{
				if (--super_menu_choice < TASK1)
					super_menu_choice = TASK5;
				break;
			}
			case TASK1:
			{
				task1_sw2();
				break;	
			}
		}
	}
}

void sw3(void)
{
	if (sw_press == 0)
	{
		sw_press = 2;
		
		switch (super_menu)
		{
			case TOP_MENU:
			{
				super_menu = super_menu_choice;
				taskInitialize();
				break;
			}
			case TASK1:
			{
				task1_changeGameSpeed();
				break;	
			}
		}
	}
}