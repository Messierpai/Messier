

#include "led.h"
#include "stm32f4xx.h"



/*
    led_init:初始化LED灯的GPIO引脚
    无参数、无返回值

        LED灯的引脚配置成:
            Output mode
                Push-Pull输出推挽

            PuPdR 上下拉电阻，可以不用。

    D1 -- PF9
    D2 -- PF10
    D3 -- PE13
    D4 -- PE14
        初始化时，灯默认为“灭”
*/
void led_init(void)
{
	//1. 使能GPIO分组的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF , ENABLE);


	//2. 配置(初始化)GPIO引脚
	GPIO_InitTypeDef p = {0};
	p.GPIO_Mode = GPIO_Mode_OUT;
	p.GPIO_OType = GPIO_OType_PP;
	p.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	p.GPIO_PuPd = GPIO_PuPd_NOPULL;
	p.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOF, &p);

	GPIO_InitTypeDef pe = {0};
	pe.GPIO_Mode = GPIO_Mode_OUT;
	pe.GPIO_OType = GPIO_OType_PP;
	pe.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	pe.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pe.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOE, &pe);



	//3. 操作GPIO引脚. 初始时，默认让四个LED灯 灭
	GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_SetBits(GPIOE, GPIO_Pin_13 | GPIO_Pin_14);
}


/*
    led_ctl: 灯控函数。控制某个灯或些灯 的 亮 or 灭
    @ledN: 灯的编号。表示您要控制哪个灯或哪些灯
        如： LED_D1, LED_D2, LED_D3, LED_D4,LED_ALL
    @OnOff: 灯的操作。
        如：
            LED_ON 开灯
            LED_OFF 关灯
    返回值：
        无
*/
void led_ctl(int ledN, int OnOff)
{

	switch (ledN)
	{
		case LED_D1:
		{
			if (OnOff == LED_ON)
			{
				GPIO_ResetBits(GPIOF, GPIO_Pin_9);
			}
			else 
			{
				GPIO_SetBits(GPIOF, GPIO_Pin_9);
			}
			break;
		}
		case LED_D2:
		{
			if (OnOff == LED_ON)
			{
				GPIO_ResetBits(GPIOF, GPIO_Pin_10);
			}
			else 
			{
				GPIO_SetBits(GPIOF, GPIO_Pin_10);
			}
			break;
		}
		case LED_D3:
		{
			if (OnOff == LED_ON)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_13);
			}
			else 
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_13);
			}
			break;
		}
		case LED_D4:
		{
			if (OnOff == LED_ON)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_14);
			}
			else 
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_14);
			}
			break;
		}
		case LED_ALL:
		{
			if (OnOff == LED_ON)
			{
				GPIO_ResetBits(GPIOE, GPIO_Pin_13 | GPIO_Pin_14);
				GPIO_ResetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
			}
			else 
			{
				GPIO_SetBits(GPIOE, GPIO_Pin_13 | GPIO_Pin_14);
				GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
			}
			break;
		}
		default:
			break;
	}
}


