
#ifndef __KEY_H__
#define __KEY_H__


typedef enum key_status_t
{
	KEY_DOWN,
	KEY_RELEASE
} key_status_t;

//按键的编号
#define KEY_S1 1
#define KEY_S2 2
#define KEY_S3 3
#define KEY_S4 4




/*
	key_init:初始化按键的几个GPIO引脚
			配置成 Input Mode(输入悬空)
	无参数、无返回值

	S1 --- PA0
	S2 --- PE2
	S3 --- PE3
	S4 --- PE4
*/
void key_init(void);


/*
	key_check:检测指定按键的状态
	@key_num: 指定要检测的按键的编号，如	：
			KEY_S1, KEY_S2, KEY_S3, KEY_S4
	返回值：
		返回按键的状态
			KEY_DOWN 按下
			KEY_RELEASE 松开
*/
key_status_t key_check(int key_num);


/*
	key_int_init:初始化按键的GPIO引脚(“中断版本”)
	无参数、无返回值

	S1 ---> PA0 --->EXTI0
	S2 ---> PE2 --->EXTI2
	S3 ---> PE3 --->EXTI3
	S4 ---> PE4 --->EXTI4
*/
void key_int_init(void);

#endif 
