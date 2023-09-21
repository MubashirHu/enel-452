#include "util.h"
#include "functions.h"
#include "GPIO.h"

int main(void)
{
	//init
	clockInit();
	led_IO_init();
	switch_IO_Init();
	led_GPIO_Init();

	//infinite loop
	while(1)
	{
		
	turnLEDON();
		
	}
	
}
