#include "lcd.h"

int first_char_left = 0;  //First character in first lcd position
void LCD_display_normal(int adc_mode, long n, long m, long freq)
{
	char line1[8];
	char line2[27];
	int i; // counter
	int char_pos; // character position in string
	long th;
	long tp;
	long dc;
	int string_length = 0;
	
	
	line1[0] = 'M';
	line1[1] = 'O';
	line1[2] = 'D';
	line1[3] = 'E';
	line1[4] = ':';
	line1[5] = adc_mode/10 % 10 + 48;
	line1[6] = adc_mode % 10 + 48;
	line1[7] = ' ';	
	
	DisplayString(LCD_LINE1, line1);
	
	if (adc_mode == 8)
	{
		double fth = (double)n*(m+1)/freq*100000;
		double ftp = (double)255*(m+1)/freq*100000; 
		double fdc = fth/ftp*10000;
		
		fth += (fth - (long)fth) >= 0.5 ? 1:0;
		ftp += (ftp - (long)ftp) >= 0.5 ? 1:0;
		fdc += (fdc - (long)fdc) >= 0.5 ? 1:0;
		
		th = (long)fth;
		tp = (long)ftp;
		dc = (long)fdc;
		
		line2[0] = 'T';
		line2[1] = 'h';
		line2[2] = '=';
		line2[3] = th / 1000 + 48;
		line2[4] = th / 100 % 10 + 48;
		line2[5] = '.';
		line2[6] = th / 10 % 10 + 48;
		line2[7] = th % 10 + 48;
		line2[8] = ',';
		line2[9] = ' ';
		line2[10] = 'T';
		line2[11] = 'p';
		line2[12] = '=';
		line2[13] = tp / 1000 + 48;
		line2[14] = tp /100 % 10 + 48;
		line2[15] = '.';
		line2[16] = tp / 10 % 10 + 48;
		line2[17] = tp % 10 + 48;
		line2[18] = ',';
		line2[19] = ' ';
		line2[20] = 'D';
		line2[21] = 'C';
		line2[22] = '=';
		line2[23] =  dc / 1000 + 48;
		line2[24] = dc /100 % 10 + 48;
		line2[25] = '.';
		line2[26] = dc/10%10 + 48;
		line2[27] = dc % 10 + 48;
		line2[28] = '%';
		line2[29] = ' ';
		line2[30] = ' ';
		
		string_length = 31;
	}
	else
	{
		double fdc = (double)n/65535*10000 ;
		fdc += (fdc - (long)fdc > 0.5 ? 1:0);
		dc = (long)fdc;
		
		
		line2[0] = 'D';
		line2[1] = 'C';
		line2[2] = '=';
		line2[3] =  dc / 1000 + 48;
		line2[4] = dc /100 % 10 + 48;
		line2[5] = '.';
		line2[6] = dc/10%10 + 48;
		line2[7] = dc % 10 + 48;
		line2[8] = '%';
		line2[9] = ' ';
		line2[10] = ' ';
		
		string_length = 11;
	}
	
	
	LCD_write(CTRL_WR, (unsigned char)(LCD_HOME_L2) );
	
	char_pos = first_char_left; // First character to print in first_char string position
	for (i = 0; i < 8; i++)
	{		
		LCD_write(DATA_WR,line2[char_pos]);
		
		if (++char_pos == string_length)
			char_pos = 0;		
	}
	
	if (++first_char_left == string_length)
		first_char_left = 0;
		
}


void LCD_dispaly_set_dc(int dc)
{
	char line1[8];
	char line2[8];
	
	line1[0] = 'S';
	line1[1] = 'E';
	line1[2] = 'T';
	line1[3] = ' ';
	line1[4] = 'D';
	line1[5] = 'C';
	line1[6] = ':';
	line1[7] = ' ';
	
	DisplayString(LCD_LINE1, line1);
	
	line2[0] = ' ';
	line2[1] = ' ';
	line2[2] = ' ';
	line2[3] = ' ';
	line2[4] = ' ';	
	line2[5] = dc / 10 % 10 + 48;
	line2[6] = dc % 10 + 48;
	line2[7] = '%';
	
	DisplayString(LCD_LINE2, line2);
}

void LCD_display_set_th(int th)
{
	char line1[8];
	char line2[8];
	
	line1[0] = 'S';
	line1[1] = 'E';
	line1[2] = 'T';
	line1[3] = ' ';
	line1[4] = 'T';
	line1[5] = 'h';
	line1[6] = ':';
	line1[7] = ' ';
	
	DisplayString(LCD_LINE1, line1);
	
	line2[0] = ' ';
	line2[1] = ' ';
	line2[2] = ' ';
	line2[3] = ' ';
	line2[4] = ' ';	
	line2[5] = ' ';	
	line2[6] = th / 10 % 10 + 48;
	line2[7] = th % 10 + 48;
	
	
	
	DisplayString(LCD_LINE2, line2);
}