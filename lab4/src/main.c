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
volatile uint8_t TIM3_UPDATE_EVENT = 0; 

int main(void)
{
	clockInit();
	serialOpen();
	prepareTerminal();
	initUSART2Interrupt();
	ledIOInit();
	
	initTIM(2);
	initTIM(3);
	configTIM(3, 2000);
	initTIMInterrupt(3);

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
		
		if(TIM3_UPDATE_EVENT == 1)
		{
			//move cursor to the top
			uint8_t set_cursor_to_row[] = "\x1b[0;0H";
			CLI_Transmit(set_cursor_to_row, sizeof(set_cursor_to_row));
			
			//print out a statement
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
				//return back to the original position
				
			uint8_t set_cursor_back_to_row[] = "\x1b[8;0H";
			CLI_Transmit(set_cursor_back_to_row, sizeof(set_cursor_back_to_row));
				TIM3_UPDATE_EVENT = 0;
		}
		
	}
}

void USART2_IRQHandler(void) {
	DATA_RECEIVED_FLAG = 1;
	USART2->SR &= ~(USART_SR_RXNE);
}

void TIM3_IRQHandler(void) {
	TIM3_UPDATE_EVENT = 1;
	TIM3->SR &= ~(TIM_SR_UIF); // reset update event flag
}
