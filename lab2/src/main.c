#include "../headers/USART.h"
#include "../headers/CLI.h"
#include "../351/util.h"

int main(void)
{
	//init
	clockInit();
	serial_open();
	
	while(1)
	{
		
		
    char receivedChar = getbyte();
		sendbyte(receivedChar);

    // Print the received character for debugging

    // Send back the received character
    sendbyte(receivedChar);
		sendbyte('a');
		
	}
	
}
