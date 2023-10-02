#include "../headers/USART.h"
#include "../headers/CLI.h"
#include "../351/util.h"

int main(void)
{
	//init
	clockInit();
	initializeUSART();

	//infinite loop
	while(1)
	{
		sendbyte(33);
		delay(1800000*1);
		sendbyte(34);
		delay(1800000*1);
		
	}
	
}
