#ifndef TIMER_STUFF_H
#define TIMER_STUFF_H

#include "main.h"
#include "sfr62p.h"
#include "lcd_utils.h"
#include "task_init.h"
#include "switch_function.h"

#include "super_menu.h"
#include "task1.h"
#include "task2.h"
#include "task3.h"

#pragma INTERRUPT timer_a0_interrupt
#pragma INTERRUPT timer_a1_interrupt
#pragma INTERRUPT timer_a2_interrupt


extern int old_knob_val; // Just for initialization

void timer_configuration(void);

void timer_a0_interrupt(void);
void timer_a1_interrupt(void);
void timer_a2_interrupt(void);

#endif 