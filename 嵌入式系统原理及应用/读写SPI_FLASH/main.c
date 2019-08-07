#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "usart1.h"
#include "spi_w25q64.h"
#include <stdio.h>
/* 开发板：	霸道v1
 * 功能：	SPI通信 FLASH
 */
typedef enum { FAILED=0, PASSED=!FAILED} TestStatus;
#define BufferSize (sizeof(Tx_Buffer)-1)
#define FLASH_WriteAddress 0x12345
#define FLASH_ReadAddress FLASH_WriteAddress
#define FLASH_SectorToErase FLASH_WriteAddress

uint8_t Tx_Buffer[] = "This is an example of using SPI to erase, program and read the FLASH!";
uint8_t Rx_Buffer[BufferSize];
volatile uint32_t DeviceID = 0;
volatile uint32_t FlashID = 0;
volatile TestStatus TransferStatus1 = PASSED;

void Delay(volatile uint32_t nCount)
{
	for(; nCount != 0; nCount--)
		;
}
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
{
	while (BufferLength--) {
		if (*pBuffer1 != *pBuffer2) {
			return FAILED;
		}
		pBuffer1++;
		pBuffer2++;
	}
	return PASSED;
}
int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。

	USART1_Config(115200);
	printf("This is an Example of using SPI to oerate 8MB FLASH(w25q64)\r\n");
	SPI_FLASH_Init();
	DeviceID = SPI_FLASH_ReadDeviceID();
	Delay(200);
	FlashID = SPI_FLASH_ReadID();
	printf("FlashID is 0x%x, Manufacturer Device ID is 0x%x\r\n", FlashID, DeviceID);
	
	if (FlashID == sFLASH_ID) {
		printf("Detect the FLASH(w25q64)!\r\n");
		SPI_FLASH_SectorErase(FLASH_SectorToErase);
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("The data written to the FLASH(w25q64):\r\n%s\r\n", Tx_Buffer);
		SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
		printf("The data read from the FLASH(w25q64):\r\n%s\r\n", Rx_Buffer);
		TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
		if (PASSED == TransferStatus1)
			printf("Program the FLASH(w25q64) successfully!\r\n");
		else
			printf("Failed to program the FLASH(w25q64)!\r\n");
	}
	else
		printf("Failed to detect the FLASH(w25q64)!\r\n");
	SPI_Flash_PowerDown();
		
	while (1) {
		
	}
}
