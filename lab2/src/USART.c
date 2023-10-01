#include "stm32f10x.h"
#include "../headers/USART.h"
#include "../headers/MY_STM32_FUNCTIONS.h"

void initializeUSART(void)
{
	//Enable port A
	enablePort('A');
	
	//Enable USART2 CLOCK
	enableUSART(2);
	
	//Configure PA2 for alternate function output push pull mode, max 50MHz
	
	//Configure PA3 for Input with pull-up / pull-down
	
	//Enable the USART Tx and Rx in the USART Control register.
	
	//Configure USART 2 for 115200 bps, 8-bits-no parity, 1 stop bit. (Peripheral clock is 36MHz)
	
}

//Create a loop to send a character to the host and make sure that character is printed. This will test the Tx.
void USART2_SendChar(char c)
{
	c = 'a';
	return;
}

//Create a function to receive a character and echo that back to the host to make sure Rx is working.
char USART2_ReceiveChar(void)
{
	
	return 'a';
}

void serial_open(void)
{
}





