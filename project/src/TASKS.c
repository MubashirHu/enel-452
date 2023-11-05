/**
 * @file TASKS.c
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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


QueueHandle_t xCLI_Queue;
QueueHandle_t xBlinky_Queue;
uint8_t my_lcd_addr = 0x3f;

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
	//xTaskCreate(vLCDTask, "LCD task", configMINIMAL_STACK_SIZE+10, NULL, LCD_TASK_PRIORITY, NULL);
}

static void vBlinkTask(void * parameters) {
	uint16_t speed = 1000;
	while(1)
	{	
		if( xQueueReceive( xBlinky_Queue, &speed, 0 ) != pdPASS )
		{
			//no data in queue
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
		if(TIM3_UPDATE_EVENT == 1)
		{
			// UPDATE TERMINAL
			updateStatusWindow();
			TIM3_UPDATE_EVENT = 0;
		} 	
		if( xQueueReceive( xCLI_Queue, &buffer[bufferElementID], 10 ) != pdPASS )
		{
			// no data in queue
		}
		else 
		{
			// data in queue
			//buffer now contains the received data
			CLI_Receive(buffer, &bufferElementID);
				
			// send new speed to blinkytask queue
			uint16_t speed = (uint16_t)200;
			xQueueSendToFront( xBlinky_Queue, &speed, 10);			
		}
	}
}

static void vLCDTask(void * parameters)
{
	lcd_write_cmd(my_lcd_addr, LCD_LN1);	// Position cursor at beginning of line 1
	stringToLCD(my_lcd_addr, "Temp: ");
	//intToLCD(my_lcd_addr, temperature); 
	stringToLCD(my_lcd_addr, " Deg C   ");
	
}
