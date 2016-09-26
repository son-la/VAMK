/*
 * helper.h contains the parameter of application and also some common use like CONSTANT, MACRO or TYPEDEF
 */

#ifndef __HELPER_H
#define __HELPER_H


//Constant define
#define DURATION 100 //Length of simulation
#define WAITINGMAX 100 // Maximum of waiting people

#define NUMBIKE 10 //Number of bike
#define TBIKE1 10 //Min time to travel of bike
#define TBIKE2 20 //Max time to travel of bike
#define TTRUCK 5 //Time to travel of truck
#define BIKEREF 8 //Bike at end to start the truck
#define PROB 40 //Probability to generate tourist
#define MAXTUORIST 3 //Maximum number of tourist at one time


#define TRACKPOSY 30//Position of track row
#define TRACKPOSX 80//Position of track column
#define TRACKSTART 10 //Starting position
#define TRACKLENGTH 60//Length of track

//Macro define
#define TRUCKSPEED (TRACKLENGTH/TTRUCK) //Calculate speed of truck



//Enum variable
enum STATE {IDLE, RUN}; //State of vehicle: idle or in use
enum DIR {LEFT,RIGHT}; //Direction of vehicle: go to the left or go to the right


//type def struct of TRUCK and BIKE
typedef struct truckType{
	enum STATE state;
	enum DIR dir;
	unsigned int timeLeft;
}TRUCK;

typedef struct bikeType{
	unsigned char state;
	unsigned int timeLeft;
	double speed;
}BIKE;

#endif
