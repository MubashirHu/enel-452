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
#include "../headers/CLI.h"
#include "../headers//util.h"
#include "stm32f10x.h"

volatile uint8_t dataReceivedFlag = 0; // Global declaration and initialization

int main(void)
{
	//init
	clockInit();
	serial_open();
	init_usart2_interrupt();
	led_IO_init();
	init_TIM2();
	char receivedData[512];
	receivedData[0] = '\0';
	int i = 0;
	
	while(1)
	{
		
		if(dataReceivedFlag == 1)
		{
			receivedData[i] = getbyte();
			sendbyte(receivedData[i]);
			
			if(receivedData[i] == BACKSPACE)
			{
				if(i == 0)
				{
					//don't backspace when at the start of the buffer
				}else
				{
					i = i - 2;
					sendbyte(' ');
					sendbyte(BACKSPACE);
				}
			}
		
			// if Enter key is pressed, manually send a carriage and new line byte
			if(receivedData[i] == NEW_LINE_FEED || receivedData[i] == CARRIAGE_RETURN)
			{
				parseReceivedData(receivedData, i);
				sendbyte(CARRIAGE_RETURN);
				sendbyte(NEW_LINE_FEED);
				i = -1;
				
				sendbyte('>');
				sendbyte('>');
			}
	
			//led_blink();
			i++;
			dataReceivedFlag = 0;
		}
	}	
}

void USART2_IRQHandler(void) {
	dataReceivedFlag = 1; // Set the flag to indicate character reception
	USART2->SR &= ~(USART_SR_RXNE);
}
