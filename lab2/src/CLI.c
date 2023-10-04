#include "../headers/CLI.h"
#include "../headers/USART.h"
 #include "stm32f10x.h"
 
void CLI_Transmit(uint8_t *pData, uint16_t Size)
{
	for(int i = 0	; i < Size; i++)
	{
		sendbyte(pData[i]);
	}
		
}
void CLI_Receive(uint8_t *pData, uint16_t Size)
{
	for ( int i = 0; i < Size; i++)
	{
		pData[i] = getbyte();
		
		// if Enter key is pressed, manually send a carriage and new line byte
		if(pData[i] == 10 || pData[i] == 13)
		{
			sendbyte(13);
			sendbyte(10);
			
		}
		
		sendbyte(pData[i]);
		
	}
	
}