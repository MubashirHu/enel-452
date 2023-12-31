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
#include "../351/util.h"
#include <string.h>
#include "stm32f10x.h"
#include <stdio.h>
 
//Allows to Transmit data from the microcontroller to the host one byte at a time
void CLI_Transmit(uint8_t *pData, uint16_t Size)
{
	for(int i = 0	; i < Size; i++)
	{
		sendbyte(pData[i]);
	}
		
}

// Receives data into the buffer passed to parameter 1 which is set to a certain size
void CLI_Receive(uint8_t *pData, uint16_t Size)
{
	sendbyte('>');
	sendbyte('>');
	for ( int i = 0; i < Size; i++)
	{
		pData[i] = getbyte();
		sendbyte(pData[i]);
		
		if(pData[i] == 8)
		{
			if(i == 0)
			{
				//don't backspace when at the start of the buffer
			}else
			{
			i = i - 2;
			sendbyte(' ');
			sendbyte(8);
			}
		}
		
		// if Enter key is pressed, manually send a carriage and new line byte
		if(pData[i] == 10 || pData[i] == 13)
		{
			parseReceivedData(pData, i);
			sendbyte(13);
			sendbyte(10);
			i = -1;
			
			sendbyte('>');
			sendbyte('>');
			}		
	}
}

//parses the data stored in the buffer pointed to by the pData pointer.
void parseReceivedData(uint8_t *pData, int Size)
{
	sendbyte('>');
	sendbyte('>');
	sendbyte(10);
	sendbyte(13);
	
	if(strncmp((char*)pData, "ledon\r", Size) == 0)
	{
		uint8_t buffer[] = "On-board led is ON";
		CLI_Transmit(buffer, sizeof(buffer));
		onboardLEDconfig(1);		
	} 
	else if(strncmp((char*)pData, "ledoff\r", Size) == 0)
	{
		uint8_t buffer[] = "On-board led is OFF";
		CLI_Transmit(buffer, sizeof(buffer));
		onboardLEDconfig(0);
	}
	else if(strncmp((char*)pData, "ledstate\r", Size) == 0)
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
	else if(strncmp((char*)pData, "help\r", Size) == 0)
	{
		uint8_t buffer[] = "Help command Currently the commands available are 'ledon', 'ledoff', 'ledstate'. If a command is typed incorrectly an error prompt of 'invalid command' will show up. If the wrong command is typed, backspacing is available";
		CLI_Transmit(buffer, sizeof(buffer));
	}
	else
	{
		uint8_t buffer[] = "Unknown command";
		CLI_Transmit(buffer, sizeof(buffer));
	}
}