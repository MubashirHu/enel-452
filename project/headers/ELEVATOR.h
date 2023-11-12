/**
 * @file ELEVATOR.h
 *
 * @brief Task Configuration for STM32 Communication with Host Terminal.
 *
 * This head holds the prototypes for Tasks that are static (only local to this module). As well as wrapper function
 * prototypes.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#ifndef ELEVATOR_H
#define ELEVATOR_H

enum direction {
	IDLE,
	DOWN,
	UP
};

enum floor {
	FIRST,
	SECOND,
	THIRD,
	FOURTH,
	FIFTH,
	SIXTH,
	SEVENTH,
	EIGHTH
};

void initGPIOPinsForElevator(void);
void setLED(int floor);
#endif //ELEVATOR_H
