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
#define CLI_TASK_PRIORITY 1
#define CLI_QUEUE_LENGTH 512
#define CLI_QUEUE_ITEM_SIZE sizeof(uint8_t)
#define BLINKY_QUEUE_LENGTH 512
#define BLINKY_QUEUE_ITEM_SIZE sizeof(uint8_t)

// Global declaration and initialization
volatile uint8_t DATA_RECEIVED_FLAG = 0; 
volatile uint8_t TIM3_UPDATE_EVENT = 0;
QueueHandle_t xCLI_Queue;
QueueHandle_t xBlinky_Queue;

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
	
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE+10, NULL, BLINKY_TASK_PRIORITY, NULL);  
	xTaskCreate(vCLITask, "CLI task", configMINIMAL_STACK_SIZE+10,(void *) buffer, CLI_TASK_PRIORITY, NULL);  
	
	vTaskStartScheduler();
}

void USART2_IRQHandler(void) {
	DATA_RECEIVED_FLAG = 1;
	USART2->SR &= ~(USART_SR_RXNE);	
	uint8_t characterReceived = USART2->DR;
	xQueueSendToFrontFromISR( xCLI_Queue, &characterReceived, NULL);
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

static void vCLITask(void * parameters)
{
	while(1)
	{
		if(TIM3_UPDATE_EVENT == 1)
		{
			// UPDATE TERMINAL
			updateStatusWindow();
			TIM3_UPDATE_EVENT = 0;
		}
		
		if(DATA_RECEIVED_FLAG == 1)
		{
			// READ FROM QUEUE
			if( xQueueReceive( xCLI_Queue, &buffer[bufferElementID], portMAX_DELAY ) != pdPASS )
				{
					/* Nothing was received from the queue – even after blocking to wait for data to arrive. */
				}
				else 
				{
					/* xMessage now contains the received data. */
					CLI_Receive(buffer, &bufferElementID);
					uint8_t speed = (uint8_t)1000;
					// TODO: sends characters to mainTask via Queue to change the frequency of the Blinky
					xQueueSendToFront( xBlinky_Queue, &speed, 10);			
					DATA_RECEIVED_FLAG = 0;
				}
		}
	}
}