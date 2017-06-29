#include "main.h"
#include "stuff.h"

uint8_t data[] = "QSPI DIRECT READ/WRITE TEST PROJECT!!!";
uint8_t received[sizeof(data)];

int main()
{
	SCB_EnableICache();
	SCB_EnableDCache();
	
	HAL_Init();
	RCC_Init();
	
	//Init green LED
	IOCLK(I);
	OUT(I, P1);
	
	IOCLK(K);
	OUT(K, P3);
	
		
	QSPI_Init();
	QSPI_EraseBlock(QSPI_ADDRESS);
	QSPI_Write(data, QSPI_ADDRESS, sizeof(data));
	QSPI_Read(received, QSPI_ADDRESS, sizeof(data));
	
	while(1)
	{
		IOSET(PI1);
		HAL_Delay(10);
		QSPI_Read(received, QSPI_ADDRESS, sizeof(data));
		IORESET(PI1);
		HAL_Delay(10);
	}
}
