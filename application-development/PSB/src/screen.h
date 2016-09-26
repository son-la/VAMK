/*
 * screen.h contains function prototype and color enumeration using for screen display
 */
#ifndef __SCREEN_H
#define __SCREEN_H

enum COLORS {BLACK = 30, RED, GREEN, YELLOW, BLUE, MAGNETA, CYAN, WHITE};//Foreground color

#define bg(a) a+10 //define macro
#define BLOCK "\u2591"//block character in unicode

void setColor(short, short);//set foreground and background color
void resetColor();//reset color to default
void clearScreen(void);//clear the screen
void gotoxy(short, short);//move cursor position
void colorString(char *s, short fg, short bg);//print string with specified color

#endif
