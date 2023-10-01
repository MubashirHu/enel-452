#include "../351/util.h"
#include "../351/functions.h"
#include "../351/GPIO.h"

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
		
	led_flash();
		
	}
	
}
