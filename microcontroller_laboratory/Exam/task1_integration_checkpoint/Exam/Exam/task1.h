#ifndef TASK1_H
#define TASK1_H


#define GAME_SPEED_OPTIONS 3
#define MIN_MAX 1
#define MAX_MIN 0
#define MIN 0x01 // 0001
#define MAX 0x08 // 1000

void task1_initialize(void);
void task1_changeGameMode(void);
void task1_checkGameOver(void);
void task1_playGame(void);
void task1_changeGameSpeed(void);
void task1_sw1(void);
void task1_sw2(void);

enum task1_game_states { STOP = 0, START = 1};
enum task1_game_modes {LED_GAME = 0, LCD_GAME = 1};
enum task1_lcd_game_modes {ONE_PLAYER = 0, TWO_PLAYER = 1};

#endif