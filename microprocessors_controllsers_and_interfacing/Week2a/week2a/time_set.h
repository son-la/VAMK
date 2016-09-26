#ifndef TIME_SET_H
#define TIME_SET_H

#include "main.h"

void hour_adjust(void);
void minute_adjust(void);
void second_adjust(void);
void year_adjust(void);
void month_adjust(void);
void day_adjust(void);
void LCD_display_time_setting(enum time_setting);

#endif