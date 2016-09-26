#ifndef SUPER_MENU_H
#define SUPER_MENU_H

extern int superMenu_old_knob_value;

void superMenu_initialize(void);

void superMenu_timer_a0_interrupt(void);

void superMenu_sw1(void);
void superMenu_sw2(void);
void superMenu_sw3(void);


#endif