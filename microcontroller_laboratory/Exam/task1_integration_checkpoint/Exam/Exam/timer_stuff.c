#include "timer_stuff.h"
#include "main.h"
#include "sfr62p.h"
#include "lcd_utils.h"
#include "task_init.h"
#include "switch_function.h"
#include "task1.h"

#pragma INTERRUPT timer_a0_interrupt
#pragma INTERRUPT timer_a1_interrupt
#pragma INTERRUPT timer_a2_interrupt


void timer_configuration(void)
{
	switch (super_menu)
	{
		case TOP_MENU:
		{
			/* Timer for ADC - 10ms */
			ta0mr = 0x40;
			ta0 = 29999;
			
			_asm(" FCLR I");
			ta0ic = 0x01;
			_asm("FSET I");
			
			ta0s = 1;
			break;	
		}
		case TASK1:
		{
			/* Timer for SW3 holds, SW1 holds and gameOver() - 10ms*/
			ta0mr = 0x40;
			ta0 = 29999;
			/* Timer for playGame() - 10ms*/
			ta1mr = 0x40;
			ta1 = 29999;
			
			_asm(" FCLR I");
			ta0ic = 0x01;
			ta0ic = 0x01;
			_asm("FSET I");
			
			ta0s = 1;
			ta1s = 1;
			break;	
		}
	}
}

int old_knob_val;
int timer_a0_cnt = 0;
int sw3_holds = 0, task1_sw1_holds = 0;
void timer_a0_interrupt(void)
{
	if (++timer_a0_cnt == 10)
	{
		timer_a0_cnt = 0;
		/* Debouncing */
		if (sw_press > 0) --sw_press;
		
		
		if (super_menu == TOP_MENU)
		{
			int knob_val;
			int *reg; 
			// Get ad0 value		
			reg = (int *)&ad0; // 0 -> 255 in 8b mode
			knob_val = *reg;
		
			if (knob_val != old_knob_val)
			{
				old_knob_val = knob_val;
				if (knob_val < 20)
					super_menu_choice = TASK1;
				else if (knob_val < 40)
					super_menu_choice = TASK2;
				else if (knob_val < 60)
					super_menu_choice = TASK3;
				else if (knob_val < 80)
					super_menu_choice = TASK4;
				else 
					super_menu_choice = TASK5;
			}	
			superMenu_lcdDisplay();		
		}// End TOP_MENU
		else
		{
			/* Check sw3 holds */	
			if (p8_4 == 0)
			{
				if (++sw3_holds == 300)
				{
					super_menu_choice = TOP_MENU;
					taskInitialize();
				}
			}
			else
				sw3_holds = 0;
		
			if (super_menu == TASK1)
			{
				/* Check sw1 holds */
				if (p8_2 == 0)
				{
					if (++task1_sw1_holds == 300)
					{
						/* TASK1: change game mode */
						task1_changeGameMode();
					}
				}
				else
					task1_sw1_holds = 0;
				
				task1_checkGameOver();
			}
		}
	}// End code-executing counter
}

void timer_a1_interrupt(void)
{
	if (super_menu == TASK1)
	{
		task1_playGame();
	}
}

void timer_a2_interrupt(void)
{
	
}