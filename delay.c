
#include "stm32f4xx.h"

#define rSysTick_Ctrl   *((volatile unsigned long*)0xE000E010)
#define rSysTick_Load    *((volatile unsigned long*)0xE000E014)
#define rSysTick_Curr    *((volatile unsigned long*)0xE000E018)



//延时  n 毫秒
void delay_ms(int n)
{
	int i;
	// Ctrl[b0] = 0, disable SysTick Timer
	rSysTick_Ctrl &= ~(1 << 0);

	rSysTick_Ctrl |= 1 << 2; //使用 处理器的时钟  SystemCoreClock
	rSysTick_Ctrl &= ~(1 << 1); //disable systick中断

	

	//设置Load值
	//rSysTick_Load = SystemCoreClock/1000*n - 1;	// ??? 当n>99时，会溢出!!!
	//   n最大为  99 ms


	//设置一次延时为  50ms
	int k = 50; // k <= 99 ,when SystemCoreClock = 168M
				// k < (1 << 24) * 1000 /SystemCoreClock
	
	int m = n / k; //要延时多少个 k ms
	int r = n % k; //还剩余多少ms  r < k

	for (i = 0; i < m; i++)
	{
		rSysTick_Load = SystemCoreClock/1000*k - 1;
		rSysTick_Curr = 0;

		//enable SysTick Timer
		rSysTick_Ctrl |=  1 << 0;

		//等待定时器溢出
		while ((rSysTick_Ctrl & (1 << 16))  == 0);


		rSysTick_Ctrl &= ~(1 << 0); //disable timer
	}


	if (r)
	{
		rSysTick_Load = SystemCoreClock/1000*r - 1;
		rSysTick_Curr = 0;

		//enable SysTick Timer
		rSysTick_Ctrl |=  1 << 0;

		//等待定时器溢出
		while ((rSysTick_Ctrl & (1 << 16))  == 0);


		rSysTick_Ctrl &= ~(1 << 0); //disable timer
	}
}


//延时n 微秒
void delay_us(int n)
{
	int i;
	// Ctrl[b0] = 0, disable SysTick Timer
	rSysTick_Ctrl &= ~(1 << 0);

	rSysTick_Ctrl |= 1 << 2; //使用 处理器的时钟  SystemCoreClock
	rSysTick_Ctrl &= ~(1 << 1); //disable systick中断

	

	//设置Load值
	//rSysTick_Load = SystemCoreClock/1000000*n - 1;	// 
	//	 n最大为  99864 us


	//设置一次延时为  50000 us
	int k = 50000; // k <= 99864 ,when SystemCoreClock = 168M
				// k < (1 << 24) * 1000000 /SystemCoreClock
	
	int m = n / k; //要延时多少个 k us
	int r = n % k; //还剩余多少us	r < k

	for (i = 0; i < m; i++)
	{
		rSysTick_Load = SystemCoreClock/1000000*k - 1;
		rSysTick_Curr = 0;

		//enable SysTick Timer
		rSysTick_Ctrl |=  1 << 0;

		//等待定时器溢出
		while ((rSysTick_Ctrl & (1 << 16))	== 0);


		rSysTick_Ctrl &= ~(1 << 0); //disable timer
	}


	if (r)
	{
		rSysTick_Load = SystemCoreClock/1000000*r - 1;
		rSysTick_Curr = 0;

		//enable SysTick Timer
		rSysTick_Ctrl |=  1 << 0;

		//等待定时器溢出
		while ((rSysTick_Ctrl & (1 << 16))	== 0);


		rSysTick_Ctrl &= ~(1 << 0); //disable timer
	}

	
}




















