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
 
void CLI_Transmit(uint8_t *pData, uint16_t Size)
{
	for(int i = 0	; i < Size; i++)
	{
		sendByte(pData[i]);
	}
}

void CLI_Receive(uint8_t *pData, int* id)
{
		pData[*id] = getByte();
		sendByte(pData[*id]);
	
		switch(pData[*id])
		{
			case BACKSPACE:
				onboardLEDconfig(1);
				if(*id == 0) 
				{
					break;
				}
				else 
				{
					*id = *id - 2;
					sendByte(' ');
					sendByte(BACKSPACE);
				}
			break;

			case NEW_LINE_FEED:
				parseReceivedData(pData, *id);
				newPromptLine();
				*id = -1;	
			break;
	
			case CARRIAGE_RETURN:
				parseReceivedData(pData, *id);
				newPromptLine();
				*id = -1;	
			break;
				
			default:
			break;
		}
	*id = *id + 1;
}

void parseReceivedData(uint8_t *pData, int Size)
{
	sendByte(NEW_LINE_FEED);
	sendByte(CARRIAGE_RETURN);
	
	if(strncmp((char*)pData, "ledon\r", 6) == 0)
	{
		uint8_t buffer[] = "On-board led is ON";
		CLI_Transmit(buffer, sizeof(buffer));
		onboardLEDconfig(1);		
	} 
	else if(strncmp((char*)pData, "ledoff\r", 7) == 0)
	{
		uint8_t buffer[] = "On-board led is OFF";
		CLI_Transmit(buffer, sizeof(buffer));
		onboardLEDconfig(0);
	}
	else if(strncmp((char*)pData, "ledstate\r", 8) == 0)
	{
		if(GPIOA->IDR & NUC_GREEN_ON)
		{
			uint8_t buffer[] = "led is on";
			CLI_Transmit(buffer, sizeof(buffer));
		}
		else 
		{
			uint8_t buffer[] = "led is off";
			CLI_Transmit(buffer, sizeof(buffer));
		}
	}
	else if(strncmp((char*)pData, "help\r", 5) == 0)
	{
		uint8_t buffer[] = "Help command Currently the commands available are 'ledon', 'ledoff', 'ledstate'. If a command is typed incorrectly an error prompt of 'invalid command' will show up. If the wrong command is typed, backspacing is available";
		CLI_Transmit(buffer, sizeof(buffer));
	}else if(strncmp((char*)pData, "clear\r", 6) == 0)
	{
		uint8_t buffer[] = "\x1b[2J";
		onboardLEDconfig(1);
		CLI_Transmit(buffer, sizeof(buffer));		
	}
	else
	{
		uint8_t buffer[] = "Unknown command:";
		CLI_Transmit(buffer, sizeof(buffer));
		CLI_Transmit(pData, Size);
	}
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

void prepareTerminal(void)
{
	uint8_t set_cursor_to_row[] = "\x1b[10;r";
	CLI_Transmit(set_cursor_to_row, sizeof(set_cursor_to_row));
	
	uint8_t set_row_to_roll_off[] = "\x1b[10;0H";
	CLI_Transmit(set_row_to_roll_off, sizeof(set_row_to_roll_off));
}