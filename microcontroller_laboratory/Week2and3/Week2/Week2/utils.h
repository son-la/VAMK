#ifndef UTILS_H
#define UTILS_H
#define PI 3.14159265358979323846
#include "lcd.h"

void lcd_display_floating_point(double value, int real_part, char unit, int line_num);
double getSine(double angle);
double getTriangle(double x);
void lcd_display_recording(int buffer, int RESOLUTION);
void lcd_display_playback(int buffer, int RESOLUTION);

#endif