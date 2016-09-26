/*
 *operation.c contains all logic operation code
 */
#include "helper.h"
#include "operation.h"
#include "ui.h"
#include "screen.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>

/*
 * operation() contains all operation code and calling function
 *
 * argument: void
 * return: void
 */
void operation()
{

//DECLARE VARIABLE AND INITIALIZE VARIABLE

	int i;//Count variable

	TRUCK truck;//
	BIKE *pBike;//array of pointer to bikes

	int *pWait;//pointer to array of waiting space;
	//= 0: free space
	//= 1: occupied space

	double average = 0;//Average waiting time
	int totalWaitingBiker = 0;//Total number waiting biker


	int waitingBiker;//waiting biker
	int bikeStart;//bike at start point
	int bikeEnd;//bike at end point

	int *pBikeStart;//pointer to bike at start
	pBikeStart = &bikeStart;
	int *pBikeEnd;//pointer to bike at end
	pBikeEnd = &bikeEnd;


	TRUCK *pTruck;//Pointer to truck
	pTruck = &truck;



	bikeStart = NUMBIKE;
	bikeEnd = 0;
	waitingBiker = 0;//Number of waiting biker

	truck.state = IDLE;
	pBike = (BIKE *)malloc(sizeof(BIKE)*NUMBIKE);
	pWait = (int *)malloc(sizeof(int) * WAITINGMAX);


	srand(time(NULL));//randomize the seed for random function

//END DECLARE VARIABLE AND INITIALIZE VARIABLE


//LOOP
	int duration; //Loop counter control
	for(duration = 0; duration < DURATION; duration++)
	{

//GENERATE NEW CUSTOMER
		if(waitingBiker + MAXTUORIST <= WAITINGMAX)
		{
			int newCustomer;
			newCustomer = rand()%100 + 1;//Generate probability to have customers from 1 to 100

			if(newCustomer <= PROB)//Inside probability range
			{
				int numOfNewBiker = rand() % MAXTUORIST + 1;//Generate probability number of customers

				waitingBiker += numOfNewBiker; //Add to waiting list

				//Set waiting space to 1 for new wait (Finding free waiting space)
				for(i = 0; i < WAITINGMAX; i++)
				{
					if(*(pWait + i) == 0)//Available bike
					{
						*(pWait +i) = 1;//Set waiting space to 1
						if(--numOfNewBiker == 0)//All is allocated
						{
							break;//End loop
						}
					}
				}
			}
		}
//END GENERATE NEW CUSTOMER

//CALCULATE NEW BIKER AND UPDATE LIST OF WAITING BIKER
		int newBiker = waitingBiker;//waitingBiker before taking bike

		//Check bike at starting point
		bikeStart -= waitingBiker; //Total bike at starting point subtract to waiting Biker

		if(bikeStart < 0)//Less bike than enough
		{
			waitingBiker = bikeStart*(-1); //waiting biker after taking bike
			bikeStart = 0;//set bike at starting point back to 0
		}
		else //enough bike -> no waiting biker
		{
			waitingBiker = 0; //set waiting biker = 0
		}

		//Get new bikers
		newBiker -= waitingBiker;//waitingBiker before taking bike - after taking bike = new Biker
		 // - 0 means all waitingBiker has bike
//END CALCULATE NEW BIKER AND UPDATE LIST OF WAITING BIKER


//REMOVE NEWBIKER FROM WAITINGLIST, START FROM WAITING LONGEST
		if(newBiker > 0)
		{
			for(i = 0; i < newBiker; i++)
			{
				*(pWait + i) = 0; //Remove or make free space
			}
		}
//END REMOVE NEWBIKER FROM WAITINGLIST


//SORTING WAITING SPACE BY WAITING TIME FROM LONGEST TO SHORTEST
		sortingWaitList(pWait);
//END SORTING WAITING SPACE BY WAITING TIME FROM LONGEST TO SHORTEST


//CALCULATE AVERAGE WAITING TIME

		average = average * totalWaitingBiker; //convert average value to total waiting time

		//Adding time for waiting biker
		for(i = 0; i < WAITINGMAX; i++)
		{
			/*
			 * If = 0: no more waiting after this point(after sorting)
			 * If = 2(1 for initialize + 1 for addint time): new waiting biker
			 * > 2: not new waiting biker -> no add to total waiting biker
			 */
			if(*(pWait + i) > 0)
			{
				*(pWait + i) += 1;//Add total waiting biker if they have to wait
				average += 1;//add time to total waiting time
			}

			if(*(pWait + i) == 2) //new waiting biker
				totalWaitingBiker += 1;

			if(*(pWait + i) == 0)//After sorting so after this point no more waiting biker
				break;
		}

		//Update average value
		if(totalWaitingBiker != 0)
			average = average / totalWaitingBiker;

//END CALCULATE AVERAGE WAITING TIME


//UPDATE TIME LEFT FOR VEHICLE
		//Moving bikes
		moveBike(pBike, pBikeEnd);

		//Check bike at end to call truck
		if(bikeEnd >= BIKEREF)
		{
			int ret = callNewTruck(pTruck);
			if(ret == 1) //Call truck success
			{
				bikeEnd -= BIKEREF;//Reduce truck at end
			}
		}

		//Moving truck
		moveTruck(pTruck, pBikeStart);

		//Initialize new biker
		callNewBike(pBike, newBiker);//Initialize new biker
//END UPDATE TILE LEFT FOR VEHICLE


//ANIMATION
		executeAnimation(pBike, pTruck, bikeStart, bikeEnd, average, waitingBiker);
//END ANIMATION


		system("sleep 1");//Sleep for 1 second
	}

//END LOOP

	free(pWait);
	free(pBike);
}




