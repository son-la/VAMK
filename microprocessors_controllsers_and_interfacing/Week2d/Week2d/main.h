#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
/***********************************************************************************

FILE NAME  	:	main.h

DESCRIPTION	:	Main header file

Copyright   : 2005 Renesas Technology Europe Ltd.
Copyright   : 2005 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
22.11.2005 RTE-DDE First Release
***********************************************************************************/


 // Opeartion modes

enum operation_modes { NORM = 0, SET_DC = 1, SET_TH = 2};

extern enum operation_modes mode;
/***********************************************************************************
Function Prototypes
***********************************************************************************/
void main(void);   
double update_tp(void);
void PWM_config(void);

#endif /* MAIN_H_INCLUDED */
