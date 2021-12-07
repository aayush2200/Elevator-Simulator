#include <at89c5131.h>
#include "lcd.h"
#include <stdio.h>	//for sprintf functions

/******************* Global variables for the project*********************************************/

//Variable to store which at which floor is the user
unsigned char src = 0;

//Variable to store at which floor the user has to go
unsigned char dst = 0;

//variable that stores which elevator to use out of the 2 elevators
unsigned char elev_index = 0;

//array that stores the current position of each elevator
unsigned char elev[2] = {0, 0};

//array that stores the commands that would be send to change the cursor position according to elevator index
code unsigned int cmd[2] = {0x80, 0x85};

//floor array that stores the value of the floor when converted to string to send to LCD
unsigned char floor[3] = {0,0,'\0'};

/*******************************Functions of the project*******************************************/


void mov_elevator(unsigned char pos)
/******************************************************************************
This function moves the elevator to the desired position which is given as
the input to the function. It uses the global elevator index to move the desired
elevator to the required position.
*******************************************************************************/
{
	char disp = pos - elev[elev_index];
	if (disp < 0) disp = -disp;
	while (1) {
		if (disp == 0) break;
		if (pos > elev[elev_index]) {
			elev[elev_index] += 1;
			msdelay(1000);
			lcd_cmd(cmd[elev_index]);
			int_to_string(elev[elev_index], floor);
			lcd_write_string(floor);
		}
		else {
			elev[elev_index] -= 1;
			lcd_cmd(cmd[elev_index]);
			msdelay(1000);
			lcd_cmd(cmd[elev_index]);
			int_to_string(elev[elev_index], floor);
			lcd_write_string(floor);
		}
		disp -= 1;
	}
}


int take_input(unsigned char prev)
/*****************************************************************************
This function takes the input from the switch and returns that value.
The argument given to the function is the previous value of switch.
If the previous value is same as the current value it remains in infinte loop.
******************************************************************************/
{
	unsigned char curr;
	while (1) {
		curr = P1 & 0x0F;
		msdelay(100);
		if (curr != prev) break;
	}
	return curr;
}


int nearest_elevator(unsigned char new_pos)
/**************************************************************************************
This function finds the nearest elevator and returns the index of that elevator.
The argument given to the function is the location where the user called the elevator.
***************************************************************************************/
{
	unsigned char move = 0;
	char diff1 = new_pos - elev[0];
	char diff2 = new_pos - elev[1];
	if (diff1 < 0) diff1 = -diff1;
	if (diff2 < 0) diff2 = -diff2;
	
	if (diff1 > diff2) {move = 1;}
	else {move = 0;}
	
	return move;
}

/***************************************MAIN FUNCTION*************************************************************/
void main(void)
{
	// Port, LCD initialisation
	P1 = 0xFF;
	lcd_init();
	lcd_write_string("00 | 00");
	while (1)
	{
		// Take input from the switch to get the user location
		src = take_input(dst);
		// Find the nearest elevator to the user
		elev_index = nearest_elevator(src);
		// Move that elevator to the user location
		mov_elevator(src);
		// Wait for the user to enter the destination location
		dst = take_input(src);
		// Move the same elevator to the destination
		mov_elevator(dst);
		// Wait for the user to get out
		msdelay(2000);
	}
}	
