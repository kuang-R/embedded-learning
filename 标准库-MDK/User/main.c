#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "usart1.h"
#include <stdio.h>

int main(void)
{
	// 来到这里的时候，系统的时钟已经被配置成72M。
	USART1_Config(115200);
	while (1) {
		
	}
}


