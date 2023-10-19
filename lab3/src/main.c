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
	clockInit();
	serial_open();
	init_usart2_interrupt();
	led_IO_init();
	init_TIM2();
	uint8_t buffer[512];
	buffer[0] = '\0';
	int bufferElementID = 0;
	sendPromptArrows();
	
	while(1)
	{
		if(dataReceivedFlag == 1)
		{
			CLI_Receive(buffer, &bufferElementID);

			dataReceivedFlag = 0;
		}
	}	
}

void USART2_IRQHandler(void) {
	dataReceivedFlag = 1; // Set the flag to indicate character reception
	USART2->SR &= ~(USART_SR_RXNE);
}
