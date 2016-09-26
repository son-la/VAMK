/*
 * ui.h contains function prototype for all display function
 */
#ifndef __UI_H
#define __UI_H

#include "helper.h"

//main function to animate ~ interface to logic code
void executeAnimation(BIKE *, TRUCK *, int, int, double, int );

//create frame which is not animated
void createFrame();
//update number parameter
void updateParameter(int, int, int, double);
//print vehicle
void printVehicle(BIKE *,TRUCK *);
//print bikes
void printBike(BIKE *);
//print truck
void printTruck(TRUCK *);


#endif
