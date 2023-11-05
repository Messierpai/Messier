
#ifndef __BEEP_H__
#define __BEEP_H__


#define BEEP_ON 1
#define BEEP_OFF 0

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
void beep_init(void);



/*
	beep_ctl:控制蜂鸣器
	@OnOff: 是打开还是关闭蜂鸣器
		BEEP_ON 打开蜂鸣器
		BEEP_OFF 关闭蜂鸣器
*/
void beep_ctl(int OnOff);

#endif 
