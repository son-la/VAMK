/*
 * ui.c contains function for animation
 */

#include "ui.h"
#include "helper.h"
#include "screen.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * executeAnimation() is main animation function, calling other related function
 * arguments:
 * 	*pBike: pointer to bikes
 * 	*pTruck: pointer to truck
 * 	bikeStart: number of bike at start
 * 	bikeEnd: number of bike at end
 * 	average: average waiting time
 * 	waitingBiker: number of waiting biker
 * return: void
 */
void executeAnimation(BIKE *pBike, TRUCK *pTruck, int bikeStart, int bikeEnd, double average, int waitingBiker)
{
	system("clear");//clear screen
	createFrame();
	updateParameter(bikeStart, bikeEnd, waitingBiker, average);
	printVehicle(pBike,pTruck);

	gotoxy(1,1);//move cursor to (1,1)
}

/*
 * createFrame() creates thing which is not animated
 */
void createFrame()
{
	/*
	 * 		|											|
	 *W B	|											| B
	 * 		|-------------------------------------------|
	 *	 	|											|
	 *
	 */
	int i;
	for(i = -6; i <= 3; i++)
	{
		gotoxy(TRACKSTART - 1, TRACKPOSY + i);
		printf("|");

		gotoxy(TRACKSTART + TRACKLENGTH + 1, TRACKPOSY +i);
		printf("|");
	}

	for(i = 0; i <= TRACKLENGTH; i++)
	{
		gotoxy(TRACKSTART + i, TRACKPOSY);
		printf("-");
	}

	gotoxy(TRACKSTART -8, TRACKPOSY -2); printf("W"); //print W at start
	gotoxy(TRACKSTART -4, TRACKPOSY -2); printf("B");//print B at start
	gotoxy(TRACKSTART + TRACKLENGTH + 4, TRACKPOSY - 2); printf("B"); //print B at end


}

/*
 * updateParameter() update the number parameter on the screen
 * arguments:
 * 	bikeStart: number of bike at start
 * 	bikeEnd: number of bike at end
 * 	waitingBiker: number of waiting biker
 * 	average: average waiting time
 * return void
 */
void updateParameter(int bikeStart, int bikeEnd, int waitingBiker, double average)
{
	gotoxy(TRACKSTART -9, TRACKPOSY); printf("%03d",waitingBiker); //print waiting biker
	gotoxy(TRACKSTART -5, TRACKPOSY); printf("%03d",bikeStart); //print bike at start
	gotoxy(TRACKSTART + TRACKLENGTH + 3, TRACKPOSY); printf("%03d",bikeEnd);//print biker at end
	gotoxy(TRACKSTART + TRACKLENGTH - 15, 10); printf("Average: %5.2f", average);//print average value
}

/*
 * printVehivle() display bikes and truck to the screen
 * argument:
 *  *pBike: pointer to bikes
 *  *pTruck: pointer to truck
 * return: void
 */
void printVehicle(BIKE *pBike,TRUCK *pTruck)
{
	printBike(pBike);
	printTruck(pTruck);
}

/*
 * printBike() display bikes to the screen
 * arguments:
 *  *pbike: pointer to bikes
 * return void
 */
void printBike(BIKE *pBike)
{
	int i, j, lane;

	int *pLane;//holding lane of each bike
	pLane = (int *)malloc(sizeof(int) * NUMBIKE);

	for(i = 0; i < NUMBIKE; i++)
	{
		if((*(pBike + i)).state == RUN)//Check state of bike
		{
			lane = 0; //No match -> lane 0

			int posx = (int)(TRACKSTART + TRACKLENGTH - (*(pBike + i)).speed * (*(pBike + i)).timeLeft - 0.5);//calculate pos x of bike

			//Adding lane if previous bike occupied area
			for(j = 0; j < i; j++)
			{
				if((*(pBike + j)).state == RUN)
				{
					int posxj = (int)(TRACKSTART + TRACKLENGTH - (*(pBike + j)).speed * (*(pBike + j)).timeLeft - 0.5);
					//Area and lane are occupied
					if(
						((posx == posxj ) ||
						(posx == (posxj + 1)) ||
						(posx == (posxj + 2)) ||
						(posx == (posxj - 1)) ||
						(posx == (posxj - 2)) )&&
						(lane == *(pLane + j))
					)
					{
						lane += 1;
					}
				}
			}

			char s[100];
			*(pLane + i) = lane;// save lane
			gotoxy(posx + 1, TRACKPOSY - 2 -  lane*3);
			sprintf(s,"%03d",(*(pBike + i)).timeLeft);
			if(i%10 != 0)//avoid black color
				colorString(s, 30+i%10, 40);
			else
				colorString(s, 30+9, 40);


			gotoxy(posx + 2, TRACKPOSY - 3 -  lane*3);
			printf(">");


		}
	}
	free(pLane);
}

/*
 * printTruck() display truck to the screen
 * arguments:
 *  pTruck: pointer to truck
 * return: void
 */
void printTruck(TRUCK *pTruck)
{
	if((*pTruck).state == RUN)//Check state of truck
	{
		int xposR = (int)(TRACKSTART + TRACKLENGTH - TRUCKSPEED * (*pTruck).timeLeft - 0.5);//Calculate posx of Truck when go to right
		int xposL = (int)(TRACKSTART + TRUCKSPEED * (*pTruck).timeLeft - 0.5);//Calculate posx of Truck when go to left

		if((*pTruck).dir == RIGHT)//Check direction
		{
			gotoxy(xposR + 1, TRACKPOSY + 2); // +1 to avoid display overlap |
			printf("%03d", (*pTruck).timeLeft);

			gotoxy(xposR + 1, TRACKPOSY + 3);
			printf("%s%s%s", BLOCK, BLOCK, BLOCK);
		}
		else
		{
			gotoxy(xposL - 1, TRACKPOSY + 2);// -1 to avoid display overlap |
			printf("%03d", (*pTruck).timeLeft);

			gotoxy(xposL - 1, TRACKPOSY + 3);
			printf("%s%s%s", BLOCK, BLOCK, BLOCK);
		}
	}
}

