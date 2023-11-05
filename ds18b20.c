#include "stm32f4xx.h"
#include "ds18b20.h"
#include "stdio.h"
#include "delay.h"

void write_odr(GPIO_TypeDef *GPIOx, short int Pin, unsigned char data)
{
    if (data == 1)
    {
        GPIO_SetBits(GPIOx, Pin);
    }
    else
    {
        GPIO_ResetBits(GPIOx, Pin);
    }
}
// 初始化
void dsb_init(void)
{
    // 使能时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // 初始化gpio
    GPIO_InitTypeDef d = {0};
    d.GPIO_Mode = GPIO_Mode_OUT;
    d.GPIO_OType = GPIO_OType_PP;
    d.GPIO_Pin = GPIO_Pin_3;
    d.GPIO_PuPd = GPIO_PuPd_NOPULL;
    d.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &d);

    // 操作引脚
    GPIO_SetBits(GPIOA, GPIO_Pin_3);

    delay_us(1); // 稍作延时

    GPIO_ResetBits(GPIOA, GPIO_Pin_3);

    delay_us(80); // 延时480到960us

    GPIO_SetBits(GPIOA, GPIO_Pin_3);

    int i = 0;
    while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)) // 等待DS18B20拉低总线
    {
        delay_us(100);
        i++;
        if (i > 5) // 约等待>5MS
        {
            printf("failed") ; // 初始化失败
        }
    }
}

void write_byte(unsigned char dat) // 写一个字节
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_3); // 每写入一位数据之前先把总线拉低1us
        delay_us(1);
        write_odr(GPIOA, GPIO_Pin_3, dat & 0x01); // 取最低位写入
        delay_us(10);                             // 延时68us，持续时间最少60us
        GPIO_SetBits(GPIOA, GPIO_Pin_3);          // 然后释放总线
        dat = dat >> 1;                           // 从低位开始写
    }
    delay_us(10);
}

unsigned char read_byte(void) // 读一个字节
{
    unsigned char i, dat = 0;
    for (i = 0; i < 8; i++)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_3); // 先将总线拉低1us
        delay_us(1);
        GPIO_SetBits(GPIOA, GPIO_Pin_3); // 然后释放总线
        delay_us(1);
        delay_us(1);
        delay_us(1);
        delay_us(1);
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
            dat = dat | 0x80; // 每次读一位
        dat = dat >> 1;       // 从最低位开始读
        delay_us(10);         // 读取完之后等待48us再接着读取下一个数
    }
    return dat;
}

unsigned int read_temper(void)
{
    unsigned char a, b;
    unsigned int t = 0;
    dsb_init();
    delay_us(15);
    write_byte(0xcc); // 跳过ROM操作命令
    write_byte(0x44); // 发送启动温度转换命令
    dsb_init();
    delay_us(15);
    write_byte(0xcc); // 跳过ROM操作命令
    write_byte(0xbe); // 发送读温度寄存器命令
    a = read_byte();  // 先读低八位
    b = read_byte();  // 再读高八位
    t = b;
    t <<= 8;   // 左移八位
    t = t | a; // t为16位的数，使高八位为b的值，低八位为a的值
    return t;  // 返回温度值
}

unsigned int temper_change(void)
{
    unsigned int temper;
    float tp;
    temper = read_temper();
	if (temper < 0) // 考虑负温度的情况
    {
        temper = temper - 1;
        temper = ~temper;
        tp = temper * 0.0625;    // 16位温度转换成10进制的温度
        temper = tp * 100 + 0.5; // 留两个小数点，并四舍五入
    }
    else
    {
        tp = temper * 0.0625;    // 16位温度转换成10进制的温度
        temper = tp * 100 + 0.5; // 留两个小数点，并四舍五入
    }
    return temper;
}