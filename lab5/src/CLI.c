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
#include <string.h>
#include "stm32f10x.h"
#include <stdio.h>

uint16_t counter = 0;

void CLI_Transmit(uint8_t *pData, uint16_t Size)
{
	for(int i = 0	; i < Size; i++)
	{
		sendByte(pData[i]);
	}
}

int CLI_Receive(uint8_t *pData, int* id, int* speed)
{
	int result;
	
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
			result = parseReceivedData(pData, *id);
			if(result != 0)
			{
				newPromptLine();
				*id = 0;
				*speed = result;
				return *speed;
			}
			else{
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
		return 0;
}

int parseReceivedData(uint8_t *pData, int Size)
{
	sendByte(NEW_LINE_FEED);
	sendByte(CARRIAGE_RETURN);
	
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
		}
		else 
		{
			uint8_t buffer[] = "led is off";
			CLI_Transmit(buffer, sizeof(buffer));
			return 0;
		}
	}
	else if(strncmp((char*)pData, "help\r", 5) == 0)
	{
		uint8_t buffer[] = "Commands available are \n\rhelp\n\rledon\n\rledoff\n\rledstate\n\rclear\n\rled200\n\rled500\n\rled1000\n\rnote: backspaces are available";
		CLI_Transmit(buffer, sizeof(buffer));
		return 0;
	}else if(strncmp((char*)pData, "clear\r", 6) == 0)
	{
		sendEscapeAnsi(CLEAR_TERMINAL);
		prepareTerminal();
		return 0;
	}
	else if(strncmp((char*)pData, "led200\r", 7) == 0)
	{
		uint8_t buffer[] = "ledspeed is set to 200ms tick";
		CLI_Transmit(buffer, sizeof(buffer));
		onboardLEDconfig(0);
		return 200;
	}else if(strncmp((char*)pData, "led500\r", 7) == 0)
	{
		uint8_t buffer[] = "ledspeed is set to 500ms tick";
		CLI_Transmit(buffer, sizeof(buffer));
		onboardLEDconfig(0);
		return 500;
	}else if(strncmp((char*)pData, "led1000\r", 8) == 0)
	{
		uint8_t buffer[] = "ledspeed is set to 1000ms tick";
		CLI_Transmit(buffer, sizeof(buffer));
		onboardLEDconfig(0);
		return 1000;
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
	placeCursor(8,0);
	sendPromptArrows();
}

void updateStatusWindow(void)
{
		counter++;
	
		sendEscapeAnsi(SAVE_CURSOR_POSITION);
	
		uint8_t clearBuffer[20]= "                    ";
			
		placeCursor(1,0);
		CLI_Transmit(clearBuffer, 20);
		placeCursor(1,0);
	
		uint8_t bigbuff[20];
		sprintf((char*)bigbuff, "counter:%d", counter);
		CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
					
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
			sprintf((char*)ansiBuffer, "\x1b[8;r");
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
