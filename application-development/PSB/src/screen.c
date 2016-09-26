/*
 * screen.c contains function support for display
 */
#include<stdio.h>
#include"screen.h"

/*setColor() set the foreground and background color
of terminal window to specified color
	arguments:
		fg: foreground color
		bg: background color
	return: void
	**Example:
		setColor(RED, bg(RED));

*/
void setColor(short fg, short bg)
{
	printf("\033[%d;%d;1m", fg, bg);
	fflush(stdout);
}


/*resetColor() reset the foreground and background color
to default, fg is white, bg is black
	arguments: void
	return: void
*/
void resetColor(void)
{
	printf("\033[0m");
	fflush(stdout);
}

/*clearScreen() clera the current terminal window
	arguments: void
	return: void
*/
void clearScreen(void)
{
	printf("\033[2J");
	fflush(stdout);
}

/*gotoxy() move the current cursor to specified row and column
of terminal window
	arguments:
		x: column
		y: row
	return: void
*/
void gotoxy(short x, short y)
{
	printf("\033[%d;%dH", y, x);
	fflush(stdout);
}

/*colorString() printf the string with specified background
and forground colors
	arguments:
		s: printed out string
		fg: forground color
		bg: background color
	return: void
	*** example:
		ColorString(s, RED, bg(RED));
*/
void colorString(char *s, short fg, short bg)
{
	setColor(fg, bg);
	printf("%s", s);
	resetColor();
}

