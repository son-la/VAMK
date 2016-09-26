#ifndef SWITCH_FUNCTION_H
#define SWITCH_FUNCTION_H




#pragma INTERRUPT sw1
#pragma INTERRUPT sw2
#pragma INTERRUPT sw3

extern int debounce_swPress;

void switch_initialize(void);

#endif