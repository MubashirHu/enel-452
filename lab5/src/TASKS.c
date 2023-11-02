/**
 * @file TIM.c
 *
 * @brief Function Definitions for USART (Universal Synchronous Asynchronous Receiver-Transmitter) Module.
 *
 * This file includes the implementation of functions corresponding to the prototypes defined
 * in the `../headers/TIM.h` header file. This module is responsible for the timings in the sendByte
 * and getByte functions as well as the interrupt timers to update the terminal.
 *
 * @note Functions in this file align with the USART module in `../headers/TIM.h`.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#include "stm32f10x.h"
#include "../headers/util.h"
#include "../headers/CLI.h"
#include "../headers/TIM.h"
#include "../headers/TASKS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

QueueHandle_t xCLI_Queue;
QueueHandle_t xBlinky_Queue;

void createQueues(void)
{
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
}

void createTasks(void)
{
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE+10, NULL, BLINKY_TASK_PRIORITY, NULL);  
	xTaskCreate(vCLITask, "CLI task", configMINIMAL_STACK_SIZE+50, NULL, CLI_TASK_PRIORITY, NULL);
}

static void vBlinkTask(void * parameters) {
	uint16_t speed = 1000;
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
	int bufferElementID = 0;
	
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
					//led_flash();
					/* buffer now contains the received data. */
					CLI_Receive(buffer, &bufferElementID);
					uint16_t speed = (uint16_t)200;
					// TODO: sends characters to mainTask via Queue to change the frequency of the Blinky
					xQueueSendToFront( xBlinky_Queue, &speed, 10);			
				}
	}
}

void USART2_IRQHandler(void) {
	//DATA_RECEIVED_FLAG = 1;
	USART2->SR &= ~(USART_SR_RXNE);	
	uint8_t characterReceived = USART2->DR;
	if(xQueueSendToFrontFromISR( xCLI_Queue, &characterReceived, NULL) == pdTRUE)
	{
		
	}
	//DATA_RECEIVED_FLAG = 0;
}
