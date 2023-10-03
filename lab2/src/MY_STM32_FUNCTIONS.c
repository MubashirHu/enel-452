#include "stm32f10x.h"
#include "../headers/MY_STM32_FUNCTIONS.h"

void enablePort(char port)
{
	switch(port)
	{
		case 'A':
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
			break;
		case 'B':
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
			break;
		case 'C':
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;
			break;
		case 'D':
			RCC->APB2ENR |= RCC_APB2ENR_IOPDEN| RCC_APB2ENR_AFIOEN;
			break;
		case 'E':
			RCC->APB2ENR |= RCC_APB2ENR_IOPEEN| RCC_APB2ENR_AFIOEN;
			break;
		default:
			// set's nothing
			break;
	}
}

void disablePort(char port)
{
	switch(port)
	{
		case 'A':
			RCC->APB2ENR &= ~(RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN);
			break;
		case 'B':
			RCC->APB2ENR &= ~(RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN);
			break;
		case 'C':
			RCC->APB2ENR &= ~(RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN);
			break;
		case 'D':
			RCC->APB2ENR &= ~(RCC_APB2ENR_IOPDEN| RCC_APB2ENR_AFIOEN);
			break;
		case 'E':
			RCC->APB2ENR &= ~(RCC_APB2ENR_IOPEEN| RCC_APB2ENR_AFIOEN);
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
void disableUSART (int usart)
{
	switch(usart)
	{
		case 1:
			RCC->APB2ENR &= ~(RCC_APB2ENR_USART1EN);
			break;
		case 2:
			RCC->APB1ENR &= ~(RCC_APB1ENR_USART2EN);
			break;
		case 3:
			RCC->APB1ENR &= ~(RCC_APB1ENR_USART3EN);
			break;
	}
}


// work in progress
void configureGPIO(GPIO_Port port, GPIO_Pin pin, GPIO_Mode mode, GPIO_Cnf configuration)
{
	
	switch(port)
	{
		
		 uint32_t crl_crh_Offset = pin * 4; // Each pin occupies 4 bits in CRL/CRH
		
		case GPIO_PORTA:
			if (pin < 8)
			{
				int8_t result = ((configuration << 2) | mode );
				GPIOA->CRL |= (result << crl_crh_Offset);
			}
			else if ( pin < 16)
			{
				//GPIOx_CRH
				int8_t result = ((configuration << 2) | mode );
				GPIOA->CRL |= (result << pin);
			}
			else 
			{
					//ERROR
			}
			break;
		case GPIO_PORTB:
			break;
		case GPIO_PORTC:
			break;
		case GPIO_PORTD:
			break;
		case GPIO_PORTE:
			break;
	}
}