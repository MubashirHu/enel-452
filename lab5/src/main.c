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
#include "queue.h"
#include "stm32f10x.h"
#define BLINKY_TASK_PRIORITY 15
#define CLI_TASK_PRIORITY 1
#define CLI_QUEUE_LENGTH 1
#define CLI_QUEUE_ITEM_SIZE sizeof(uint8_t)
#define BLINKY_QUEUE_LENGTH 1
#define BLINKY_QUEUE_ITEM_SIZE sizeof(uint16_t)

// Global declaration and initialization
volatile uint8_t DATA_RECEIVED_FLAG = 0; 
volatile uint8_t TIM3_UPDATE_EVENT = 0;
QueueHandle_t xCLI_Queue;
QueueHandle_t xBlinky_Queue;

uint16_t blinky_speed = 1000;

static void vBlinkTask(void * parameters);
static void vCLITask(void * parameters);

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
		
	xCLI_Queue = xQueueCreate(CLI_QUEUE_LENGTH, CLI_QUEUE_ITEM_SIZE);
	if( xCLI_Queue == NULL )
	{
		/* The queue could not be created. */
		led_flash();
	}
	
	xBlinky_Queue = xQueueCreate(BLINKY_QUEUE_LENGTH, BLINKY_QUEUE_ITEM_SIZE);
	if( xBlinky_Queue == NULL )
	{
		/* The queue could not be created. */
		led_flash();
	}
	
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE+10,(void*) blinky_speed, BLINKY_TASK_PRIORITY, NULL);  
	xTaskCreate(vCLITask, "CLI task", configMINIMAL_STACK_SIZE+50, NULL, CLI_TASK_PRIORITY, NULL);
	vTaskStartScheduler();
}

void USART2_IRQHandler(void) {
	DATA_RECEIVED_FLAG = 1;
	USART2->SR &= ~(USART_SR_RXNE);	
	uint8_t characterReceived = USART2->DR;
	xQueueSendToFrontFromISR( xCLI_Queue, &characterReceived, NULL);
	DATA_RECEIVED_FLAG = 0;
}

void TIM3_IRQHandler(void) {
	TIM3_UPDATE_EVENT = 1;
	TIM3->SR &= ~(TIM_SR_UIF); // reset update event flag
}

static void vBlinkTask(void * parameters) {
	uint16_t speed = (uint16_t)parameters;
	while(1)
		{	
			// code doesn't execute here
			if( xQueueReceive( xBlinky_Queue, &speed, 0 ) != pdPASS )
			{
			}
			onboardLEDconfig(1);
			vTaskDelay(speed);
			onboardLEDconfig(0);
			vTaskDelay(speed);
		}
}

static void vCLITask(void * parameters)
{
	
	uint8_t buffer[50];
	uint8_t bufferElementID = 0;
	
	while(1)
	{		
		/* Nothing was received from the queue  even after blocking to wait for data to arrive. */
			if(TIM3_UPDATE_EVENT == 1)
			{
				// UPDATE TERMINAL
				updateStatusWindow();
				TIM3_UPDATE_EVENT = 0;
			} 
					
			// READ FROM QUEUE
			if( xQueueReceive( xCLI_Queue, &buffer[bufferElementID], 10 ) != pdPASS )
				{
				}
				else 
				{
					/* buffer now contains the received data. */
					CLI_Receive(buffer, &bufferElementID);
					uint16_t speed = (uint16_t)200;
					// TODO: sends characters to mainTask via Queue to change the frequency of the Blinky
					xQueueSendToFront( xBlinky_Queue, &speed, 10);			
				}
	}
}