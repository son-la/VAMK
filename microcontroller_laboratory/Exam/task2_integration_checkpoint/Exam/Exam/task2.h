#ifndef TASK2_H
#define TASK2_H



#define TASK2_RESOLUTION 20 // Samples per period
#define ON 1
#define OFF 0

enum TASK2_MODES {NORMAL, SINE_WAVE, TRIANGLE_WAVE, RECORD, PLAYBACK};

extern enum TASK2_MODES task2_mode;
extern int task2_max_adc_mode;
extern float task2_percent; // 0 to 100
extern float task2_voltage; // 0 to 5
extern float task2_freq; 
extern int task2_data_pos;

void task2_initialize(void);

void task2_timer_a0_interrupt(void);
void task2_timer_a1_interrupt(void);

void task2_sw1(void);
void task2_sw2(void);
void task2_sw3(void);


#endif