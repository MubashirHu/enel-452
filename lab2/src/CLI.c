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
		
		CLI_Transmit(&pData[i], 1);
		
	}
	
}