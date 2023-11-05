
#include "stm32f4xx.h"
#include "ds18b20.h"
#include "usart.h"
#include "stdio.h"




int main(void)
{

	short int temp;
  unsigned char DS18B20_ROM[8]; //存放DS18B20的64为ROM编码
	dsb_init();					 // DS18B20初始化
	uart1_init();
	printf("success");

	// 发送读取64为ROM编码的命令
	write_byte(0x33);

	// 循环读取64位ROM编码
	for (int i = 0; i < 8; i++)
	{
		DS18B20_ROM[i] = read_byte();
		printf("DS18B20_ROM[%d]=0x%X\n", i, DS18B20_ROM[i]);
	}

	while (1)
	{
		write_byte(0xCC); // 跳过ROM序列检测
		write_byte(0x44); // 启动一次温度转换(让总线上所有的DS18B20都转换温度)
		delay_ms(500);			 // 等待线上所有的DS18B20温度转换完成


		write_byte(0x55); // 发送匹配ROM的命令
		for (int i = 0; i < 8; i++)	 // 发送64位编码
		{
			write_byte(DS18B20_ROM[i]);
		}
		write_byte(0xBE);		 // 读取温度
		temp = read_byte();		 // 读取的温度低位数据
		temp |= read_byte() << 8; // 读取的温度高位数据
		printf("temp1=%d.%d\n", temp >> 4, temp & 0xF);
		printf("temp2=%f\n", temp * 0.0625);

		delay_ms(500);
	}
}
