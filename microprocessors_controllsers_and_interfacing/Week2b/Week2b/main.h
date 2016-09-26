#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

/***********************************************************************************

FILE NAME  	:	main.h

DESCRIPTION	:	header file

Copyright   : 2005 Renesas Technology Corporation.
All Rights Reserved
***********************************************************************************/

/**********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
11.08.2005 RTA-MGF First Release
**********************************************************************************/ 

/***********************************************************************************
Function Prototypes
***********************************************************************************/

void main(void);   
void timer_init(void);
/* declare routine as an interrupt so compiler ends routine with REIT instead of RTS, also
declare vector number so the compiler can automatically generate vector table */ 
#pragma INTERRUPT ta3_irq
#pragma INTERRUPT ta2_irq
#pragma INTERRUPT ta1_irq
#pragma INTERRUPT sw1_irq
#pragma INTERRUPT sw0_irq

enum light_mode{ MODE0, MODE1 };

void ta3_irq(void);
void ta2_irq(void);
void ta1_irq(void);
void sw1_irq(void);
void sw0_irq(void);

#endif /* MAIN_H_INCLUDED */
