/**
 * @file USART.c
 *
 * @brief Function Definitions for USART (Universal Synchronous Asynchronous Receiver-Transmitter) Module.
 *
 * This file includes the implementation of functions corresponding to the prototypes defined
 * in the `../headers/USART.h` header file. The USART module is responsible for configuring and
 * managing communication with external devices using the USART protocol.
 *
 * @note Functions in this file align with the USART module in `../headers/USART.h`.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#include "stm32f10x.h"
#include "../headers/USART.h"
#include "../headers/MY_STM32_FUNCTIONS.h"

void serial_open(void)
{
	//Enable port A
	enablePort('A');
	
	//Enable USART2 CLOCK
	enableUSART(2);
	
	//Configure PA2 for alternate function output push pull mode, max 50MHz b1011
	GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2;
	
	//Configure PA3 for Input with pull-up / pull-down b1000
	GPIOA->CRL |= GPIO_CRL_CNF3_0;
	
	//Enable the USART Tx and Rx in the USART Control register.
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE; //Enable Tx and Rx
	
	//Enable USART2 RXNE interrupt
	USART2->CR1 |= USART_CR1_RXNEIE;
	
	// Enable USART
	USART2->CR1 |= USART_CR1_UE;
	
	//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz)
	USART2->BRR = 0x0139;
	//USART2->BRR = 0x09c4;
}

void serial_close(void)
{
	disablePort('A');
	disableUSART(2);
	
	//Configure PA2 for alternate function output push pull mode, max 50MHz b1011
	GPIOA->CRL &= ~(GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2);
	
	//Configure PA3 for Input with pull-up / pull-down b1000
	GPIOA->CRL &= ~(GPIO_CRL_CNF3_1);
	
	//Disable the USART Tx and Rx in the USART Control register.
	USART2->CR1 &= ~(USART_CR1_TE | USART_CR1_RE); //Disable Tx and Rx
	USART2->CR1 &= ~(USART_CR1_UE); // Disable USART
	USART2->CR1 &= ~(USART_CR1_RXNEIE); // Disable RXNE interrupt-enable
	
	//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz)
	USART2->BRR = 0x0000;
	
}

int sendbyte(uint8_t b)
{
	uint16_t Timeout = SENDDATA_TIMER;
	TIM2->ARR = 10*Timeout;
	TIM2->CNT = 0;
	
	// Wait until transmit buffer is empty
    while (!(USART2->SR & USART_SR_TXE))
		{
			if((TIM2->SR & TIM_SR_UIF) != 0)
			{
				return 1;
			}
		}
    
    // Send character
    USART2->DR = b;
		
		while((USART2->SR&(1<<6)) == 0) //wait until the TC flag is set
		{
			if((TIM2->SR & TIM_SR_UIF) != 0)
			{
				return 1;
			}
		}
		
		return 0;
}

char getbyte(void)
{
	uint16_t Timeout = RECEIVEDATA_TIMER;
	TIM2->ARR = 10*Timeout;
	TIM2->CNT = 0;
	
	// Wait until receive buffer is not empty
    while (!(USART2->SR & USART_SR_RXNE))
		{
			// Return received character
			//sendbyte('a');
			if((TIM2->SR & TIM_SR_UIF) != 0)
			{
				return 1;
			}
		}
	
		return USART2->DR;
		
}

void USART2_IRQHandler(void) {
    if ((USART2->SR & USART_SR_RXNE) != 0) 
		{
      uint8_t receivedData = USART2->DR; // Read the received data
			dataReceivedFlag = 1; // Set the flag to indicate character reception
    }
}

void init_TIM2(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 0xE0F; // Divide 36 MHz by 3600 (PSC+1), PSC_CLK= 10000 Hz, 1 count = 0.1 ms
	TIM2->ARR = 10; // 10 unts = 1 ms or 1000 Hz, period 
	TIM2->CNT = 0;
	TIM2->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN; // Auto-reload preload enable and Counter enable
}
