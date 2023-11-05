
#include <stdio.h>
#include "stm32f4xx.h"
#include "led.h"
#include "beep.h"


/*
	uart1_init:串口1的初始化
*/
void uart1_init(void)
{

	//1. 配置USART1的Tx/Rx引脚
	// USART1  PA9/PA10
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef p = {0};

	p.GPIO_Mode = GPIO_Mode_AF;
	p.GPIO_OType = GPIO_OType_PP;
	p.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	p.GPIO_PuPd = GPIO_PuPd_NOPULL;
	p.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &p);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10,GPIO_AF_USART1);


	//2. 配置USART
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitTypeDef u = {0};
	u.USART_BaudRate = 115200;
	u.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	u.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	u.USART_Parity = USART_Parity_No;
	u.USART_StopBits = USART_StopBits_1;
	u.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &u);


	//3. 配置USART中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	NVIC_InitTypeDef n;
	n.NVIC_IRQChannel = USART1_IRQn;
	n.NVIC_IRQChannelCmd = ENABLE;
	n.NVIC_IRQChannelPreemptionPriority = 2;
	n.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&n);

	//4. 使能USART1
	USART_Cmd(USART1, ENABLE);

}




int fputc(int c, FILE* stream)
{
    //把字符c,输出到 串口1

	while( (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET));
	USART_SendData(USART1, c);


	return c;
    
}

//串口1 的中断处理函数
void USART1_IRQHandler(void)
{
	uint8_t data;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		data = USART_ReceiveData(USART1);

		if (data == 0x01)
		{
			//..
			led_ctl(LED_D1, LED_ON);
		}
		else if (data == 0x02)
		{
			led_ctl(LED_D1, LED_OFF);
		}



		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


