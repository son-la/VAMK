#include "set_operation.h"
#include "main.h"
#include "lcd.h"


void LCD_display_show_options(int choice)
{
	LCD_write(CTRL_WR,LCD_CLEAR);
	
	DisplayString(LCD_LINE1, "Modes:");
		
	switch(choice)
	{
		case 0:
		{
			DisplayString(LCD_LINE2, "BACK");	
			break;
		}
		case 1:
		{
			DisplayString(LCD_LINE2, "SET TIME");	
			break;	
		}
		case 2:
		{
			DisplayString(LCD_LINE2, "ALARM");	
			break;		
		}
	}
	
}
