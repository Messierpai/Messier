
#include "key.h"
#include "led.h"
#include "beep.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"


/*
	key_init:初始化按键的几个GPIO引脚
			配置成 Input Mode(输入悬空)
	无参数、无返回值

	S1 --- PA0
	S2 --- PE2
	S3 --- PE3
	S4 --- PE4
*/
void key_init(void)
{
	//1. 使能GPIO分组的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOA , ENABLE);
	
	
	//2. 配置(初始化)GPIO引脚
	GPIO_InitTypeDef pa = {0};
	pa.GPIO_Mode = GPIO_Mode_IN;
	//pa.GPIO_OType = 
	pa.GPIO_Pin = GPIO_Pin_0;
	pa.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pa.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &pa);


	GPIO_InitTypeDef pe = {0};
	pe.GPIO_Mode = GPIO_Mode_IN;
	//pe.GPIO_OType = 
	pe.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	pe.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pe.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &pe);

}

/*
	key_int_init:初始化按键的GPIO引脚(“中断版本”)
	无参数、无返回值

	S1 ---> PA0 --->EXTI0
	S2 ---> PE2 --->EXTI2
	S3 ---> PE3 --->EXTI3
	S4 ---> PE4 --->EXTI4
*/
void key_int_init(void)
{
//1. 中断源的配置
//  1.1
	//	RCC_AHB1 
	//	GPIO_Init 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitTypeDef pa0 = {0};
	pa0.GPIO_Mode = GPIO_Mode_IN;
	//pa0.GPIO_OType
	pa0.GPIO_Pin = GPIO_Pin_0;
	pa0.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pa0.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &pa0);
	

	GPIO_InitTypeDef pe234 = {0};
	pe234.GPIO_Mode = GPIO_Mode_IN;
	//pe234.GPIO_OType
	pe234.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	pe234.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pe234.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &pe234);


//  1.2
	//	RCC_APB2 
	//	SYSCFG_EXTILineConfig(uint8_t EXTI_PortSourceGPIOx, uint8_t EXTI_PinSourcex)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); //选择 PA0 ---> EXTI0
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2); //选择 PE2 ---> EXTI2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3); //选择 PE3 ---> EXTI3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4); //选择 PE4 ---> EXTI4

//  1.3
	//	RCC_APB2 
	//	EXTI_Init(EXTI_InitTypeDef * EXTI_InitStruct)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT, ENABLE);
	EXTI_InitTypeDef exti = {0};
	exti.EXTI_Line = EXTI_Line0 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
	exti.EXTI_LineCmd = ENABLE;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&exti);


//2. NVIC的配置
	//NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
	//NVIC_Init(NVIC_InitTypeDef * NVIC_InitStruct)
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	
	NVIC_InitTypeDef nvic = {0};
	nvic.NVIC_IRQChannel = EXTI0_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = EXTI2_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = EXTI3_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = EXTI4_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 2;
	nvic.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic);

}

/*
	key_check:检测指定按键的状态
	@key_num: 指定要检测的按键的编号，如	：
			KEY_S1, KEY_S2, KEY_S3, KEY_S4
	返回值：
		返回按键的状态
			KEY_DOWN 按下
			KEY_RELEASE 松开
*/
key_status_t key_check(int key_num)
{
	key_status_t s;

	switch (key_num)
	{
		case KEY_S1:
		{
			if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET)
			{
				s = KEY_RELEASE;
			}
			else 
			{
				s = KEY_DOWN;
			}
			break;
		}
		case KEY_S2:
		{
			if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_SET)
			{
				s = KEY_RELEASE;
			}
			else 
			{
				s = KEY_DOWN;
			}
			break;
		}
		case KEY_S3:
		{
			if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_SET)
			{
				s = KEY_RELEASE;
			}
			else 
			{
				s = KEY_DOWN;
			}
			break;
		}
		case KEY_S4:
		{
			if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_SET)
			{
				s = KEY_RELEASE;
			}
			else 
			{
				s = KEY_DOWN;
			}
			break;
		}
	}

	return s;
}



void EXTI0_IRQHandler (void)
{

	if (EXTI_GetITStatus(EXTI_Line0) == SET)
	{

		//...作相应的中断处理
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
		{
			//按下
			led_ctl(LED_D1, LED_ON);

			if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET)
			{
				beep_ctl(BEEP_ON);
			}
			else 
			{
				beep_ctl(BEEP_OFF);
			}
		}
		else 
		{
			//弹起
			led_ctl(LED_D1, LED_OFF);

			beep_ctl(BEEP_OFF);
		}




		//清掉EXTI0的中断
		EXTI_ClearITPendingBit(EXTI_Line0);
	}

}

void EXTI2_IRQHandler (void)
{

	if (EXTI_GetITStatus(EXTI_Line2) == SET)
	{

		//...作相应的中断处理
		if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_RESET)
		{
			//按下
			led_ctl(LED_D2, LED_ON);

			if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)
			{
				beep_ctl(BEEP_ON);
			}
			else 
			{
				beep_ctl(BEEP_OFF);
			}
		}
		else 
		{
			//弹起
			led_ctl(LED_D2, LED_OFF);

			beep_ctl(BEEP_OFF);
		}




		//清掉EXTI0的中断
		EXTI_ClearITPendingBit(EXTI_Line2);
	}

}

void EXTI3_IRQHandler (void)
{

	if (EXTI_GetITStatus(EXTI_Line3) == SET)
	{

		//...作相应的中断处理
		if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET)
		{
			//按下
			led_ctl(LED_D3, LED_ON);

			
		}
		else 
		{
			//弹起
			led_ctl(LED_D3, LED_OFF);

			
		}




		//清掉EXTI0的中断
		EXTI_ClearITPendingBit(EXTI_Line3);
	}

}


void EXTI4_IRQHandler (void)
{

	if (EXTI_GetITStatus(EXTI_Line4) == SET)
	{

		//...作相应的中断处理
		if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)
		{
			//按下
			led_ctl(LED_D4, LED_ON);

			
		}
		else 
		{
			//弹起
			led_ctl(LED_D4, LED_OFF);

			
		}




		//清掉EXTI0的中断
		EXTI_ClearITPendingBit(EXTI_Line4);
	}

}



