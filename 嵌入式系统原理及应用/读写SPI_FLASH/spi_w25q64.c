#include "stm32f10x.h"
#include "spi_w25q64.h"

#define SPI_FLASH_PageSize					256
#define SPI_FLASH_PerWritePageSize	256
#define W25X_WriteEnable						0x06
#define W25X_WriteDisable						0x04
#define W25X_ReadStatusReg					0x05
#define W25X_ReadData								0x03
#define W25X_FastReadData						0x0B
#define W25X_FastReadDual						0x3B
#define W25X_PageProgram						0x02
#define W25X_BlockErase							0xD8
#define W25X_SectorErase						0x20
#define W25X_ChipErase							0xC7
#define W25X_PowerDown							0xB9
#define W25X_DeviceID								0xAB
#define W25X_ManufactDeviceID				0x90
#define W25X_JedecDeviceID					0x9F
#define WIP_Flag										0x01
#define Dummy_Byte									0xFF

void SPI_FLASH_Init(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* SCK, MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* MISO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* CS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_FLASH_CS_HIGH();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_SectorErase);
	SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
	SPI_FLASH_SendByte((SectorAddr & 0x00FF00) >> 8);
	SPI_FLASH_SendByte(SectorAddr & 0x0000FF);
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}
static void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();

	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_PageProgram);
	SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
	SPI_FLASH_SendByte((WriteAddr & 0x00FF00) >> 8);
	SPI_FLASH_SendByte(WriteAddr & 0x0000FF);
	if (NumByteToWrite > SPI_FLASH_PerWritePageSize) {
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
	}
	while (NumByteToWrite--) {
		SPI_FLASH_SendByte(*pBuffer);
		pBuffer++;
	}
	SPI_FLASH_CS_HIGH();

	SPI_FLASH_WaitForWriteEnd();
}
void SPI_FLASH_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	Addr = WriteAddr % SPI_FLASH_PageSize;
	count = SPI_FLASH_PageSize - Addr;
	NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	if (Addr == 0) {
		/* WriteAddr is SPI_FLASH_PageSize aligned */
		if (NumOfPage == 0) {
			/* NumByteToWrite < SPI_FLASH_PageSize */
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else {
			/* NumByteToWrite > SPI_FLASH_PageSize */
			while (NumOfPage--) {
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else {
		/* WriteAddr is not SPI_FLASH_PageSize aligned */
		if (NumOfPage == 0) {
			/* NumByteToWrite < SPI_FLASH_PageSize */
			if (NumOfSingle > count) {
				temp = NumOfSingle - count;
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				WriteAddr += count;
				pBuffer += count;
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
			}
			else {
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else {
			/* NumByteToWrite > SPI_FLASH_PageSize */
			NumByteToWrite -= count;
			NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			WriteAddr += count;
			pBuffer += count;
			while (NumOfPage--) {
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr += SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
			if (NumOfSingle != 0) {
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
}
void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_ReadData);
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
	SPI_FLASH_SendByte((ReadAddr & 0x00FF00) >> 8);
	SPI_FLASH_SendByte(ReadAddr & 0x0000FF);
	while (NumByteToRead--) {
		*pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
		pBuffer++;
	}
	SPI_FLASH_CS_HIGH();
}
uint32_t SPI_FLASH_ReadID(void)
{
	uint32_t Temp=0, Temp0=0, Temp1=0, Temp2=0;
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_JedecDeviceID);
	Temp0 = SPI_FLASH_SendByte(Dummy_Byte);
	Temp1 = SPI_FLASH_SendByte(Dummy_Byte);
	Temp2 = SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_CS_HIGH();
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}
uint32_t SPI_FLASH_ReadDeviceID(void)
{
	uint32_t Temp = 0;
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_DeviceID);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);
	Temp = SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_CS_HIGH();
	return Temp;
}
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		;
	SPI_I2S_SendData(SPI1, byte);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		;
	return SPI_I2S_ReceiveData(SPI1);
}
void SPI_FLASH_WriteEnable(void)
{
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_WriteEnable);
	SPI_FLASH_CS_HIGH();
}
void SPI_FLASH_WaitForWriteEnd(void)
{
	uint8_t FLASH_Status = 0;
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_ReadStatusReg);
	do {
		FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
	} while ((FLASH_Status & WIP_Flag) == SET);
	SPI_FLASH_CS_HIGH();
}
void SPI_Flash_PowerDown(void)
{
	SPI_FLASH_CS_LOW();
	SPI_FLASH_SendByte(W25X_PowerDown);
	SPI_FLASH_CS_HIGH();
}
