#include "utils.h"

void lcd_display_floating_point(double value, int real_part, char unit, int line_num)
{
	int i;
	long round_val;
	char line[8];
	
	for (i = 0; i < 7 - real_part; i++)
		value *= 10;
	
	// Rounding
	if (value - (long)value > 0)
		value += 1;	
	
	round_val = (long)value;
	
	// Get digit
	for (i = 0; i < 7; i++)
	{
		long divider = 1;
		int j;
		
		if (i == real_part)
		{
			line[i] = '.';	
			continue;
		}
		
		for (j = 0; j < 6 - i; j++)
			divider *= 10;
		if (i > real_part)
			divider *= 10;
			
		line[i] = round_val/divider % 10 + 48;
	}
	
	// Append unit
	line[7] = unit;
	
	// Print
	if (line_num == 1)
		DisplayString(LCD_LINE1, line);
	else
		DisplayString(LCD_LINE2, line);
}

int factorial(int i)
{
	if (i <= 0)
		return 1;
	
	return i*factorial(i-1);	
}

double power(double base, int ord)
{
	if (ord == 0)
		return 1;
	
	return base * power(base,ord - 1);
}

double getSine(double angle)
{
	// Power series methods
	int i;
	double sine_value = angle;
	int k = -1;
	for (i = 3; i < 14; i+=2)
	{
		sine_value += k*((double)power(angle,i))/((double)factorial(i));
		k = -k;
		//printf("%f\n",sine_value);
	}
	
	return sine_value;
}

double getTriangle(double x)
{
	// Fourier tranform, L = 2;
	double ret = 0.0;
	int k = 1;
	int i;
	
	for (i = 1; i < 120; i+=2)
	{
		ret = ret + (double)k/i/i*getSine(i*PI*x);
		k = -k;
	}
	
	return ret*8/PI/PI;
}

void lcd_display_recording(int buffer, int RESOLUTION)
{
	char line1[8];
	char line2[8];
	double fPercent;
	int iPercent;
	
	line1[0] = 'R';
	line1[1] = 'E';
	line1[2] = 'C';
	line1[3] = 'O';
	line1[4] = 'R';
	line1[5] = 'D';
	line1[6] = ':';
	line1[7] = ' ';
	
	if (buffer < 0)
		iPercent = 0;
	else if (buffer > RESOLUTION)
		iPercent = 100;
	else
	{
		fPercent = (double)(buffer)/RESOLUTION * 100;
		fPercent += fPercent - (int)fPercent >= 0.5 ? 1:0;
		iPercent = (int)fPercent;
	}
	
	line2[0] = iPercent/100 + 48;
	line2[1] = iPercent/10%10 + 48;
	line2[2] = iPercent%10 + 48;
	line2[3] = '%';
	line2[4] = ' ';
	line2[5] = ' ';
	line2[6] = ' ';
	line2[7] = ' ';
	
	DisplayString(LCD_LINE1, line1);
	DisplayString(LCD_LINE2, line2);
	
}

void lcd_display_playback(int buffer, int RESOLUTION)
{
	char line1[8];
	char line2[8];
	double fPercent;
	int iPercent;
	
	line1[0] = 'P';
	line1[1] = 'L';
	line1[2] = 'A';
	line1[3] = 'Y';
	line1[4] = ':';
	line1[5] = ' ';
	line1[6] = ' ';
	line1[7] = ' ';
	
	if (buffer < 0)
		iPercent = 0;
	else if (buffer > RESOLUTION)
		iPercent = 100;
	else
	{
		fPercent = (double)(buffer)/RESOLUTION * 100;
		fPercent += fPercent - (int)fPercent >= 0.5 ? 1:0;
		iPercent = (int)fPercent;
	}
	
	line2[0] = iPercent/100% + 48;
	line2[1] = iPercent/10%10 + 48;
	line2[2] = iPercent%10 + 48;
	line2[3] = '%';
	line2[4] = ' ';
	line2[5] = ' ';
	line2[6] = ' ';
	line2[7] = ' ';
	
	DisplayString(LCD_LINE1, line1);
	DisplayString(LCD_LINE2, line2);
}