#include "lcd_utils.h"
#include "lcd.h"

void updateLCD(int n, int m, long freq)
{
	char line1[8];
	char line2[8];
	long th,tp;
	
	float fth = (float)n*(m+1)/(float)freq * 1000 *100; // 100 for decimal, 1000 for ms
	float ftp = (float)255*(m+1)/(float)freq * 1000 *100;  // 100 for decimal, 1000 for ms
	
	fth += (fth - (long)fth) >= 0.5 ? 1:0;
	ftp += (ftp - (long)ftp) >= 0.5 ? 1:0;
	
	th = (long)fth;
	tp = (long)ftp;
	
	line1[0] = 'T';
	line1[1] = 'h';
	line1[2] = '=';
	line1[3] = th/1000 + 48;
	line1[4] = th/100%10 + 48;
	line1[5] = '.';
	line1[6] = th/10%10 + 48;
	line1[7] = th%10 + 48;
	
	line2[0] = 'T';
	line2[1] = 'p';
	line2[2] = '=';
	line2[3] = tp/1000 + 48;
	line2[4] = tp/100%10 + 48;
	line2[5] = '.';
	line2[6] = tp/10%10 + 48;
	line2[7] = tp%10 + 48;
	
	DisplayString(LCD_LINE1, line1);
	DisplayString(LCD_LINE2, line2);
}