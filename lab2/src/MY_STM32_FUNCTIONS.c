#include "stm32f10x.h"
#include "../headers/MY_STM32_FUNCTIONS.h"

void enablePort(char port)
{
	switch(port)
	{
		case 'A':
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
			break;
		case 'B':
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
			break;
		case 'C':
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
			break;
		case 'D':
			RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
			break;
		case 'E':
			RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
			break;
		default:
			// set's nothing
			break;
	}
}

void enableUSART (int usart)
{
	switch(usart)
	{
		case 1:
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
			break;
		case 2:
			RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
			break;
		case 3:
			RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
			break;
	}
}

void configureGPIO(char port, int pin, char io, int configuration );
{
	
	
}