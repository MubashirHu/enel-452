#include "../headers/USART.h"
#include "../headers/CLI.h"
#include "../351/util.h"

int main(void)
{
	//init
	clockInit();
	serial_open();
	uint8_t receivedData[512];
	receivedData[0] = '\0';
	
	while(1)
	{
		CLI_Receive(receivedData, sizeof(receivedData));
	}
	
	
}
