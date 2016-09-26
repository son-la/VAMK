/***********************************************************************************
FILE NAME  	:	main.c
DESCRIPTION :	Empty proejct set up for RSKM16C62P					


Copyright   : 2005 Renesas Technology Europe Ltd.
Copyright   : 2005 Renesas Technology Corporation.
All Rights Reserved

***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
21.01.2006 RTA-MGF First Release
***********************************************************************************/

/**********************************************************************************
System Includes
***********************************************************************************/

/**********************************************************************************
User Includes
***********************************************************************************/
#include "sfr62p.h"
/* rskM16C62p_def.h defines some common definitions */
#include "rskM16C62Pdef.h"	
#include "main.h"
#include "lcd.h"


/**********************************************************************************
Global variables
***********************************************************************************/

#pragma INTERRUPT sw1 // int0
#pragma INTERRUPT sw3 // int2
#pragma INTERRUPT timer_a0_interrupt // Timer a0

#define MIN 0x01 // 0001
#define MAX 0x08 // 1000

#define LCD_MAX 16 // 2 line LCD spaces
#define STRING_LENGTH 26 // Length of string


#define MIN_MAX 1
#define MAX_MIN 0
/**********************************************************************************
User Program Code
***********************************************************************************/


unsigned char preset_data = 0xFE;//1111 1110
unsigned char dir; // MIN_MAX for left to right, MAX_MIN for right to left
unsigned char toggle; // Button have already pressed at least once 
unsigned char changed; // Push button is pressing
char string[] = "Vaasan ammattikorkeakoulu ";



int first_char_scroll = 0;
int first_char_left = 0;  //First character in first lcd position
int lcd_pos_2line = 0; 
int char_pos_2line1 = 0;
int char_pos_2line2 = 0;


void sw1(void)
{
	/* Push butotn 1 is pressed */
	toggle = 1;
	if (dir == 1) dir = 0;
	else
		dir = 1;
		
	//dir != dir;
	/*if (dir == 1)
		DisplayString(LCD_LINE1, "A");
	else
		DisplayString(LCD_LINE1, "B");
	*/
}

int show_choice = 2;
void sw3(void)
{
	if (++show_choice == 3)
		show_choice = 0;
		
		
	first_char_left = 0; 
	first_char_scroll = 0;
	lcd_pos_2line = 0; 
	char_pos_2line1 = 0;
	char_pos_2line2 = 0;
		
}

unsigned int msecond = 0;
unsigned int second = 0;

void push_button_pressed(void)
{
	toggle = 1; // Set toggle 1
	

	if (changed) // Check begin or middle of push session
		return;	// Middle -> return

	changed = 1;
	dir = !dir;	
}

void push_button_released(void)
{
	if (!changed) // Check in session or not
		return; // Not in session -> return
	
	changed = 0;
	dir = !dir;	
			
}


// Scrolling up

void LCD_display_scroll_up()
{
	int i;
	int char_pos;
	
	LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L1) );
	
	

	// Activate scrolling
	char_pos = first_char_scroll;
	
	for	(i = 0; i < LCD_MAX; i++)
	{
		if (char_pos >= STRING_LENGTH) // Print all character
		{
			int j;
			first_char_scroll = 0;
			for (j = 0; j < LCD_MAX-i; j++)
			{
				LCD_write(DATA_WR,' ');
			}	
			return;
		}
		
		LCD_write(DATA_WR,string[char_pos++]);
		
		/* IF NOT AUTO NEW LINE */
		if (i == 7)
			LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L2) );
	}
	first_char_scroll += 8;
	
}




// Scrolling left

void LCD_display_scroll_left()
{
	
	// ===========================================
	int i; // counter
	int char_pos; // character position in string
	
	LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L1) );
	
	

	char_pos = first_char_left; // First character to print in first_char string position
	for (i = 0; i < LCD_MAX; i++)
	{
		LCD_write(DATA_WR,string[char_pos]);
		if (++char_pos == STRING_LENGTH)
			char_pos = 0;	
		/* IF NOT AUTO NEW LINE */
		if (i == 7)
			LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L2) );
	}
	
	if (++first_char_left == STRING_LENGTH)
		first_char_left = 0;
}



