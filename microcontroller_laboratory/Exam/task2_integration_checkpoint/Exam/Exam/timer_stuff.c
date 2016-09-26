#include "timer_stuff.h"


void timer_configuration(void)
{
	switch (super_menu)
	{
		case TOP_MENU:
		{
			/* Timer for ADC - 10ms */
			ta0mr = 0x40;
			ta0 = 29999;
			
			/* Set interrupt level */
			_asm(" FCLR I");
			ta0ic = 0x01;
			_asm("FSET I");
			
			/* Start timer */
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
			ta1ic = 0x01;
			_asm("FSET I");
			
			ta0s = 1;
			ta1s = 1;
			break;	
		}
		case TASK2:
		{
			/* Configure timer a0 - 10ms*/
			ta0mr = 0x40; // f8
			ta0 = 29999; // 10ms
	
			/* Configure timer a1*/
			ta1mr = 0x40;
			ta1 = 29;
			
			_asm(" FCLR I");
			ta0ic = 0x01;
			ta1ic = 0x01;
			_asm("FSET I");
			
			
			/* Start timers */
			ta0s = 1;
			ta1s = 1;
			break;
		}
		case TASK4:
		{
			/* Configure timer a0 - 10ms*/
			ta0mr = 0x40; // f8
			ta0 = 29999; // 10ms	
			
			_asm(" FCLR I");
			ta0ic = 0x01;
			_asm("FSET I");
			
			/* Start timers */
			ta0s = 1;
			
			break;
		}
		case TASK5:
		{
			/* Configure timer a0 - 10ms*/
			ta0mr = 0x40; // f8
			ta0 = 29999; // 10ms	
			
			_asm(" FCLR I");
			ta0ic = 0x01;
			_asm("FSET I");
			
			/* Start timers */
			ta0s = 1;
			
			break;
		}
	}
}

int old_knob_val;
int timer_a0_cnt = 0;
int sw3_holds = 0;
void timer_a0_interrupt(void)
{
	if (++timer_a0_cnt > 9) // This make everytask timer a0 interval is 100ms
	{
		timer_a0_cnt = 0;
		/* Debouncing */
		if (debounce_swPress > 0) --debounce_swPress;
		
		
		if (super_menu == TOP_MENU)
		{
			superMenu_timer_a0_interrupt();
		}
		else
		{
			/* Check sw3 holds to exit*/	
			if (p8_4 == 0)
			{
				if (++sw3_holds > 29)
				{
					sw3_holds = 0;
					super_menu = TOP_MENU;
					taskInitialize();
				}
			}
			else
			{
				sw3_holds = 0;
				
				switch (super_menu)
				{
					case TASK1:
					{
						task1_timer_a0_interrupt();
						break;
					}
					case TASK2:
					{
						task2_timer_a0_interrupt();	
						break;
					}
					case TASK3:
					{
						task3_timer_a0_interrupt();
						break;	
					}
				}
			}
		}
	}// End code-executing counter
}

void timer_a1_interrupt(void)
{
	switch (super_menu)
	{
		case TASK1:
		{
			task1_timer_a1_interrupt();
			break;
		}
		case TASK2:
		{
			task2_timer_a1_interrupt();
			break;
		}
		case TASK3:
		{
			task3_timer_a1_interrupt();	
			break;
		}
	}
}

void timer_a2_interrupt(void)
{
	switch (super_menu)
	{
		case TASK3:
		{
			task3_timer_a2_interrupt();	
		}
	}
}