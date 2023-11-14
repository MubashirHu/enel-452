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
QueueHandle_t xUP_REQUEST_Queue;
QueueHandle_t xDOWN_REQUEST_Queue;
QueueHandle_t xLCD_Queue;
QueueHandle_t xIN_ELEVATOR_BUTTONS_Queue;
uint8_t my_lcd_addr = 0x3f;

void createQueues(void)
{
	xCLI_Queue = xQueueCreate(CLI_QUEUE_LENGTH, CLI_QUEUE_ITEM_SIZE);
	xUP_REQUEST_Queue = xQueueCreate(ELEVATOR_UP_QUEUE_LENGTH, ELEVATOR_UP_QUEUE_ITEM_SIZE);
	xDOWN_REQUEST_Queue = xQueueCreate(ELEVATOR_DOWN_QUEUE_LENGTH, ELEVATOR_DOWN_QUEUE_ITEM_SIZE);
	xLCD_Queue = xQueueCreate(LCD_QUEUE_LENGTH, LCD_QUEUE_ITEM_SIZE);
	xIN_ELEVATOR_BUTTONS_Queue = xQueueCreate(IN_ELEVATOR_BUTTONS_QUEUE_LENGTH, IN_ELEVATOR_BUTTONS_QUEUE_ITEM_SIZE);
}

void createTasks(void)
{  
	xTaskCreate(vCLITask, "CLI task", configMINIMAL_STACK_SIZE+50, NULL, CLI_TASK_PRIORITY, NULL);
	xTaskCreate(vLCDTask, "LCD task", configMINIMAL_STACK_SIZE+100, NULL, LCD_TASK_PRIORITY, NULL);
	xTaskCreate(vELEVATORCONTROLTask, "Elevator task", configMINIMAL_STACK_SIZE+100, NULL, ELEVATOR_CONTROL_TASK_PRIORITY, NULL);
}

static void vCLITask(void * parameters)
{
	
	uint8_t buffer[50];
	int bufferElementID = 0;
	
	while(1)
	{		
		if( xQueueReceive( xCLI_Queue, &buffer[bufferElementID], 10 ) == pdPASS )
		{
			CLI_Receive(buffer, &bufferElementID);	
		}
	}
}

static void vLCDTask(void * parameters)
{
	ElevatorInformation elevator;
	uint16_t LCD_DELAY = 50;
	while(1)
	{
		
		if( xQueueReceive( xLCD_Queue, &elevator, 0 ) == pdPASS )
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
			}
			else if(elevator.elevatorDirection == UP)
			{
			stringToLCD(my_lcd_addr, "Dir:UP");
			vTaskDelay(LCD_DELAY);
			}
			else if(elevator.elevatorDirection == DOWN)
			{
			stringToLCD(my_lcd_addr, "Dir:DOWN");
			vTaskDelay(LCD_DELAY);
			}
			
			//intToLCD(my_lcd_addr, elevator.elevatorDirection); 
			vTaskDelay(LCD_DELAY);
		}
	}
}

static void vELEVATORCONTROLTask(void * parameters) {
	
	// default values
	ElevatorInformation elevator;
	elevator.currentFloor = FIRST;
	elevator.targetFloor = FIRST;
	elevator.elevatorDirection = IDLE;
	elevator.arrivalStatus = HOME;
	elevator.someoneInElevator = NO;
	uint16_t ELEVATOR_DELAY = 100;
	
	while(1)
	{		
		//FLOOR REQUESTS
		if(elevator.arrivalStatus == HOME && elevator.elevatorDirection == IDLE)
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
		else if(elevator.arrivalStatus == ARRIVED && elevator.elevatorDirection == IDLE)
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
		
		else if(elevator.arrivalStatus == ARRIVED && elevator.elevatorDirection != IDLE)
		{
			// if there is a up-request
			if(xQueueReceive( xUP_REQUEST_Queue, &elevator.targetFloor, 0 ) == pdPASS )
			{
				//then you set the direction to up
				elevator.elevatorDirection = UP;
			} 
			else
			{
				// there is no up-request 
				// if there is a down-request
				if( xQueueReceive( xDOWN_REQUEST_Queue, &elevator.targetFloor, 0 ) == pdPASS )
				{
					//then you set the direction to down
				elevator.elevatorDirection = DOWN;
				} 
				else
				{
					// there is no down-request
					elevator.elevatorDirection = IDLE;
				}
				
			}
		}
		
		//CONTROL LOGIC
		if (elevator.elevatorDirection == UP) {
        processUpRequests(&elevator);
				vTaskDelay(ELEVATOR_DELAY);
				updateLCDToNewFloor(&elevator);
				vTaskDelay(ELEVATOR_DELAY);
			
    } else if (elevator.elevatorDirection == DOWN) {
        processDownRequests(&elevator);
				vTaskDelay(ELEVATOR_DELAY);
				updateLCDToNewFloor(&elevator);
				vTaskDelay(ELEVATOR_DELAY);
			
    } else {
        checkForNewRequests(&elevator);
				vTaskDelay(ELEVATOR_DELAY);
				updateLCDToNewFloor(&elevator);
				vTaskDelay(ELEVATOR_DELAY);
    }
		
		if(TIM3_UPDATE_EVENT == 1)
		{
			updateStatusWindow(&elevator);
			TIM3_UPDATE_EVENT = 0;
		}
	}
}
