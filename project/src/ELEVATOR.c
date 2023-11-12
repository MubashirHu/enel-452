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
	
	//GPIOA->CRL |= GPIO_CRL_MODE5;
	//GPIOA->CRL &= ~GPIO_CRL_CNF5;
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
	// if the target floor is above the current floor
	if(elevator->targetFloor > elevator->currentFloor)
	{
			moveToUpperFloor(elevator);
	}
}

void processDownRequests(ElevatorInformation *elevator)
{
	// if the target floor is below the current floor
	if(elevator->targetFloor < elevator->currentFloor)
	{
			moveToLowerFloor(elevator);
	}
}

void checkForNewRequests(ElevatorInformation *elevator)
{
	
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

