/**
 * @file TASKS.c
 *
 * @brief Function Definitions for TASKS Module.
 *
 * This file contains all of:
 * <1> Queue creation
 * <2> Task Creation
 * 
 * Which are used to achieve the objective of simulating a 2- Selective collective operation elevator.
 * 
 * Reference : http://www.electrical-knowhow.com/2012/04/elevator-control-system.html
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
QueueHandle_t xMAINTENANCE_MODE_Queue;
uint8_t my_lcd_addr = 0x3f;

void createQueues(void)
{
	xCLI_Queue = xQueueCreate(CLI_QUEUE_LENGTH, CLI_QUEUE_ITEM_SIZE);
	xUP_REQUEST_Queue = xQueueCreate(ELEVATOR_UP_QUEUE_LENGTH, ELEVATOR_UP_QUEUE_ITEM_SIZE);
	xDOWN_REQUEST_Queue = xQueueCreate(ELEVATOR_DOWN_QUEUE_LENGTH, ELEVATOR_DOWN_QUEUE_ITEM_SIZE);
	xLCD_Queue = xQueueCreate(LCD_QUEUE_LENGTH, LCD_QUEUE_ITEM_SIZE);
	xIN_ELEVATOR_BUTTONS_Queue = xQueueCreate(IN_ELEVATOR_BUTTONS_QUEUE_LENGTH, IN_ELEVATOR_BUTTONS_QUEUE_ITEM_SIZE);
	xMAINTENANCE_MODE_Queue = xQueueCreate(MAINTENANCE_MODE_QUEUE_LENGTH, MAINTENANCE_MODE_QUEUE_ITEM_SIZE);
}

void createTasks(void)
{  
	xTaskCreate(vCLITask, "CLI task", configMINIMAL_STACK_SIZE+250, NULL, CLI_TASK_PRIORITY, NULL);
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
			stringToLCD(my_lcd_addr, "Current-floor: ");
			vTaskDelay(LCD_DELAY);
			intToLCD(my_lcd_addr, elevator.currentFloor+1); 
			vTaskDelay(LCD_DELAY);
			lcd_write_cmd(my_lcd_addr, LCD_LN2);	// Position cursor at beginning of line 2
			vTaskDelay(LCD_DELAY);
			
			if(elevator.elevatorDirection == IDLE)
			{
			stringToLCD(my_lcd_addr, "Dir:IDLE        ");
			vTaskDelay(LCD_DELAY);
			}
			else if(elevator.elevatorDirection == UP)
			{
			stringToLCD(my_lcd_addr, "Dir:UP          ");
			vTaskDelay(LCD_DELAY);
			}
			else if(elevator.elevatorDirection == DOWN)
			{
			stringToLCD(my_lcd_addr, "Dir:DOWN");
			vTaskDelay(LCD_DELAY);
			}
			
			switch(elevator.doorMessage)
			{
				case OPENING:
					lcd_write_cmd(my_lcd_addr, LCD_LN1);	// Position cursor at beginning of line 1
					stringToLCD(my_lcd_addr, "OUTSIDE-ELEVATOR");
					lcd_write_cmd(my_lcd_addr, LCD_LN2);	// Position cursor at beginning of line 1
					stringToLCD(my_lcd_addr, "  DOORS-OPENING ");
					vTaskDelay(1000);
					break;
				
				case INSIDE_CLOSING:
					lcd_write_cmd(my_lcd_addr, LCD_LN1);	// Position cursor at beginning of line 1
					stringToLCD(my_lcd_addr, " INSIDE-ELEVATOR ");
					lcd_write_cmd(my_lcd_addr, LCD_LN2);	// Position cursor at beginning of line 1
					stringToLCD(my_lcd_addr, "  DOORS-CLOSING  ");
					vTaskDelay(1000);
					lcd_write_cmd(my_lcd_addr, LCD_LN1);	// Position cursor at beginning of line 1
					stringToLCD(my_lcd_addr, "  DOORS-CLOSED  ");
					lcd_write_cmd(my_lcd_addr, LCD_LN2);	// Position cursor at beginning of line 1
					stringToLCD(my_lcd_addr, "  CHOOSE-FLOOR  ");
					vTaskDelay(2000);
					break;
				
				case OUTSIDE_CLOSING:
					lcd_write_cmd(my_lcd_addr, LCD_LN1);	// Position cursor at beginning of line 1
					stringToLCD(my_lcd_addr, " OUTSIDE-ELEVATOR ");
					lcd_write_cmd(my_lcd_addr, LCD_LN2);	// Position cursor at beginning of line 1
					stringToLCD(my_lcd_addr, " DOORS-CLOSING ");
					vTaskDelay(1000);
					lcd_write_cmd(my_lcd_addr, LCD_LN1);	// Position cursor at beginning of line 1
					stringToLCD(my_lcd_addr, "  DOORS-CLOSED  ");
					lcd_write_cmd(my_lcd_addr, LCD_LN2);	// Position cursor at beginning of line 1
					stringToLCD(my_lcd_addr, "                ");
					vTaskDelay(2000);
					break;
					
				case NONE:
					break;
			}
			vTaskDelay(LCD_DELAY);
		}
	}
}

static void vELEVATORCONTROLTask(void * parameters) 
{
	// default values
	ElevatorInformation elevator;
	elevator.currentFloor = FIRST;
	elevator.targetFloor = FIRST;
	elevator.elevatorDirection = IDLE;
	elevator.arrivalStatus = HOME;
	elevator.someoneInElevator = NO;
	elevator.maintenanceStatus = FALSE;
	uint16_t ELEVATOR_DELAY = 500;
	
	while(1)
	{		
		//MAINTENANCE-MODE-INTERRUPT
		if( xQueueReceive( xMAINTENANCE_MODE_Queue, &elevator.maintenanceStatus, 10 ) == pdPASS )
		{
		}
		
		if(elevator.maintenanceStatus == TRUE)
		{
			elevator.elevatorDirection = IDLE;
		}
		
		//MANAGING-FLOOR-REQUESTS - MANAGING-DIRECTION-OF-ELEVATOR
		if(elevator.arrivalStatus == HOME && elevator.elevatorDirection == IDLE)
		{
			if(elevator.maintenanceStatus == FALSE)
			{
				determineElevatorDirection(&elevator);
			}
		}
		else if(elevator.arrivalStatus == ARRIVED && elevator.elevatorDirection == IDLE)
		{
			if(elevator.maintenanceStatus == FALSE)
			{
				determineElevatorDirection(&elevator);
			}
		}
		else if(elevator.arrivalStatus == ARRIVED && elevator.elevatorDirection != IDLE)
		{
			if(xQueueReceive( xUP_REQUEST_Queue, &elevator.targetFloor, 0 ) == pdPASS )
			{
				elevator.elevatorDirection = UP;
			} 
			else
			{
				if( xQueueReceive( xDOWN_REQUEST_Queue, &elevator.targetFloor, 0 ) == pdPASS )
				{
					elevator.elevatorDirection = DOWN;
				} 
				else
				{
					elevator.elevatorDirection = IDLE;
				}
			}
		}
		
		// ELEVATOR-CONTROL-LOGIC
		if (elevator.elevatorDirection == UP) 
		{
        processUpRequests(&elevator);
				updateLCDToNewFloor(&elevator);
    } 
		else if (elevator.elevatorDirection == DOWN) 
		{
        processDownRequests(&elevator);
				updateLCDToNewFloor(&elevator);
		
    } 
		else if(elevator.elevatorDirection == IDLE) 
		{
				elevator.targetFloor = FIRST;
        goHome(&elevator);
				updateLCDToNewFloor(&elevator);
    }
		
		if(TIM3_UPDATE_EVENT == 1)
		{
			updateStatusWindow(&elevator);
			TIM3_UPDATE_EVENT = 0;
		}
	
		vTaskDelay(ELEVATOR_DELAY);
	}
}
