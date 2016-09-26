#include "switch_function.h"
#include "sfr62p.h"
#include "main.h"

#include "super_menu.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "task5.h"

int debounce_swPress = 0;

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
	if (debounce_swPress == 0)
	{
		debounce_swPress = 2;	
	
		switch (super_menu)
		{
			case TOP_MENU:
			{				
				superMenu_sw1();
				break;
			}
			case TASK1:
			{
				task1_sw1();
				break;	
			}
			case TASK2:
			{
				task2_sw1();
				break;	
			}
			case TASK3:
			{
				task3_sw1();
				break;	
			}
			case TASK4:
			{
				task4_sw1();
				break;	
			}
			case TASK5:
			{
				task5_sw1();
				break;	
			}
		}
	}
}

void sw2(void)
{
	if (debounce_swPress == 0)
	{
		debounce_swPress = 2;
		
		switch (super_menu)
		{
			case TOP_MENU:
			{
				superMenu_sw2();
				break;
			}
			case TASK1:
			{
				task1_sw2();
				break;	
			}
			case TASK2:
			{
				task2_sw2();
				break;	
			}
			case TASK3:
			{
				task3_sw2();
				break;	
			}
			case TASK4:
			{
				task4_sw2();
				break;	
			}
			case TASK5:
			{
				task5_sw2();
				break;	
			}
		}
	}
}

void sw3(void)
{
	if (debounce_swPress == 0)
	{
		debounce_swPress = 2;
		
		switch (super_menu)
		{
			case TOP_MENU:
			{
				superMenu_sw3();
				break;
			}
			case TASK1:
			{
				task1_sw3();
				break;	
			}
			case TASK2:
			{
				task2_sw3();	
			}
			case TASK3:
			{
				task3_sw3();
				break;	
			}
			case TASK4:
			{
				task4_sw3();
				break;	
			}
			case TASK5:
			{
				task5_sw3();
				break;	
			}
		}
	}
}