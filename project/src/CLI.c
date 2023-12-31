/**
 * @file CLI.c
 *
 * @brief Function Definitions for Command Line Interface (CLI) Module.
 *
 * This file includes the implementation of functions that correspond to the prototypes defined
 * in the `../headers/CLI.h` header file. The CLI module facilitates communication with a host terminal
 * and enables interactive command execution and feedback.
 *
 * @note Functions in this file correspond to the CLI module in `../headers/CLI.h`.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#include "../headers/CLI.h"
#include "../headers/USART.h"
#include "../headers/util.h"
#include "../headers/ELEVATOR.h"
#include <string.h>
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

uint16_t counter = 0;
extern QueueHandle_t xUP_REQUEST_Queue;
extern QueueHandle_t xDOWN_REQUEST_Queue;
extern QueueHandle_t xIN_ELEVATOR_BUTTONS_Queue;

void CLI_Transmit(uint8_t *pData, uint16_t Size)
{
	for(int i = 0	; i < Size; i++)
	{
		sendByte(pData[i]);
	}
}

void CLI_Receive(uint8_t *pData, int* id)
{
	switch(pData[*id])
	{
		case BACKSPACE:
			if(*id == 0) 
			{
				*id = *id - 1;
			}
			else 
			{
				*id = *id - 2;
				sendByte(BACKSPACE);
				sendByte(' ');
				sendByte(BACKSPACE);
			}
			break;
	
		case CARRIAGE_RETURN:
			if(parseReceivedData(pData, *id) != 1)
			{
				newPromptLine();
				*id = -1;
			}
			break;
				
		case SPACE:
			sendByte(SPACE);
			*id = -1;
			break;
			
		default:
			sendByte(pData[*id]);
			break;
		}
	*id = *id + 1;
}

int parseReceivedData(uint8_t *pData, int Size)
{
	sendByte(NEW_LINE_FEED);
	sendByte(CARRIAGE_RETURN);
	ElevatorInformation elevator;
	elevator.currentFloor = FIRST;
	elevator.targetFloor = FIRST;
	elevator.arrivalStatus = HOME;
	//enum floor targetFloor = FIRST;
	
	if(strncmp((char*)pData, "ledon\r", 6) == 0)
	{
		uint8_t buffer[] = "On-board led is ON";
		CLI_Transmit(buffer, sizeof(buffer));
		onboardLEDconfig(1);
		return 0;
	} 
	else if(strncmp((char*)pData, "ledoff\r", 7) == 0)
	{
		uint8_t buffer[] = "On-board led is OFF";
		CLI_Transmit(buffer, sizeof(buffer));
		onboardLEDconfig(0);
		return 0;
	}
	else if(strncmp((char*)pData, "ledstate\r", 8) == 0)
	{
		if(GPIOA->IDR & NUC_GREEN_ON)
		{
			uint8_t buffer[] = "led is on";
			CLI_Transmit(buffer, sizeof(buffer));
			return 0;
		}else 
		{
			uint8_t buffer[] = "led is off";
			CLI_Transmit(buffer, sizeof(buffer));
			return 0;
		}
	}
	else if(strncmp((char*)pData, "help\r", 5) == 0)
	{
		uint8_t buffer[] = "Current available commands:\n\rledon\n\rledoff\n\rledstate\n\rclear\n\rhelp\n\r\n\nElevator Algorithm is based on a Selective collective operation\n\rWhich is most common, remembers and answers calls in one direction\n\rthen reverses. When trip complete, programmed to return to a home landing.\n\r\n\rElevator Commands:\n\r\n\rFloor Buttons\n\r1u\n\r2u\n\r2d\n\r3u\n\r3d\n\r4u\n\r4d\n\r5u\n\r5d\n\r6u\n\r6d\n\r7u\n\r7d\n\r8d\n\r\nIn-elevator buttons:\n\r\n\r1\n\r2\n\r3\n\r4\n\r5\n\r6\n\r7\n\r8\n\r\n If a command is typed incorrectly an error prompt of 'invalid command' will appear.\n";
		CLI_Transmit(buffer, sizeof(buffer));
		return 0;
	}else if(strncmp((char*)pData, "clear\r", 6) == 0)
	{
		prepareTerminal();
		return 0;
	}else if(strncmp((char*)pData, "1u\r", 3) == 0)
	{
		elevator.targetFloor = FIRST;
		xQueueSendToBack(xUP_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}
	else if(strncmp((char*)pData, "2u\r", 3) == 0)
	{
		elevator.targetFloor = SECOND;
		xQueueSendToBack(xUP_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "2d\r", 3) == 0)
	{
		elevator.targetFloor = SECOND;
		xQueueSendToBack(xDOWN_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "3u\r", 3) == 0)
	{
		elevator.targetFloor = THIRD;
		xQueueSendToBack(xUP_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "3d\r", 3) == 0)
	{
		elevator.targetFloor = THIRD;
		xQueueSendToBack(xDOWN_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "4u\r", 3) == 0)
	{
		elevator.targetFloor = FOURTH;
		xQueueSendToBack(xUP_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "4d\r", 3) == 0)
	{
		elevator.targetFloor = FOURTH;
		xQueueSendToBack(xDOWN_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "5u\r", 3) == 0)
	{
		elevator.targetFloor = FIFTH;
		xQueueSendToBack(xUP_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "5d\r", 3) == 0)
	{
		elevator.targetFloor = FIFTH;
		xQueueSendToBack(xDOWN_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "6u\r", 3) == 0)
	{
		elevator.targetFloor = SIXTH;
		xQueueSendToBack(xUP_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "6d\r", 3) == 0)
	{
		elevator.targetFloor = SIXTH;
		xQueueSendToBack(xDOWN_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "7u\r", 3) == 0)
	{
		elevator.targetFloor = SEVENTH;
		xQueueSendToBack(xUP_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "7d\r", 3) == 0)
	{
		elevator.targetFloor = SEVENTH;
		xQueueSendToBack(xDOWN_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "8d\r", 3) == 0)
	{
		elevator.targetFloor = EIGHTH;
		xQueueSendToBack(xDOWN_REQUEST_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "1\r", 2) == 0)
	{
		elevator.targetFloor = FIRST;
		xQueueSendToBack(xIN_ELEVATOR_BUTTONS_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "2\r", 2) == 0)
	{
		elevator.targetFloor = SECOND;
		xQueueSendToBack(xIN_ELEVATOR_BUTTONS_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "3\r", 2) == 0)
	{
		elevator.targetFloor = THIRD;
		xQueueSendToBack(xIN_ELEVATOR_BUTTONS_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "4\r", 2) == 0)
	{
		elevator.targetFloor = FOURTH;
		xQueueSendToBack(xIN_ELEVATOR_BUTTONS_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "5\r", 2) == 0)
	{
		elevator.targetFloor = FIFTH;
		xQueueSendToBack(xIN_ELEVATOR_BUTTONS_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "6\r", 2) == 0)
	{
		elevator.targetFloor = SIXTH;
		xQueueSendToBack(xIN_ELEVATOR_BUTTONS_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "7\r", 2) == 0)
	{
		elevator.targetFloor = SEVENTH;
		xQueueSendToBack(xIN_ELEVATOR_BUTTONS_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "8\r", 2) == 0)
	{
		elevator.targetFloor = EIGHTH;
		xQueueSendToBack(xIN_ELEVATOR_BUTTONS_Queue, &elevator.targetFloor, 10);
		return 0;
	}else if(strncmp((char*)pData, "elevatorsequence1\r", 3) == 0)
	{
		//TODO
		return 0;
	}else if(strncmp((char*)pData, "elevatorsequence1\r", 3) == 0)
	{
		//TODO
		return 0;
	}	
	else
	{
		uint8_t buffer[] = "Unknown command:";
		CLI_Transmit(buffer, sizeof(buffer));
		CLI_Transmit(pData, Size);
		return 0;
	}
}

void prepareTerminal(void)
{
	sendEscapeAnsi(CLEAR_TERMINAL);
	sendEscapeAnsi(SET_SCROLLABLE_ROW);
	placeCursor(10,0);
	sendPromptArrows();
}

void updateStatusWindow(ElevatorInformation *elevator)
{
		counter++;

		sendEscapeAnsi(SAVE_CURSOR_POSITION);
	
		uint8_t clearBuffer[20]= "                    ";
	
		//display a counter - from lab5
		placeCursor(1,0);
		CLI_Transmit(clearBuffer, 20);
		placeCursor(1,0);
	
		uint8_t bigbuff[20];
		sprintf((char*)bigbuff, "counter:%d", counter);
		CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
	
		//display the current floor
		placeCursor(2,0);
		CLI_Transmit(clearBuffer, 20);
		placeCursor(2,0);
	
		sprintf((char*)bigbuff, "current-floor:%d", elevator->currentFloor+1);
		CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
	
		//display the target floor
		placeCursor(3,0);
		CLI_Transmit(clearBuffer, 20);
		placeCursor(3,0);
	
		sprintf((char*)bigbuff, "target-floor:%d", elevator->targetFloor+1);
		CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		
		//display the elevator-state arrival state
		placeCursor(4,0);
		CLI_Transmit(clearBuffer, 20);
		placeCursor(4,0);
		
		if (elevator->arrivalStatus == HOME)
		{
			sprintf((char*)bigbuff, "status:HOME");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		}		
		else if (elevator->arrivalStatus == OTW)
		{
			sprintf((char*)bigbuff, "status:OTW");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		}
		else if (elevator->arrivalStatus == ARRIVED_TO_REQUEST)
		{
			sprintf((char*)bigbuff, "ARRIVED_TO_REQUEST");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		}
		else if(elevator->arrivalStatus == ARRIVED)
		{
			sprintf((char*)bigbuff, "status:ARRIVED");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		}
		
		//display the elevator-state, direction
		placeCursor(5,0);
		CLI_Transmit(clearBuffer, 20);
		placeCursor(5,0);
		
		if(elevator->elevatorDirection == IDLE)
		{
			sprintf((char*)bigbuff, "elevator-dir:IDLE");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		} 
		else if(elevator->elevatorDirection == UP)
		{
			sprintf((char*)bigbuff, "elevator-dir:UP");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
			
		} 
		else if(elevator->elevatorDirection == DOWN)
		{
			sprintf((char*)bigbuff, "elevator-dir:DOWN");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		}
		
		//display if there's a passenger or not
		placeCursor(6,0);
		CLI_Transmit(clearBuffer, 20);
		placeCursor(6,0);
		
		if(elevator->someoneInElevator == YES)
		{
			sprintf((char*)bigbuff, "Passengers:YES");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		}
		else
		{
			sprintf((char*)bigbuff, "Passengers:NO");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		}
		
		//display the elevator-maintenance status
		placeCursor(7,0);
		CLI_Transmit(clearBuffer, 20);
		placeCursor(7,0);
		
		if(elevator->maintenanceStatus == TRUE)
		{
			sprintf((char*)bigbuff, "Maintenance-Mode!");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		}
		else
		{
			sprintf((char*)bigbuff, "elevator-functional!");
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
		}
	
		sendEscapeAnsi(RESTORE_CURSOR_POSITION);
}

void sendEscapeAnsi(uint16_t ANSI)
{
	uint8_t ansiBuffer[20];
	
	switch(ANSI)
	{
		case CLEAR_TERMINAL:
			sprintf((char*)ansiBuffer, "\x1b[2J");
			CLI_Transmit(ansiBuffer, strlen((char*)(ansiBuffer)));
			break;
		
		case SAVE_CURSOR_POSITION:
			sprintf((char*)ansiBuffer, "\x1b[s");
			CLI_Transmit(ansiBuffer, strlen((char*)(ansiBuffer)));
			break;
		
		case RESTORE_CURSOR_POSITION:
			sprintf((char*)ansiBuffer, "\x1b[u");
			CLI_Transmit(ansiBuffer, strlen((char*)(ansiBuffer)));
			break;
		
		case SET_SCROLLABLE_ROW:
			sprintf((char*)ansiBuffer, "\x1b[10;r");
			CLI_Transmit(ansiBuffer, strlen((char*)(ansiBuffer)));
			break;
				
		default:
			break;
	}
}

void placeCursor(int row, int col)
{
	uint8_t bigbuff[20];
	
	sprintf((char*)bigbuff, "\x1b[%d;%dH", row, col);
	CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
}

void sendPromptArrows(void)
{
	sendByte('>');
	sendByte('>');
}

void newPromptLine(void)
{
	sendByte(NEW_LINE_FEED);
	sendByte(CARRIAGE_RETURN);
	sendPromptArrows();
}
