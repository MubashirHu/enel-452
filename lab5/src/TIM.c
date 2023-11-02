/**
 * @file TIM.c
 *
 * @brief Function Definitions for USART (Universal Synchronous Asynchronous Receiver-Transmitter) Module.
 *
 * This file includes the implementation of functions corresponding to the prototypes defined
 * in the `../headers/TIM.h` header file. This module is responsible for the timings in the sendByte
 * and getByte functions as well as the interrupt timers to update the terminal.
 *
 * @note Functions in this file align with the USART module in `../headers/TIM.h`.
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#include "stm32f10x.h"
#include "../headers/TIM.h"
#include "../headers/MY_STM32_FUNCTIONS.h"

void initTIM(int timer)
{
	switch(timer)
	{
		case 2:
			RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
			TIM2->PSC = 0xE0F; // Divide 36 MHz by 3600 (PSC+1), PSC_CLK= 10000 Hz, 1 count = 0.1 ms
			TIM2->ARR = 10; // 10 unts = 1 ms or 1000 Hz, period 
			TIM2->CNT = 0;
			TIM2->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN; // Auto-reload preload enable and Counter enable
			break;
		
		case 3:
			RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
			TIM3->PSC = 0xE0F; // Divide 36 MHz by 3600 (PSC+1), PSC_CLK= 10000 Hz, 1 count = 0.1 ms
			TIM3->ARR = 10; // 10 unts = 1 ms or 1000 Hz, period 
			TIM3->CNT = 0;
			TIM3->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN; // Auto-reload preload enable and Counter enable
			break;
		
		case 4:
			RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
			TIM4->PSC = 0xE0F; // Divide 36 MHz by 3600 (PSC+1), PSC_CLK= 10000 Hz, 1 count = 0.1 ms
			TIM4->ARR = 10; // 10 unts = 1 ms or 1000 Hz, period 
			TIM4->CNT = 0;
			TIM4->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN; // Auto-reload preload enable and Counter enable
			break;
		
		default:
			break;
	}
}

void configTIM(int timer, uint16_t time_period)
{
	switch(timer)
	{
		case 2:
			TIM2->ARR = 10*time_period;
			TIM2->CNT = 0;
			break;
		
		case 3:
			TIM3->ARR = 10*time_period;
			TIM3->CNT = 0;
			break;
		
		case 4:
			TIM4->ARR = 10*time_period;
			TIM4->CNT = 0;
			break;
		
		default:
			break;
	}
}

void initTIMInterrupt(int timer)
{
	switch(timer)
	{
		case 2:
			TIM2->CR1 |= TIM_DIER_UIE;
			NVIC_EnableIRQ(TIM2_IRQn);
			break;
		
		case 3:
			TIM3->DIER |= TIM_DIER_UIE;
			NVIC_EnableIRQ(TIM3_IRQn);
			break;
		
		case 4:
			TIM4->CR1 |= TIM_DIER_UIE;
			NVIC_EnableIRQ(TIM4_IRQn);
			break;
		
		default:
			break;
	}
}

void TIM3_IRQHandler(void) {
	TIM3_UPDATE_EVENT = 1;
	TIM3->SR &= ~(TIM_SR_UIF); // reset update event flag
}
