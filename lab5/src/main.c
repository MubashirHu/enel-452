/**
 * @file main.c
 *
 * @brief Main Program for STM32F103RB Microcontroller.
 *
 * This file comprises the primary program logic that is intended to be flashed onto the STM32F103RB board.
 * It serves as the central component, orchestrating the microcontroller's operations and functions. 
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#include "../headers/USART.h"
#include "../headers/TIM.h"
#include "../headers/CLI.h"
#include "../headers/util.h"
#include "../headers/TASKS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f10x.h"

// Global declaration and initialization
volatile uint8_t DATA_RECEIVED_FLAG = 0; 
volatile uint8_t TIM3_UPDATE_EVENT = 0;

int main(void)
{
	clockInit();
	serialOpen();
	prepareTerminal();
	initUSART2Interrupt();
	ledIOInit();
	
	initTIM(2);
	initTIM(3);
	configTIM(3, 1000);
	initTIMInterrupt(3);
		
	createQueues();
	createTasks();
	vTaskStartScheduler();
}

