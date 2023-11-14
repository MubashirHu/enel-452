/**
 * @file ELEVATOR.c
 *
 * @brief Function Definitions for USART (Universal Synchronous Asynchronous Receiver-Transmitter) Module.
 *
 * This file includes the implementation of functions corresponding to the prototypes defined
 * in the `../headers/Tasks.h` header file. This module is responsible for making all the queue connections
 * between tasks. As well as creating the various tasks that are to be used. 
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#include "stm32f10x.h"
#include "../headers/util.h"
#include "../headers/CLI.h"
#include "../headers/TIM.h"
#include "../headers/TASKS.h"
#include "../headers/i2c.h"
#include "../headers/i2c_lcd_driver.h"
#include "../headers/MY_STM32_FUNCTIONS.h"
#include "../headers/ELEVATOR.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern QueueHandle_t xLCD_Queue;
extern QueueHandle_t xIN_ELEVATOR_BUTTONS_Queue;
extern QueueHandle_t xUP_REQUEST_Queue;
extern QueueHandle_t xDOWN_REQUEST_Queue;


void initGPIOPinsForElevator(void)
{
	enablePort('A');
	
	//Configure PA8 for general purpose push-pull, max 50MHz b1011
	GPIOA->CRH |= GPIO_CRH_MODE8;
	GPIOA->CRH &= ~GPIO_CRH_CNF8;
	

	//Configure PA9 for general purpose push-pull, max 50MHz b1011
	GPIOA->CRH |= GPIO_CRH_MODE9;
	GPIOA->CRH &= ~GPIO_CRH_CNF9;
	
	//Configure PA10 for general purpose push-pull, max 50MHz b1011
	GPIOA->CRH |= GPIO_CRH_MODE10;
	GPIOA->CRH &= ~GPIO_CRH_CNF10;
	
	//Configure PA11 for Input mode with Input with pull-up / pull-down 
	
	//GPIOA->CRH &= ~GPIO_CRH_MODE11;
	//GPIOA->CRH |= GPIO_CRH_CNF11_1;
	//GPIOA->CRH |= 0x00008000;
	
	//Configure PB4 for Input mode with Input with pull-up / pull-down 
	//GPIOB->CRL |= GPIO_CRL_CNF4_0;
	
	//Configure PB8 for Input mode with Input with pull-up / pull-down 
	GPIOB->CRH |= GPIO_CRH_CNF8_0;
	
	
}

void setLED(int floor)
{	
	switch(floor)
	{
		case 0:	
			GPIOA->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8);
			break;
		case 1:	
			GPIOA->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8);
			GPIOA->ODR |= GPIO_ODR_ODR8;
			break;
		case 2:	
			GPIOA->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8);
			GPIOA->ODR |= GPIO_ODR_ODR9;
			break;
		case 3:	
			GPIOA->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8);
			GPIOA->ODR |= GPIO_ODR_ODR9 | GPIO_ODR_ODR8;
			break;
		case 4:	
			GPIOA->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8);
			GPIOA->ODR |= GPIO_ODR_ODR10;
			break;
		case 5:	
			GPIOA->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8);
			GPIOA->ODR |= GPIO_ODR_ODR10 | GPIO_ODR_ODR8;
			break;
		case 6:	
			GPIOA->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8);
			GPIOA->ODR |= GPIO_ODR_ODR10 | GPIO_ODR_ODR9;
			break;
		case 7:	
			GPIOA->ODR &= ~(GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8);
			GPIOA->ODR |= GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8;
			break;
	}
}

void processUpRequests(ElevatorInformation *elevator)
{
	if(elevator->someoneInElevator == NO)
	{
		// if you need to go up
		if(elevator->targetFloor > elevator->currentFloor)
		{
				// go up
				moveToUpperFloor(elevator);
		}
		
		if(elevator->targetFloor < elevator->currentFloor)
		{
				// go down
				moveToLowerFloor(elevator);
		}
	
		// if you reached your floor
		if(elevator->targetFloor == elevator->currentFloor)
		{
			elevator->someoneInElevator = YES;
			
			messagesEnteringElevator(elevator);
			if(xQueueReceive( xIN_ELEVATOR_BUTTONS_Queue, &elevator->targetFloor, 0 ) == pdPASS )
			{
				if(elevator->targetFloor > elevator->currentFloor)
				{
					elevator->elevatorDirection = UP;
				}

			}
		}
	}
	
	else if(elevator->someoneInElevator == YES)
	{
		// if the target floor is above the current floor
		if(elevator->targetFloor > elevator->currentFloor)
		{
				moveToUpperFloor(elevator);
		}
	
		if(elevator->targetFloor == elevator->currentFloor)
		{
			elevator->arrivalStatus = ARRIVED;
			elevator->someoneInElevator = NO;
			messagesLeavingElevator(elevator);
		}
		else
		{
			elevator->arrivalStatus = OTW;
		}
	}
}

void processDownRequests(ElevatorInformation *elevator)
{
	if(elevator->someoneInElevator == NO)
	{
		// if you need to go down
		if(elevator->targetFloor < elevator->currentFloor)
		{
				// go down
				moveToLowerFloor(elevator);
		}
		
		if(elevator->targetFloor > elevator->currentFloor)
		{
			moveToUpperFloor(elevator);
		}
	
		// if you reached your floor
		if(elevator->targetFloor == elevator->currentFloor)
		{
			elevator->someoneInElevator = YES;
			
			vTaskDelay(500);
			//get a destination to go to next
			messagesEnteringElevator(elevator);
			if(xQueueReceive( xIN_ELEVATOR_BUTTONS_Queue, &elevator->targetFloor, 0 ) == pdPASS )
			{
				if(elevator->targetFloor < elevator->currentFloor)
				{
					elevator->elevatorDirection = DOWN;
				}
			}
		}
	}
	
	else if(elevator->someoneInElevator == YES)
	{
		// if the target floor is below the current floor
		if(elevator->targetFloor < elevator->currentFloor)
		{
				moveToLowerFloor(elevator);
		}
	
		if(elevator->targetFloor == elevator->currentFloor)
		{
			elevator->arrivalStatus = ARRIVED;
			elevator->someoneInElevator = NO;
			messagesLeavingElevator(elevator);
		}
		else
		{
			elevator->arrivalStatus = OTW;
		}
	}
}

void goHome(ElevatorInformation *elevator)
{
	if(elevator->targetFloor < elevator->currentFloor)
	{
		moveToLowerFloor(elevator);
	}
	
	if(elevator->targetFloor == elevator->currentFloor)
	{
		elevator->arrivalStatus = HOME;
	}
}

void moveToUpperFloor(ElevatorInformation *elevator)
{
	elevator->currentFloor++;
	setLED(elevator->currentFloor);
}

void moveToLowerFloor(ElevatorInformation *elevator)
{
	elevator->currentFloor--;
	setLED(elevator->currentFloor);
}

void updateLCDToNewFloor(ElevatorInformation *elevator)
{
	xQueueSendToFront( xLCD_Queue, elevator, 10);
}

void determineElevatorDirection(ElevatorInformation *elevator)
{
	if(xQueueReceive( xUP_REQUEST_Queue, &elevator->targetFloor, 0 ) == pdPASS )
	{
		elevator->elevatorDirection = UP;
	}
	
	if( xQueueReceive( xDOWN_REQUEST_Queue, &elevator->targetFloor, 0 ) == pdPASS )
	{
		elevator->elevatorDirection = DOWN;
	}
}

void messagesEnteringElevator(ElevatorInformation *elevator)
{
	elevator->doorMessage = OPENING;
			updateLCDToNewFloor(elevator);
			vTaskDelay(1500);
			elevator->doorMessage = INSIDE_CLOSING;
			updateLCDToNewFloor(elevator);
			vTaskDelay(1500);
			elevator->doorMessage = NONE;
			updateLCDToNewFloor(elevator);
			vTaskDelay(2000);
}

void messagesLeavingElevator(ElevatorInformation *elevator)
{
	elevator->doorMessage = OUTSIDE_CLOSING;
	updateLCDToNewFloor(elevator);
	vTaskDelay(1500);
	elevator->doorMessage = NONE;
	updateLCDToNewFloor(elevator);
	vTaskDelay(2000);
}

void readElevator(void)
{
	if ((GPIOB->IDR & GPIO_IDR_IDR8) == 0) {
						//PB8 IS LOW
						//button pressed
						//pinState = 0;
				} else {
						// PA11 is low
						// button released
						//pinState = 1;
				}
}

