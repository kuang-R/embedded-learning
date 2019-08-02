#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "usart1.h"
#include <stdio.h>
/* 开发板：	霸道v1
 * 功能：	K1按下时LED1亮，松开时LED1灭
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
void K1_Config(void)
{
	GPIO_InitTypeDef GPIO_K1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_K1.GPIO_Pin = GPIO_Pin_0;
	GPIO_K1.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_K1);
}
int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。
	USART1_Config(115200);
	LED1_Config();
	K1_Config();
	while (1) {
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
			GPIO_ResetBits(GPIOF, GPIO_Pin_7);
		else
			GPIO_SetBits(GPIOF, GPIO_Pin_7);
	}
}

