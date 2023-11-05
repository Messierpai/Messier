

#ifndef __LED_H__
#define __LED_H__

//灯编号的宏
#define LED_D1  1
#define LED_D2  2
#define LED_D3  3
#define LED_D4  4
#define LED_ALL 100


//灯控制的宏
#define LED_ON 1    //开灯
#define LED_OFF 0   //关灯





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
void led_init(void);

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
void led_ctl(int ledN, int OnOff);

#endif 

