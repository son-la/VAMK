#ifndef LCD_UTILS_H
#define LCD_UTILS_H

#include "main.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"

/*TOP MENU*/
void superMenu_lcdDisplay(void);
void superMenu_scrollDisplay(void);

/* TASK1*/
void task1_LEDGameInit(unsigned char *dir, unsigned char *led_data); 
void task1_LCDGameInit(unsigned char *dir, int *p1_y, int *p2_y, int *ball_x, int *ball_y, int *ball_x_val, int *ball_y_val);
void task1_gameOver(void);
void task1_LCDGameDisplay(enum task1_lcd_game_modes lcd_game_mode, int p1_y, int p2_y, int ball_x, int ball_y);	

/* TASK2 */
void task2_updateDisplay(void);
void task2_lcdDisplayFloatingPoint(double value, int real_part, char unit, int line_num);

void task2_lcdDisplayRecording(int buffer, int resolution);
void task2_lcdDisplayPlayBack(int buffer, int resolution);

/* TASK3 */
void task3_LCDDisplayFunction(enum TASK3_MODES mode, double task3_dc_val, int task3_fq_choice);


/* Empty Project */
void emptyProjectDisplay(void);
#endif