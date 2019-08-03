#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "usart1.h"
#include <stdio.h>
/* 开发板：	霸道v1
 * 功能：	PWM输出LED_G
 */
void LEDG_Config(void)
{
	GPIO_InitTypeDef GPIO_LED1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_LED1.GPIO_Pin = GPIO_Pin_0;
	GPIO_LED1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_LED1.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_LED1);
}
void TIM3_PWMInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TimOCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
	TIM_TimeBaseStructure.TIM_Period = 50-1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TimOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TimOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TimOCInitStructure.TIM_Pulse = 47;
	TimOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM3, &TimOCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
}
int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。

	USART1_Config(115200);
	LEDG_Config();
	TIM3_PWMInit();
	while (1) {
		
	}
}

