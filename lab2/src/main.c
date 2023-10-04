/**
 * @file main.c
 *
 * @brief Main Program for STM32F103RB Microcontroller.
 *
 * This file comprises the primary program logic that is intended to be flashed onto the STM32F103RB board.
 * It serves as the central component, orchestrating the microcontroller's operations and functions. 
 *
 * @author Mubashir Hussain
 * @studentID 200396797
 */

#include "../headers/USART.h"
#include "../headers/CLI.h"
#include "../351/util.h"

int main(void)
{
	//init
	clockInit();
	serial_open();
	led_IO_init();
	uint8_t receivedData[512];
	//receivedData[0] = '\0';
	
	while(1)
	{
		CLI_Receive(receivedData, sizeof(receivedData));
	}
	
	
}
