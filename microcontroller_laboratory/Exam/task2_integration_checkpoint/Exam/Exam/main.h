#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


#pragma INTERRUPT sw1
#pragma INTERRUPT sw2
#pragma INTERRUPT sw3

enum super_menu_options {TOP_MENU = 0, TASK1 = 1, TASK2 = 2, TASK3 = 3, TASK4 = 4, TASK5 = 5};


extern enum super_menu_options super_menu;
extern enum super_menu_options super_menu_choice;


void main(void); 
void onceInitialize(void);
void taskInitialize(void);  




#endif
