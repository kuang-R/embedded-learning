
#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H
#include "stm32f10x.h"

#define sFLASH_ID 0xEF4017
#define SPI_FLASH_CS_LOW() GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_FLASH_CS_HIGH() GPIO_SetBits(GPIOA, GPIO_Pin_4)

static void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

uint8_t SPI_FLASH_SendByte(uint8_t byte);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_Init(void);
void SPI_Flash_PowerDown(void);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
#endif
