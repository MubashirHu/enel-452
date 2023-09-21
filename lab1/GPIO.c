#include "GPIO.h"
#include "stm32f10x.h"
void switch_IO_Init(void)
{
	
	//Enable Port B clocks
    
    RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN ;
		
	//****************NUCLEO-64*******************************************************************
//    //Set the config and mode bits for Port B bit 4 so it will be in input mode and configuration of Input with pull-up / pull-down 
    GPIOB->CRL |= GPIO_CRL_CNF4_1 ;
	//****************NUCLEO-64*******************************************************************
	
	//****************NUCLEO-64*******************************************************************
//    //Set the config and mode bits for Port B bit 5 so it will be in input mode and configuration of Input with pull-up / pull-down 
    GPIOB->CRL |= GPIO_CRL_CNF5_1;
	//****************NUCLEO-64*******************************************************************
	
	//****************NUCLEO-64*******************************************************************
//    //Set the config and mode bits for Port B bit 6 so it will be in input mode and configuration of Input with pull-up / pull-down 
    GPIOB->CRL |= GPIO_CRL_CNF6_1;
	//****************NUCLEO-64*******************************************************************
	
	GPIOB->CRL &= 0x4888ffff;
	
	//****************NUCLEO-64*******************************************************************
//    //Set the config and mode bits for Port B bit 8 so it will be in input mode and configuration of Input with pull-up / pull-down 
    GPIOB->CRH |= GPIO_CRH_CNF8_1;
	GPIOB->CRH &= 0xfffffff8;
	//****************NUCLEO-64*******************************************************************
	
}

void led_GPIO_Init(void)
{
	//Enable Port A and AFIO clocks
			
    RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;

// Configure PA7 as GPIO ( Write 0011b into the configuration and mode bits )
    GPIOA->CRL |= GPIO_CRL_CNF7 | GPIO_CRL_MODE7;
	
		GPIOA->CRL &= 0x3fffffff;
	
// Configure PA8 as GPIO ( Write 0011b into the configuration and mode bits )
    GPIOA->CRH |= GPIO_CRH_CNF8 | GPIO_CRH_MODE8;
	
// Configure PA9 as GPIO ( Write 0011b into the configuration and mode bits )
    GPIOA->CRH |= GPIO_CRH_CNF9 | GPIO_CRH_MODE9;
	
// Configure PA10 as GPIO ( Write 0011b into the configuration and mode bits )
    GPIOA->CRH |= GPIO_CRH_CNF10 | GPIO_CRH_MODE10;
	
		GPIOA->CRH &= 0xfffff333;
	
}