/*
 * callNewBike() initialize new bikers
 *arguments: pointers to bikes and number of new biker
 *return:void
 */
void callNewBike(BIKE *pBike, int newBiker)
{
	int i; //count variable
	for(i = 0; i < NUMBIKE; i++)
	{
		if(newBiker == 0)//No new biker
			break;
		if((*(pBike + i)).state == IDLE)//Found available bikes
		{
			(*(pBike + i)).state = RUN; //Change state of bike
			(*(pBike + i)).timeLeft = rand()%(TBIKE2 - TBIKE1) + TBIKE1 + 1; //Initialize time left
			(*(pBike + i)).speed = (double)TRACKLENGTH / (double)(*(pBike + i)).timeLeft; //Speed
			--newBiker;//Decrease new biker
		}
	}
}

/*callTruck() calling truck to carry bikes at end
 *arguments: pointer to truck
 *return: void
 */
int callNewTruck(TRUCK *truck)
{
	//Check truck available or not
	if((*truck).state == RUN)
	{
		return 0;//Return 0 for calling unsuccessful
	}
	else
	{
		//Initialize truck
		(*truck).dir = LEFT;
		(*truck).state = RUN;
		(*truck).timeLeft = TTRUCK;
		return 1;
	}
}


/*
 * moveTruck() reduce the time left of truck if state is not idle
 *and control truck state and bike at start
 * arguments:
 * 	truck: pointer to truck
 * 	pBikeStart: pointer to bike at starting point
 * return:void
 *
 */
void moveTruck(TRUCK *truck, int *pBikeStart)
{
	if((*truck).state == IDLE) //no move if truck is idle
	{
		return;
	}
	else
	{
		if(--((*truck).timeLeft) <= 0) //reduce time left
		{
			if((*truck).dir == LEFT) //move to left
			{
				(*truck).dir = RIGHT;//make truck go back
				(*truck).timeLeft = TTRUCK;//set time left again
				(*pBikeStart) += BIKEREF;//Add bike to starting point
			}
			else
			{
				(*truck).state = IDLE;// if truck direction is right, make truck rest
			}
		}
	}
}


/*
* moveBike() reduce the time left of bike which has state not idle
* also increase bike at the end if bike go to the end
* arguments:
* 	pBike: pointer to bikes
* 	bikeEnd: number of bike at end
*/
void moveBike(BIKE *pBike, int *bikeEnd)
{
	int i;
	for(i = 0; i<NUMBIKE; i++)
	{
		//Check bike state
		if((*(pBike +i)).state == RUN)
		{
			if(--(*(pBike + i)).timeLeft <= 0)//Reduce time
			{
				(*(pBike + i)).state = IDLE;//Set bike to available
				(*bikeEnd)++;//Increase bike at end
			}
		}
	}
}


/*
 * sortingWaitList() sort the wait list by waiting time from oldest to newest. Bubble sort
 *	arguments: pointer to waiting space
 *	return: void
 */
void sortingWaitList(int *pWait)
{
	int i,j;
	for(i = 0; i < WAITINGMAX; i++)
	{
		for(j = i; j < WAITINGMAX; j++)
		{
			if(*(pWait + j) > *(pWait + i))
			{
				int temp = *(pWait + i);
				*(pWait + i) = *(pWait + j);
				*(pWait + j) = temp;
			}
		}
	}
}
