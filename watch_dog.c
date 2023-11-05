

#include "stm32f4xx.h"
#include "led.h"

#define rIWDG_KR  *((volatile unsigned long*)0x40003000)
#define rIWDG_PR  *((volatile unsigned long*)0x40003004)
#define rIWDG_RLR  *((volatile unsigned long*)0x40003008)
#define rIWDG_SR  *((volatile unsigned long*)0x4000300C)


/*
	IWDG_Init:初始化并使能 “独立看门狗”

	IWDG_PR 采用32分频，独立看门狗的时钟频率为:
		LSI(32k)/32 = 1k Hz, T = 1ms
	IWDG_RLR = 0xFFF , 表示， 4096*1ms = 4096 ms 超时溢出
*/
void IWDG_Init(void)
{
	//1. 把 0X5555写入 IWDG_KR，破除IWDG_PR、IWDG_RLR的写保护功能
	rIWDG_KR = 0x5555;
	//IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable);

	//2. 采用32分频
	rIWDG_PR = 0x3;

	//IWDG_SetPrescaler( IWDG_Prescaler_32);


	//3. 设置重载寄存器
	rIWDG_RLR = 0xFFF; //计数得出理论上的溢出时间为 ： 4096 ms
	//IWDG_SetReload(0xFFF);


	//4. 把 重载寄存器中的值    加载到 独立看门狗的计数器中
	// "喂狗"
	rIWDG_KR = 0xAAAA;
	//IWDG_ReloadCounter();


	//5. Enable  IWDG
	rIWDG_KR = 0xCCCC;
	//IWDG_Enable();
}

 



//喂狗
void IWDG_Feed(void)
{
	rIWDG_KR = 0xAAAA;
	//IWDG_ReloadCounter();
}



#define rWWDG_CR  *((volatile unsigned long*)0x40002c00)
#define rWWDG_CFR  *((volatile unsigned long*)0x40002c04)



/*
	WWDG_Init:窗口看门狗的初始化并使能。

		WWDG 在 APB1总线上
			Fapb1 = 42Mhz
*/
void WWDG_Init(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

	//  WWDG_CFR   9    [8:7]            [6:0]
	//			 EWI	  WDGTB[1:0]	   W[6:0]
	// W[6:0] 窗口值
	// WDGTB[1:0] 预分频器



	//WWDG_CR  7    [6:0]
	//       WDGA   T[6:0]
	// WDGA 使能/禁止   窗口看门狗
	// T[6:0]  计数器

	

	WWDG_SetWindowValue(0X7F); //设置窗口值
	WWDG_SetPrescaler(WWDG_Prescaler_8); //设置预分频值 
	WWDG_SetCounter(0x7F); //设置计数器的值
	WWDG_EnableIT(); //使能中断

	NVIC_InitTypeDef n = {0};
	n.NVIC_IRQChannel = WWDG_IRQn;
	n.NVIC_IRQChannelCmd = ENABLE;
	n.NVIC_IRQChannelPreemptionPriority = 0;
	n.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&n);

	WWDG_Enable(0X7f);
	

	


	
}

void WWDG_FEED(void)
{

	unsigned long t = rWWDG_CR & 0x3F;
	unsigned long w = rWWDG_CFR &0x3F;

	if (t < w)
	{
		rWWDG_CR |= 0X2F;
	}

}

//窗口看门狗 中断
void WWDG_IRQHandler(void)
{
	if (WWDG_GetFlagStatus() == SET)
	{
		WWDG_SetCounter(0x7F); //设置计数器的值 "喂狗"

		WWDG_ClearFlag(); //清掉 EWI 标记

		//led_ctl(LED_D4, LED_ON);
	}
}

