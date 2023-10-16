#include "PWM.h"
#include "stm32f10x.h"

void tim3GpioSetup(void)
{
	//turn on AFIO, and clock for timer 3
	RCC->APB2ENR |=  RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	// turn on PORT A6 output
	// Configure PA6 as GPIO ( Write 1011b into the configuration and mode bits )
		GPIOA->CRL &= 0x0B0f000f;
    GPIOA->CRL |= GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6;
	
TIM3->CR1 |= TIM_CR1_CEN; // Enable Timer3
TIM3->EGR |= TIM_EGR_UG; // Reinitialize the counter
TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // PWM mode 1
TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC1FE; // Preload Enable, Fast Enable
TIM3->CCER |= TIM_CCER_CC1E; // Enable CH1
TIM3->PSC = 0x095F; // Divide 24 MHz by 2400 (PSC+1), PSC_CLK= 10000 Hz, 1 count = 0.1 ms
TIM3->ARR = 10000; // 100 counts = 10 ms or 100 Hz
TIM3->CCR1 = 50; // 50 counts = 5 ms = 50% duty cycle
TIM3->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN; // Enable Timer3
}

void updateDutyCycle(uint16_t DutyCycle)
{

TIM3->CCR1 = DutyCycle; // 50 counts = 5 ms = 50% duty cycle

}