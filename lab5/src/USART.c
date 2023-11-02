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
#include "../headers/TIM.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern QueueHandle_t xCLI_Queue;

void serialOpen(void)
{
	enablePort('A');
	
	enableUSART(2);
	
	//Configure PA2 for alternate function output push pull mode, max 50MHz b1011
	GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2;

	//Configure PA3 for Input with pull-up / pull-down b1000
	GPIOA->CRL |= GPIO_CRL_CNF3_0;
	
	//Enable the USART Tx and Rx in the USART Control register.
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE; 
		
	// Enable USART
	USART2->CR1 |= USART_CR1_UE;
	
	//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz)
	USART2->BRR = 0x0139;
}

void serialClose(void)
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

int sendByte(uint8_t b)
{
	configTIM(2, SENDDATA_TIMER);
	
	// Wait until transmit buffer is empty
    while (!(USART2->SR & USART_SR_TXE))
		{
			if((TIM2->SR & TIM_SR_UIF) != 0)
			{
				//reset update event
				TIM2->SR &= ~(TIM_SR_UIF);
				return 1;
			}
		}
    
    USART2->DR = b; // Send character
		
		while((USART2->SR&(1<<6)) == 0) //wait until the TC flag is set
		{
			if((TIM2->SR & TIM_SR_UIF) != 0)
			{
				//reset update event
				TIM2->SR &= ~(TIM_SR_UIF);
				return 1;
			}
		}
		
		return 0;
}

char getByte(void)
{
	uint16_t Timeout = RECEIVEDATA_TIMER;
	TIM2->ARR = 10*Timeout;
	TIM2->CNT = 0;
	
	// Wait until receive buffer is not empty
    while (!(USART2->SR & USART_SR_RXNE))
		{
			if((TIM2->SR & TIM_SR_UIF) != 0)
			{
				//reset update event
				TIM2->SR &= ~(TIM_SR_UIF);
				return USART2->DR;
			}
		}
}

void initUSART2Interrupt(void)
{
	USART2->CR1 |= USART_CR1_RXNEIE; // enable RX interrupt in the control register
	NVIC_EnableIRQ(USART2_IRQn); // enable usart2 interrupts in the NVIC table
}

void USART2_IRQHandler(void) {
	DATA_RECEIVED_FLAG = 1;
	USART2->SR &= ~(USART_SR_RXNE);	
	uint8_t characterReceived = USART2->DR;
	xQueueSendToFrontFromISR( xCLI_Queue, &characterReceived, NULL);
	DATA_RECEIVED_FLAG = 0;
}
