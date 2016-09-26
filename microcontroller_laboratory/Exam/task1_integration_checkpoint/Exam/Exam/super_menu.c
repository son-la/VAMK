#include "super_menu.h"
#include "adc_stuff.h"
#include "timer_stuff.h"

void superMenu_initialize(void)
{
	adc_configuration();
	timer_configuration();
}