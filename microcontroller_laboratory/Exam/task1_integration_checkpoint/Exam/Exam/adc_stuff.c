#include "adc_stuff.h"
#include "main.h"
#include "sfr62p.h"


void adc_configuration(void)
{	
	switch (super_menu)
	{
		case TOP_MENU:
		{		
			adst = 0;	
			adcon0 = 0x08; // AD0, Sw trigger
			adcon1 = 0x20; // Vref connected, 8bit mode
			adcon2 = 0x00; // No sample and hold
			adst = 1; // Enable ADC
			break;	
		}	
	}	
}