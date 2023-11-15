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

enum arrival {
	ARRIVED,
	ARRIVED_TO_REQUEST,
	OTW,
	HOME
};

enum someoneInElevator {
	NO,
	YES
};

enum doorStatusMessage {
	OPENING,
	INSIDE_CLOSING,
	OUTSIDE_CLOSING,
	NONE
};

enum maintenanceMode {
	TRUE,
	FALSE
};

typedef struct 
{
	enum direction elevatorDirection;
	enum floor currentFloor;
	enum floor targetFloor;
	enum arrival arrivalStatus;
	enum someoneInElevator someoneInElevator; 
	enum doorStatusMessage doorMessage;
	enum maintenanceMode maintenanceStatus;
}ElevatorInformation;

/**
 * @brief Initializes GPIO pins for elevator control.
 *
 * This function initializes the GPIO pins required for elevator control.
 * It configures the necessary hardware resources to set up the elevator system.
 */
void initGPIOPinsForElevator(void);

/**
 * @brief Sets the LED corresponding to the specified floor on the MUX.
 *
 * This function turns on the LED indicator for the given floor to indicate the current position of
 * the elevator.
 *
 * @param 'floor' The floor for which the LED should be set.
 */
void setLED(int floor);

/**
 * @brief Processes upward elevator requests.
 *
 * This function processes requests for the elevator to move up, based on the provided elevator information.
 * Only ever called when the direction of the elevator is UP.
 *
 * @param '*elevator' A pointer to the ElevatorInformation structure.
 */
void processUpRequests(ElevatorInformation *elevator);

/**
 * @brief Processes downward elevator requests.
 *
 * This function processes requests for the elevator to move down, based on the provided elevator information.
 * Only ever called when the direction of the elevator is DOWN.
 *
 * @param '*elevator' A pointer to the ElevatorInformation structure.
 */
void processDownRequests(ElevatorInformation *elevator);

/**
 * @brief Returns the elevator to its home (FIRST) floor.
 *
 * This function instructs the elevator to return to its initial home position.
 *
 * @param '*elevator' A pointer to the ElevatorInformation structure.
 */
void goHome(ElevatorInformation *elevator);

/**
 * @brief Moves the elevator to the upper floor.
 *
 * This function increments the current floor. 
 * Called when trying to reach a target floor.
 *
 * @param '*elevator' A pointer to the ElevatorInformation structure.
 */
void moveToUpperFloor(ElevatorInformation *elevator);

/**
 * @brief Moves the elevator to the lower floor.
 *
 * This function increments the current floor. 
 * Called when trying to reach a target floor.
 *
 * @param '*elevator' A pointer to the ElevatorInformation structure.
 */
void moveToLowerFloor(ElevatorInformation *elevator);

/**
 * @brief Updates the LCD display
 *
 * This function updates the LCD display with the current floor information
 * and the direction that the elevator is headed in.
 *
 * @param '*elevator' A pointer to the ElevatorInformation structure.
 */
void updateLCDToNewFloor(ElevatorInformation *elevator);

/**
 * @brief Determines the direction of the elevator.
 *
 * This function determines the direction (up, down, or stationary(idle)) of the elevator 
 * based on its current state. This state is dependent on whether there are any requests or not. 
 *
 * @param '*elevator' A pointer to the ElevatorInformation structure.
 */
void determineElevatorDirection(ElevatorInformation *elevator);

/**
 * @brief Handles messages for passengers entering the elevator.
 *
 * This function displays messages and instructions for passengers entering the elevator
 * through the LCD.
 *
 * @param '*elevator' A pointer to the ElevatorInformation structure.
 */
void messagesEnteringElevator(ElevatorInformation *elevator);

/**
 * @brief Handles messages for passengers leaving the elevator.
 *
 * This function displays messages and instructions for passengers leaving the elevator
 * through the LCD.
 *
 * @param '*elevator' A pointer to the ElevatorInformation structure.
 */
void messagesLeavingElevator(ElevatorInformation *elevator);

/**
 * @brief Initializes the PB8 as an interrupt. 
 *
 * @param None
 */
void initMaintenanceButtonInterrupt(void);

/**
 * @brief Interrupt handler prototype
 *
 * @param None
 */
void EXTI9_5_IRQHandler(void);

#endif //ELEVATOR_H
