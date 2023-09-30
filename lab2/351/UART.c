#include "UART.h"
#include "stm32f10x.h"


void init_UART1(void)
{
	//turn on AFIO, PORT A and USART1
	RCC->APB2ENR |=  RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN ;
	//RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	
	// Configure USART1 Tx (PA9) as alternate function push-pull
		GPIOA->CRH &= 0x00000000;
    GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9;
		
	// Configure USART1 Rx (PA10) as input floating
    GPIOA->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
    GPIOA->CRH |= GPIO_CRH_CNF10_0;
		
	// Configure USART1
		USART1->BRR = 0x09c4;
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE; // Enable Tx and Rx
    USART1->CR1 |= USART_CR1_UE; // Enable USART1
}

void USART1_SendChar(char c)
{
    // Wait until transmit buffer is empty
    while (!(USART1->SR & USART_SR_TXE));
    
    // Send character
    USART1->DR = c;
}

char USART1_ReceiveChar(void)
{
    // Wait until receive buffer is not empty
    while (!(USART1->SR & USART_SR_RXNE));
    
    // Return received character
    return USART1->DR;
}