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
#include "../headers/TIM.h"
#include "../headers/CLI.h"
#include "../headers//util.h"
#include "stm32f10x.h"

volatile uint8_t DATA_RECEIVED_FLAG = 0; // Global declaration and initialization

int main(void)
{
	clockInit();
	serialOpen();
	prepareTerminal();
	initUSART2Interrupt();
	ledIOInit();
	initTIM2();
	
	uint8_t buffer[512];
	
	int bufferElementID = 0;
	sendPromptArrows();
	
	while(1)
	{
		if(DATA_RECEIVED_FLAG == 1)
		{
			CLI_Receive(buffer, &bufferElementID);
			DATA_RECEIVED_FLAG = 0;
		}
		
	}	
}

void USART2_IRQHandler(void) {
	DATA_RECEIVED_FLAG = 1;
	USART2->SR &= ~(USART_SR_RXNE);
}
