#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "usart1.h"
#include <stdio.h>
/* 开发板：	霸道v1
 * 功能：	PC串口通信（回显）
 */

uint8_t USART1_RxIsNotEmpty(void)
{
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
		return 1;
	else
		return 0;
}
uint8_t USART1_RecvChar(void)
{
	return USART_ReceiveData(USART1);
}
int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。

	USART1_Config(115200);
	while (1) {
		if (USART1_RxIsNotEmpty())
			fputc(USART1_RecvChar(), stdout);
	}
}
