#include "main.h"
#include "task_init.h"

enum super_menu_options super_menu;
enum super_menu_options super_menu_choice;

void main(void)				
{
	onceInitialize();
	taskInitialize();
	
	while(1){}
}         