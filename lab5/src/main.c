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
#include "../headers//util.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x.h"
#define BLINKY_TASK_PRIORITY 5

volatile uint8_t DATA_RECEIVED_FLAG = 0; // Global declaration and initialization
volatile uint8_t TIM3_UPDATE_EVENT = 0;

static void vBlinkTask(void * parameters);

int main(void)
{
	clockInit();
	serialOpen();
	//prepareTerminal();
	initUSART2Interrupt();
	ledIOInit();
	
	//initTIM(2);
	//initTIM(3);
	//configTIM(3, 1000);
	//initTIMInterrupt(3);

	//uint8_t buffer[512];
	
	//int bufferElementID = 0;
	
	/*
	while(1)
	{
		if(DATA_RECEIVED_FLAG == 1)
		{
			CLI_Receive(buffer, &bufferElementID);
			DATA_RECEIVED_FLAG = 0;
		}
				
		if(TIM3_UPDATE_EVENT == 1)
		{
			updateStatusWindow();
			TIM3_UPDATE_EVENT = 0;
		}
	}
	*/
	
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE+10, NULL, BLINKY_TASK_PRIORITY, NULL);  
	 
	vTaskStartScheduler();
}

void USART2_IRQHandler(void) {
	DATA_RECEIVED_FLAG = 1;
	USART2->SR &= ~(USART_SR_RXNE);
}

void TIM3_IRQHandler(void) {
	TIM3_UPDATE_EVENT = 1;
	TIM3->SR &= ~(TIM_SR_UIF); // reset update event flag
}

static void vBlinkTask(void * parameters) {
	
	while(1)
	{
	onboardLEDconfig(1);
	vTaskDelay(1000);
	onboardLEDconfig(0);
	vTaskDelay(1000);
	}
}