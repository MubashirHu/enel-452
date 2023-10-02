#include "stm32f10x.h"
#include "../headers/USART.h"
#include "../headers/MY_STM32_FUNCTIONS.h"

void initializeUSART(void)
{
	//Enable port A
	enablePort('A');
	
	//Enable USART2 CLOCK
	enableUSART(2);
	
	//Configure PA2 for alternate function output push pull mode, max 50MHz b1011
	GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2;
	
	//Configure PA3 for Input with pull-up / pull-down b1000
	GPIOA->CRL |= GPIO_CRL_CNF3_1;
	
	//Enable the USART Tx and Rx in the USART Control register.
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE; //Enable Tx and Rx
	USART2->CR1 |= USART_CR1_UE; // Enable USART
	
	//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz)
	//USART2->BRR = 0x0139;
	USART2->BRR = 0x09c4;
}

//Create a loop to send a character to the host and make sure that character is printed. This will test the Tx.
void sendbyte(uint8_t c)
{
	// Wait until transmit buffer is empty
    while (!(USART2->SR & USART_SR_TXE));
    
    // Send character
    USART2->DR = c;
}

//Create a function to receive a character and echo that back to the host to make sure Rx is working.
uint8_t getbyte(void)
{
	// Wait until receive buffer is not empty
    while (!(USART2->SR & USART_SR_RXNE));
    
    // Return received character
    return USART2->DR;
}