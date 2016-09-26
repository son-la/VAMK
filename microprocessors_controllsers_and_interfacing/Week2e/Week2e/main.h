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
#pragma INTERRUPT tb0_irq
#pragma INTERRUPT ta0_irq

#pragma INTERRUPT sw0
void tb0_irq(void);

#endif /* MAIN_H_INCLUDED */
