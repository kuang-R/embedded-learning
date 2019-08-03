#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "usart1.h"
#include <stdio.h>
/* 开发板：	霸道v1
 * 功能：	精准定时LED闪烁
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
void TIM2_Delay500MS(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_Cmd(TIM2, ENABLE);
	while (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == RESET)
		;
}
int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。
	uint32_t CountOfToggle = 0;

	USART1_Config(115200);
	LED1_Config();
	while (1) {
		GPIO_ResetBits(GPIOF, GPIO_Pin_7);
		TIM2_Delay500MS();
		GPIO_SetBits(GPIOF, GPIO_Pin_7);
		TIM2_Delay500MS();

		CountOfToggle++;
		printf("The Count of Toggle is %d\r\n", CountOfToggle);
	}
}

