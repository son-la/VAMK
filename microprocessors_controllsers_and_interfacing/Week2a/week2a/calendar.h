#ifndef CALENDAR_H
#define CALENDAR_H


#define CAL_STRING_LENGTH 19


// Function prototypes
unsigned int get_day_of_week(unsigned int pyear, unsigned int pmonth, unsigned int pday);
void LCD_display_calendar(unsigned int pyear,unsigned int pmonth,unsigned int pday, unsigned int pday_of_week, unsigned int phour,unsigned int pminute,unsigned int psecond);
void update_calendar(void);

#endif
