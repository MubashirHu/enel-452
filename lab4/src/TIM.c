/**
 * @file USART.c
 *
 * @brief Function Definitions for USART (Universal Synchronous Asynchronous Receiver-Transmitter) Module.
 *
 * This file includes the implementation of functions corresponding to the prototypes defined
 * in the `../headers/USART.h` header file. The USART module is responsible for configuring and
 * managing communication with external devices using the USART protocol.
 *
 * @note Functions in this file align with the USART module in `../headers/TIM.h`.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#include "stm32f10x.h"
#include "../headers/TIM.h"
#include "../headers/MY_STM32_FUNCTIONS.h"


void initTIM2(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 0xE0F; // Divide 36 MHz by 3600 (PSC+1), PSC_CLK= 10000 Hz, 1 count = 0.1 ms
	TIM2->ARR = 10; // 10 unts = 1 ms or 1000 Hz, period 
	TIM2->CNT = 0;
	TIM2->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN; // Auto-reload preload enable and Counter enable
}


