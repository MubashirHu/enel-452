#include "../headers/USART.h"
#include "../headers/CLI.h"
#include "../351/util.h"

int main(void)
{
	//init
	clockInit();
	led_IO_init();

	//infinite loop
	while(1)
	{
		
	led_flash();
		
	}
	
}
