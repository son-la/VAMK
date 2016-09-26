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

// MIN  1-1-2015
// MAX 31-12-2020
#define MIN_YEAR 2015
#define MAX_YEAR 2020
#define DAY_OF_WEEK0 4 // Thursday
#define DELAY 5

 // Opeartion modes

extern unsigned int year;
extern unsigned int month;
extern unsigned int day;
extern unsigned int day_of_week;

extern unsigned int hour;
extern unsigned int minute;
extern unsigned int second;
extern unsigned int msecond;

extern char days_of_week[7][4];
extern char months_of_year[12][4];


enum operation_modes { CALENDAR = 0, SET_TIME = 1, ALARM = 2, MODE_CHOOSING = 3};
enum time_setting {SET_HOUR, SET_MINUTE, SET_SECOND, SET_YEAR, SET_MONTH, SET_DAY};

extern enum operation_modes mode;
extern enum time_setting set_time_choice;
/***********************************************************************************
Function Prototypes
***********************************************************************************/
void main(void);   


#endif /* MAIN_H_INCLUDED */
