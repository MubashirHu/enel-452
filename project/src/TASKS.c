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
#include "../headers/ELEVATOR.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


QueueHandle_t xCLI_Queue;
QueueHandle_t xBlinky_Queue;
QueueHandle_t xMux_Queue;
QueueHandle_t xUP_REQUEST_Queue;
QueueHandle_t xDOWN_REQUEST_Queue;
QueueHandle_t xCURRENT_FLOOR_Queue;
QueueHandle_t xLCD_Queue;
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
	
	xMux_Queue = xQueueCreate(MUX_QUEUE_LENGTH, MUX_QUEUE_ITEM_SIZE);
	if( xMux_Queue == NULL )
	{
		/* The queue could not be created. */
		led_flash();
	}
	
	xUP_REQUEST_Queue = xQueueCreate(ELEVATOR_UP_QUEUE_LENGTH, ELEVATOR_UP_QUEUE_ITEM_SIZE);
	if( xUP_REQUEST_Queue == NULL )
	{
		/* The queue could not be created. */
		led_flash();
	}
	
	xDOWN_REQUEST_Queue = xQueueCreate(ELEVATOR_DOWN_QUEUE_LENGTH, ELEVATOR_DOWN_QUEUE_ITEM_SIZE);
	if( xDOWN_REQUEST_Queue == NULL )
	{
		/* The queue could not be created. */
		led_flash();
	}
	
	xCURRENT_FLOOR_Queue = xQueueCreate(CURRENT_FLOOR_QUEUE_LENGTH, CURRENT_FLOOR_QUEUE_ITEM_SIZE);
	if( xCURRENT_FLOOR_Queue == NULL )
	{
		/* The queue could not be created. */
		led_flash();
	}
	
	xLCD_Queue = xQueueCreate(LCD_QUEUE_LENGTH, LCD_QUEUE_ITEM_SIZE);
	if( xLCD_Queue == NULL )
	{
		/* The queue could not be created. */
		led_flash();
	}
}

void createTasks(void)
{
	xTaskCreate(vBlinkTask, "Blinky", configMINIMAL_STACK_SIZE+10, NULL, BLINKY_TASK_PRIORITY, NULL);  
	xTaskCreate(vCLITask, "CLI task", configMINIMAL_STACK_SIZE+50, NULL, CLI_TASK_PRIORITY, NULL);
	xTaskCreate(vLCDTask, "LCD task", configMINIMAL_STACK_SIZE+100, NULL, LCD_TASK_PRIORITY, NULL);
	xTaskCreate(vMUXTask, "MUX task", configMINIMAL_STACK_SIZE+100, NULL, MUX_TASK_PRIORITY, NULL);
	xTaskCreate(vELEVATORCONTROLTask, "Elevator task", configMINIMAL_STACK_SIZE+100, NULL, ELEVATOR_CONTROL_TASK_PRIORITY, NULL);
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
	ElevatorInformation elevator;
	uint16_t LCD_DELAY = 50;
	while(1)
	{
		
		if( xQueueReceive( xLCD_Queue, &elevator, 0 ) != pdPASS )
		{
			//no data in queue
		}
		else
		{
			lcd_write_cmd(my_lcd_addr, LCD_LN1);	// Position cursor at beginning of line 1
			vTaskDelay(LCD_DELAY);
			stringToLCD(my_lcd_addr, "current-floor: ");
			vTaskDelay(LCD_DELAY);
			intToLCD(my_lcd_addr, elevator.currentFloor+1); 
			vTaskDelay(LCD_DELAY);
			lcd_write_cmd(my_lcd_addr, LCD_LN2);	// Position cursor at beginning of line 2
			vTaskDelay(LCD_DELAY);
			
			if(elevator.elevatorDirection == IDLE)
			{
			stringToLCD(my_lcd_addr, "Dir:IDLE");
			vTaskDelay(LCD_DELAY);
			}else 
			if(elevator.elevatorDirection == UP)
			{
			stringToLCD(my_lcd_addr, "Dir:UP");
			vTaskDelay(LCD_DELAY);
			}else if(elevator.elevatorDirection == DOWN)
			{
			stringToLCD(my_lcd_addr, "Dir:DOWN");
			vTaskDelay(LCD_DELAY);
			}
			
			//intToLCD(my_lcd_addr, elevator.elevatorDirection); 
			vTaskDelay(LCD_DELAY);
		}
	}
}

static void vMUXTask(void * parameters)
{
	ElevatorInformation elevator;
	elevator.currentFloor = FIRST;
	elevator.targetFloor = FIRST;
	uint16_t MUX_TASK_DELAY = 800;
	
	while(1)
	{
		//recieves target floor over queue
		if( xQueueReceive( xMux_Queue, &elevator.targetFloor, 0 ) == pdPASS )
		{
			// Move up
			if(elevator.targetFloor > elevator.currentFloor)
			{
				elevator.currentFloor++;
				vTaskDelay(MUX_TASK_DELAY);
				setLED(elevator.currentFloor);
				vTaskDelay(MUX_TASK_DELAY);
			}
			
			// Move down
			if(elevator.targetFloor < elevator.currentFloor)
			{
				elevator.currentFloor--;
				vTaskDelay(MUX_TASK_DELAY);
				setLED(elevator.currentFloor);
				vTaskDelay(MUX_TASK_DELAY);
			}
			
			// Do not move
			if(elevator.targetFloor == elevator.currentFloor)
			{
				setLED(elevator.currentFloor);
				vTaskDelay(MUX_TASK_DELAY);
			}
		}
	}
}

static void vELEVATORCONTROLTask(void * parameters) {
	
	// default values
	ElevatorInformation elevator;
	elevator.currentFloor = FIRST;
	elevator.targetFloor = FIRST;
	elevator.elevatorDirection = IDLE;
	elevator.arrivalStatus = ARRIVED;
	
	while(1)
	{		
		//FLOOR REQUESTS
		if(elevator.arrivalStatus == ARRIVED)
		{
			if(xQueueReceive( xUP_REQUEST_Queue, &elevator.targetFloor, 0 ) == pdPASS )
			{
				elevator.elevatorDirection = UP;
			}
	
			if( xQueueReceive( xDOWN_REQUEST_Queue, &elevator.targetFloor, 0 ) == pdPASS )
			{
				elevator.elevatorDirection = DOWN;
			}
		}
		
		//CONTROL LOGIC
		if (elevator.elevatorDirection == UP) {
        processUpRequests(&elevator);
				vTaskDelay(500);
				updateLCDToNewFloor(&elevator);
				vTaskDelay(500);
    } else if (elevator.elevatorDirection == DOWN) {
        processDownRequests(&elevator);
				vTaskDelay(500);
				updateLCDToNewFloor(&elevator);
				vTaskDelay(500);
    } else {
        checkForNewRequests(&elevator);
				vTaskDelay(500);
				updateLCDToNewFloor(&elevator);
				vTaskDelay(500);
    }
	}
}
