#include "../headers/USART.h"
#include "../headers/CLI.h"
#include "../351/util.h"

int main(void)
{
	//init
	clockInit();
	serial_open();
	
	uint8_t testData[] = "hi";
  uint16_t dataSize = sizeof(testData) - 1; 
	
	CLI_Transmit(testData, dataSize);
	
	// wait on user to send data 
	CLI_Receive(testData, sizeof(testData));
	
	// once recieved, echo back each character 

		//sendbyte(33);
		//delay(1800000*1);
		//sendbyte(34);
		//delay(1800000*1);
	
	
}
