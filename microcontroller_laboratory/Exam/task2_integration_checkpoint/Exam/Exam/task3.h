#ifndef TASK3_H
#define TASK3_H

#include "main.h"
#include "sfr62p.h"
#include "adc_stuff.h"
#include "dac_stuff.h"
#include "pwm_stuff.h"


enum TASK3_MODES {TASK3_NORMAL, TASK3_SET_FQ, TASK3_SET_DC};

void task3_initialize(void);

void task3_PWMConfig(void);

void task3_sw1(void);
void task3_sw2(void);
void task3_sw3(void);


void task3_timer_a0_interrupt(void);
void task3_timer_a1_interrupt(void);
void task3_timer_a2_interrupt(void);

void task3_update_dc_fq(void);
void task3_update_n(void); // As its name
int task3_update_m(void); // As its name
void task3_PWMConfig(void);
void task3_sineConfig(void);
void task3_updateCopyarray(void);


#endif