#ifndef __QSPI_H
#define __QSPI_H

#include "stm32f7xx.h"

#define QSPI_ADDRESS	0x00000000

void QSPI_Init();
void QSPI_Read(uint8_t* data, uint32_t address, uint32_t size);
void QSPI_Write(uint8_t* data, uint32_t address, uint32_t size);
void QSPI_EraseBlock(uint32_t block_addr);

#endif
