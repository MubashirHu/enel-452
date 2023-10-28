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
#define BLINKY_TASK_PRIORITY 5
#define CLI_TASK_PRIORITY 5
#define USART2_QUEUE_LENGTH 512
#define USART2_QUEUE_ITEM_SIZE sizeof(uint8_t)

volatile uint8_t DATA_RECEIVED_FLAG = 0; // Global declaration and initialization
QueueHandle_t xQueue;
uint8_t buffer[512];
uint8_t bufferElementID = 0;

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
	
	xQueue = xQueueCreate(USART2_QUEUE_LENGTH, USART2_QUEUE_ITEM_SIZE);
	if( xQueue == NULL )
	{
		/* The queue could not be created. */
		led_flash();
	}
	
	//xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE+10, NULL, BLINKY_TASK_PRIORITY, NULL);  
	xTaskCreate(vCLITask, "CLI task", configMINIMAL_STACK_SIZE+10,(void *) buffer, CLI_TASK_PRIORITY, NULL);  
	
	vTaskStartScheduler();
}

void USART2_IRQHandler(void) {
	DATA_RECEIVED_FLAG = 1;
	USART2->SR &= ~(USART_SR_RXNE);	
	uint8_t characterReceived = USART2->DR;
	xQueueSendToFrontFromISR( xQueue, &characterReceived, NULL);
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

static void vCLITask(void * parameters)
{
	
	while(1)
	{
		// UPDATE TERMINAL
		updateStatusWindow();
		vTaskDelay(500);
		
		if(DATA_RECEIVED_FLAG == 1)
		{
			// READ FROM QUEUE
			if( xQueueReceive( xQueue, &buffer, portMAX_DELAY ) != pdPASS )
				{
					/* Nothing was received from the queue – even after blocking to wait for data to arrive. */
				}
				else 
				{
					/* xMessage now contains the received data. */
					sendByte(buffer[bufferElementID]);
					DATA_RECEIVED_FLAG = 0;
				}
			// sends characters to mainTask via Queue to change the frequency of the Blinky
		}
	}
}