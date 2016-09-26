#ifndef UTILS_H
#define UTILS_H
#include "main.h"

void led_game_init(unsigned char *dir, unsigned char *led_data); 
void lcd_game_init(unsigned char *dir, int *p1_y, int *p2_y, int *ball_x, int *ball_y, int *ball_x_val, int *ball_y_val);
void gameover(void);
void lcd_game_display(enum lcd_game_modes lcd_game_mode, int p1_y, int p2_y, int ball_x, int ball_y);	

#endif