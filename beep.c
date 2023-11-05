
#include "beep.h"
#include "stm32f4xx.h"


/*
    beep_init:初始化beep的GPIO引脚
    无参数、无返回值

        beep灯的引脚配置成:
            Output mode
                Push-Pull输出推挽

            PuPdR 上下拉电阻，可以不用。

   beep - PF8
   	  write 1 to pf8 , beep on
   	  write 0 to pf8 , beep off
*/
void beep_init(void)
{
	//1. 使能GPIO分组的时钟
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF , ENABLE);
	
	
	//2. 配置(初始化)GPIO引脚
	GPIO_InitTypeDef p = {0};
	p.GPIO_Mode = GPIO_Mode_OUT;
	p.GPIO_OType = GPIO_OType_PP;
	p.GPIO_Pin = GPIO_Pin_8;
	p.GPIO_PuPd = GPIO_PuPd_NOPULL;
	p.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOF, &p);
	

	
	//3. 操作GPIO引脚. 初始时，默认beep off
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);

}

/*
	beep_ctl:控制蜂鸣器
	@OnOff: 是打开还是关闭蜂鸣器
		BEEP_ON 打开蜂鸣器
		BEEP_OFF 关闭蜂鸣器
*/
void beep_ctl(int OnOff)
{
	if (OnOff == BEEP_ON)
	{
		GPIO_SetBits(GPIOF, GPIO_Pin_8);
	}
	else 
	{
		GPIO_ResetBits(GPIOF, GPIO_Pin_8);
	}
}


