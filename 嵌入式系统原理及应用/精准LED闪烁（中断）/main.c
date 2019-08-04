#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
#include "usart1.h"
#include <stdio.h>
/* �����壺	�Ե�v1
 * ���ܣ�	ÿ500MS��תһ��LEDG��״̬
 */
void LEDG_Config(void)
{
	GPIO_InitTypeDef GPIO_LED1;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_LED1.GPIO_Pin = GPIO_Pin_0;
	GPIO_LED1.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LED1.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_LED1);
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}	
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void TIM2_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 36000-1;
	TIM_TimeBaseStructure.TIM_Period = 1000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}
int main(void)
{
	// ���������ʱ��ϵͳ��ʱ���Ѿ������ó�72M��

	USART1_Config(115200);
	LEDG_Config();
	NVIC_Config();
	TIM2_Config();
	while (1) {
		
	}
}

