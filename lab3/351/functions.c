#include"functions.h"
#include "stm32f10x.h"
#include "util.h"
#include "functions.h"

//**************************** Utility ************************************************************
// delay = 1800 is approximately 1 ms @ SYSCLK = 24 MHz (ymmv)

uint16_t read_DIP(void)
{
	uint16_t sw_val;
	// Note how the >> operator helps us create a four bit value in the 4 least significant bit positions of
	// our return value. This four bit value indicates the state of all 4 switches
	//sw_val = (((GPIOB->IDR &( GPIO_IDR_IDR4 | GPIO_IDR_IDR5))>>6) | ((GPIOB->IDR &( GPIO_IDR_IDR10 | GPIO_IDR_IDR11))>>8)) & 0x000F;
	// sw_val = (GPIOB->IDR & 0x0FF0);
	//return sw_val;
	
	sw_val = ( ((GPIOB->IDR &( GPIO_IDR_IDR4 | GPIO_IDR_IDR5 | GPIO_IDR_IDR6))>>4) | ( GPIOB->IDR & GPIO_IDR_IDR8) >> 5 ) & 0x00FF;
	
	return sw_val;

}

void drive_LED(int LED)
{
	if(LED == 1)
	{
			GPIOA->ODR = GPIO_ODR_ODR7;
	}
	else if(LED == 2)
	{
			GPIOA->ODR = GPIO_ODR_ODR8;

	}
	else if(LED == 3)
	{
			GPIOA->ODR = GPIO_ODR_ODR9;

	}
	else if(LED == 4)
	{
			GPIOA->ODR = GPIO_ODR_ODR10;
	}
}

void drive_LED_Binary(int LED)
{
	if(LED == 1)
	{
	GPIOA->ODR = GPIO_ODR_ODR7;
	}
	
	if(LED == 2)
	{
		GPIOA->ODR = GPIO_ODR_ODR8;
	}
	
	if(LED == 3)
	{
		GPIOA->ODR = GPIO_ODR_ODR7 | GPIO_ODR_ODR8 ;
	}
	if(LED == 4)
	{
		GPIOA->ODR = GPIO_ODR_ODR9;
	}
	if(LED == 5)
	{
		GPIOA->ODR = GPIO_ODR_ODR9 | GPIO_ODR_ODR7;
	}
	if(LED == 6)
	{
		GPIOA->ODR = GPIO_ODR_ODR9 | GPIO_ODR_ODR8;
	}
	if(LED == 7)
	{
		GPIOA->ODR = GPIO_ODR_ODR9 | GPIO_ODR_ODR8 |GPIO_ODR_ODR7;
	}
	if(LED == 8)
	{
		GPIOA->ODR = GPIO_ODR_ODR10;
	}
	if(LED == 9)
	{
		GPIOA->ODR = GPIO_ODR_ODR10 | GPIO_ODR_ODR7;
	}
	if(LED == 10)
	{
		GPIOA->ODR = GPIO_ODR_ODR10 | GPIO_ODR_ODR8 ;
	}
	if(LED == 11)
	{
		GPIOA->ODR = GPIO_ODR_ODR10 | GPIO_ODR_ODR8 | GPIO_ODR_ODR7;
	}
	if(LED == 12)
	{
		GPIOA->ODR = GPIO_ODR_ODR10 | GPIO_ODR_ODR9;
	}
	if(LED == 13)
	{
		GPIOA->ODR = GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR7;
	}
	if(LED == 14)
	{
		GPIOA->ODR = GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8;
	}
	if(LED == 15)
	{
		GPIOA->ODR = GPIO_ODR_ODR10 | GPIO_ODR_ODR9 | GPIO_ODR_ODR8 | GPIO_ODR_ODR7;
	}
	
}

void routine1(void)
{
drive_LED(1);
	delay(900000);
drive_LED(2);
	delay(900000);
drive_LED(3);
	delay(900000);
drive_LED(4);
	delay(900000);
}

void routine2(void)
{
	drive_LED(1);
	delay(400000);
	drive_LED(2);
	delay(400000);
	drive_LED(3);
	delay(400000);
	drive_LED(4);
	delay(400000);
}

void routine3(void)
{
	for ( int i = 1; i < 16; i++)
	{
		drive_LED_Binary(i);
		delay(1800000);
	}
}

void routine4(void)
{
	for ( int i = 1; i < 16; i++)
	{
		drive_LED_Binary(i);
		delay(900000);
	}
}
	
