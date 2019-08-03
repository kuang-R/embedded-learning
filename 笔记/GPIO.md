## 输出速度
当GPIO输出时，应该设置速度。
这里的速度指驱动电路的响应速度，输出速度取决于软件程序。
### 输出速度的设置
推荐I/O引脚的输出速度为输出信号速度的5～10倍。
## GPIO重映射
需要打开AFIO时钟。
~~~ c
/* 未验证 */
/* 将PD5，PD6重映射为USART2的输出输入引脚 */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_Init(GPIOD, &GPIO_InitStructure);
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOD, &GPIO_InitStructure);

RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
~~~
