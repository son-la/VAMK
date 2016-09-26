#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
/***********************************************************************************

FILE NAME  	:	main.h

DESCRIPTION	:	Main header file

Copyright   : 2005 Renesas Technology Europe Ltd.
Copyright   : 2005 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/
#define GAME_SPEED_OPTIONS 3
#define MIN_MAX 1
#define MAX_MIN 0
#define MIN 0x01 // 0001
#define MAX 0x08 // 1000

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
22.11.2005 RTE-DDE First Release
***********************************************************************************/
enum game_states { STOP = 0, START = 1};
enum game_modes {LED_GAME = 0, LCD_GAME = 1};
enum lcd_game_modes {ONE_PLAYER = 0, TWO_PLAYER = 1};
/***********************************************************************************
Function Prototypes
***********************************************************************************/
void main(void);   

#endif /* MAIN_H_INCLUDED */
