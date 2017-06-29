#include "qspi.h"
#include "stm32746g_discovery_qspi.h"

void QSPI_Init()
{
	BSP_QSPI_Init();
}

void QSPI_Read(uint8_t* data, uint32_t address, uint32_t size)
{
	BSP_QSPI_Read(data, address, size);
}

void QSPI_Write(uint8_t* data, uint32_t address, uint32_t size)
{
	BSP_QSPI_Write(data, address, size);
}

void QSPI_EraseBlock(uint32_t block_addr)
{
	BSP_QSPI_Erase_Block(block_addr);
}
