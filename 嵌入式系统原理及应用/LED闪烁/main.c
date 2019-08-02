#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "usart1.h"
#include <stdio.h>
/* 开发板：	霸道v1
 * 功能：	LED1闪烁
 */
void LED1_Config(void)
{
	GPIO_InitTypeDef GPIO_LED1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	GPIO_LED1.GPIO_Pin = GPIO_Pin_7;
	GPIO_LED1.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED1.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOF, &GPIO_LED1);
}
void Delay(uint32_t x)
{
	uint32_t i;
	for (i = 0; i < x; i++)
		;
}
int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。
	USART1_Config(115200);
	LED1_Config();
	while (1) {
		GPIO_ResetBits(GPIOF, GPIO_Pin_7);
		Delay(0x5FFFFF);
		GPIO_SetBits(GPIOF, GPIO_Pin_7);
		Delay(0x5FFFFF);
	}
}

