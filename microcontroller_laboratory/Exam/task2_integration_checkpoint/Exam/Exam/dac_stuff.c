#include "dac_stuff.h"

#include "main.h"
#include "sfr62p.h"
#include "task2.h"

void dac_configuration(void)
{
	switch (super_menu)
	{
		case TASK2:
		case TASK3:
		{
			da0e = 0; // Turn off DAC
			prc2 = 1; // Turn off protect bit
			pd9_3 = 0; // P9_3 as input
			da0 = 0; 
			da0e = 1; // Enable DAC
			break;	
		}	
	}
}