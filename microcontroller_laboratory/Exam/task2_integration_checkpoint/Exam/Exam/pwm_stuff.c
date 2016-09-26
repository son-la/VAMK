#include "pwm_stuff.h"

#include "task3.h"
#include "main.h"

void pwm_configuration(void)
{
	switch (super_menu)
	{
		case TASK3:
		{
			task3_PWMConfig();
			break;	
		}	
	}	
}