void LCD_display_scroll_2_line()
{
	
	int i;
	LCD_write(CTRL_WR,LCD_CLEAR);
	LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L1) );
	for (i = 0 + lcd_pos_2line*16 + char_pos_2line1; i < 0 + lcd_pos_2line*16 + 8; i++)
	{
		if (i >= STRING_LENGTH)
			break;
		LCD_write(DATA_WR,string[i]);
		
	}
	LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L2) );
	for (i = 8 + 16*lcd_pos_2line + char_pos_2line2; i < 8 + lcd_pos_2line*16 + 8; i++)
	{
		if (i >= STRING_LENGTH)
			break;
		
		LCD_write(DATA_WR,string[i]);
		
	}
	
		
	if (char_pos_2line1 == 8)
	{
		char_pos_2line1 = 0;
		char_pos_2line2 = 0;
		if (++lcd_pos_2line == STRING_LENGTH / 8 - 1)
			lcd_pos_2line = 0;
			
		return;	
	}
	
	char_pos_2line1++;
	char_pos_2line2++;
}

void timer_a0_interrupt(void)
{
	// Delay 

	if (++msecond == 1000){
		msecond = 0;
		
		// Assign data to port LED
		p4 = preset_data; 
	

		// Inverse for processing 
		preset_data = ~preset_data;
		
		// Check input button
		if (p8_3 == 0) // Push button (SW2) pushed	
		{
			push_button_pressed();
			if (dir == MIN_MAX && preset_data == MIN) 
			{
				preset_data = ~MAX;	
				return;	
			}
			else if (dir == MAX_MIN && preset_data == MAX)
			{
				preset_data = ~MIN;		
				return;	
			}
		}
			
		else // Not pushed
		{
			push_button_released();
		
			if (!toggle) // Button never pressed -> Bouncing
			{
				if (preset_data == MIN || preset_data == MAX)
					dir = !dir;
			}
			else // One direction =D
			{
				if (dir == MIN_MAX && preset_data == MIN) 
				{
					preset_data = ~MAX;	
					return;	
				}
				else if (dir == MAX_MIN && preset_data == MAX)
				{
					preset_data = ~MIN;		
					return;	
				}
				
			}
		}
		
		
		// Shifting
		if (dir == MAX_MIN)
			preset_data = preset_data << 1;
		else 
			preset_data = preset_data >> 1;
		
		preset_data = ~preset_data;
		
		
		

		
		// LCD
		
		
		switch(show_choice)
		{
			case 0:
			{
				LCD_display_scroll_left();
				break;
			}
			case 1:
			{
				LCD_display_scroll_up();
				break;	
			}
			case 2:
			{
				LCD_display_scroll_2_line();
				break;	
			}
				
		}
	}
}


/**********************************************************************************
Function Name: 	Main
Description:	Main function
Parameters: 	none
Return value: 	none
***********************************************************************************/
void main(void)				
{
	InitialiseDisplay();
	//LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L1) );	
	// Intialize value
	dir = MIN_MAX;
	toggle = 0;
	changed = 0;
	
	// Set LED
	p4 = 0xF;
	
	// Set button as input
	pd8_3 = 0;
		
	// Time interval
	ta0 = 9999;
	
	/* Enable interrupt */
	asm("FSET I");
	
	ta0ic = 0x07; // Priority level for timer a0
	
	int0ic = 0x01; // Priority level for sw0
	int2ic = 0x01; // Priority level for sw2
	
	tabsr |= (1 << 0); // Start timer a0
	
	while(1){}
	
}         
/**********************************************************************************
End of function main
***********************************************************************************/   

