#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "usart1.h"
#include <stdio.h>
/* 开发板：	霸道v1
 * 功能：	每500MS翻转一次LEDG的状态
 */
 
typedef enum { FAILED=0, PASSED=!FAILED} TestStatus;
#define BufferSize 12
uint32_t CurrDataCounterBegin = 0;
volatile uint32_t CurrDataCounterEnd = 1;
TestStatus TransferStatus = FAILED;
const uint32_t SRC_Const_Buffer[BufferSize] = {
	0x54826358, 0x54826358, 0x54826358, 0x54826358,
	0x54826358, 0x54826358, 0x54826358, 0x54826358,
	0x54826358, 0x54826358, 0x54826358, 0x54826358,
};
uint32_t DST_Buffer[BufferSize];

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer;
	DMA_InitStructure.DMA_BufferSize = BufferSize;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
	CurrDataCounterBegin = DMA_GetCurrDataCounter(DMA1_Channel6);
	DMA_Cmd(DMA1_Channel6, ENABLE);
}
TestStatus Buffercmp(const uint32_t *pBuffer, uint32_t *pBuffer1, uint16_t BufferLength)
{
	while (BufferLength--) {
		if (*pBuffer != *pBuffer1)
			return FAILED;
		pBuffer++;
		pBuffer1++;
	}
	return PASSED;
}
int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。

	USART1_Config(115200);
	NVIC_Config();
	DMA_Config();
	while (CurrDataCounterEnd != 0)
		;
	TransferStatus = Buffercmp(SRC_Const_Buffer, DST_Buffer, BufferSize);
	while (1) {
		
	}
}

