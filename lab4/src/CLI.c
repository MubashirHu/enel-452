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

int cursor_row = ROW_OF_SCROLL_WINDOW;
int cursor_col = COL_OF_SCROLL_WINDOW;

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
	
		switch(pData[*id])
		{
			case BACKSPACE:
				if(*id == 0) 
				{
					*id = *id - 1;
					cursor_col = COL_OF_SCROLL_WINDOW;
				}
				else 
				{
					*id = *id - 2;
					sendByte(BACKSPACE);
					sendByte(' ');
					sendByte(BACKSPACE);
					cursor_col--;
				}
			break;
	
			case CARRIAGE_RETURN:
				if(parseReceivedData(pData, *id) != 1)
				{
					newPromptLine();
					*id = -1;
					cursor_row = cursor_row + 2;
					cursor_col = COL_OF_SCROLL_WINDOW;
				} else{
					*id = -1;
					placeCursor(ROW_OF_SCROLL_WINDOW, 0);
					sendPromptArrows();
					cursor_row = cursor_row + 2;
					cursor_col = COL_OF_SCROLL_WINDOW;
				}
			
			break;
				
			case SPACE:
				sendByte(SPACE);
				cursor_col++;
				*id = -1;
				
				break;
			default:
				sendByte(pData[*id]);
				cursor_col++;
			break;
		}
	*id = *id + 1;
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
		cursor_row++;
	}else if(strncmp((char*)pData, "clear\r", 6) == 0)
	{
		clearTerminal();
		cursor_row = ROW_OF_SCROLL_WINDOW-2;
		return 1;
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
void placeCursor(int row, int col)
{
	uint8_t bigbuff[20];
	
	sprintf((char*)bigbuff, "\x1b[%d;%dH", row, col);
	CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
}

void clearTerminal(void)
{
	uint8_t buffer[] = "\x1b[2J";
	CLI_Transmit(buffer, sizeof(buffer));
}

void prepareTerminal(void)
{
	clearTerminal();
	
	uint8_t set_scroll_row[] = "\x1b[8;r";
	CLI_Transmit(set_scroll_row, sizeof(set_scroll_row));
		
	placeCursor(ROW_OF_SCROLL_WINDOW,COL_OF_SCROLL_WINDOW-2);
	sendPromptArrows();
	updateCursorforCommandWindow();
}

void updateStatusWindow(void)
{
	
		uint8_t clearbuffer[20]= "                    ";
			
			//first data
			placeCursor(1,0);
			CLI_Transmit(clearbuffer, 20);
			placeCursor(1,0);
			uint8_t bigbuff[20];
			sprintf((char*)bigbuff, "cursor_row:%d", cursor_row);
			CLI_Transmit(bigbuff, strlen((char*)(bigbuff)));
	
			//second data
			placeCursor(2,0);
			CLI_Transmit(clearbuffer, 20);
			placeCursor(2,0);
			uint8_t bigbuff1[20];
			sprintf((char*)bigbuff1, "cursor_col:%d", cursor_col);
			CLI_Transmit(bigbuff1, strlen((char*)(bigbuff1)));
	
			updateCursorforCommandWindow();
}

void updateCursorforCommandWindow(void)
{
		placeCursor(cursor_row, cursor_col);
}