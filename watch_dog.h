
#ifndef __WATCH_DOG_H__
#define __WATCH_DOG_H__


/*
	IWDG_Init:初始化并使能 “独立看门狗”

	IWDG_PR 采用32分频，独立看门狗的时钟频率为:
		LSI(32k)/32 = 1k Hz, T = 1ms
	IWDG_RLR = 0xFFF , 表示， 4096*1ms = 4096 ms 超时溢出
*/
void IWDG_Init(void);

//喂狗
void IWDG_Feed(void);



/*
	WWDG_Init:窗口看门狗的初始化并使能。

		WWDG 在 APB1总线上
			Fapb1 = 42Mhz
*/
void WWDG_Init(void);

#endif 
