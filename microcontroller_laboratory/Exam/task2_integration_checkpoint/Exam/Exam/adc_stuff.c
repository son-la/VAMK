#include "adc_stuff.h"
#include "main.h"
#include "sfr62p.h"

#include "task2.h"

void adc_configuration(void)
{	
	switch (super_menu)
	{
		case TOP_MENU:
		case TASK3:
		{		
			adst = 0;	
			adcon0 = 0x08; // AD0, Sw trigger
			adcon1 = 0x20; // Vref connected, 8bit mode
			adcon2 = 0x00; // No sample and hold
			adst = 1; // Enable ADC
			break;	
		}	
		case TASK2:
		{
			adst = 0; // Turn off ADC
			
			if (task2_mode != RECORD)
			{	
				adcon0 = 0x08; // AD0
			}
			else
			{
				adcon0 = 0x89; // AD1	
			}
			
			if (task2_max_adc_mode == 255)
			{
				adcon1 = 0x20; // Vref connected, 8bit mode
			}
			else
			{
				adcon1 = 0x28;	// Vref connected, 10bit mode
			}
			adcon2 = 0x00; // No sample and hold
			
			adst = 1; // Enable ADC
			break;	
		}
	}	
}