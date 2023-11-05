#include "ds18b20.h"
 
/*
函数功能: DS18B20初始化
硬件连接: PB15
*/
void DS18B20_Init(void)
{
		RCC->APB2ENR|=1<<3; //PB
		GPIOB->CRH&=0x0FFFFFFF;
		GPIOB->CRH|=0x30000000;
		GPIOB->ODR|=1<<15; //上拉
}
 
 
/*
函数功能: 检测DS18B20设备是否存在
返回值  : 1表示设备不存在 0表示设备正常
*/
u8 DS18B20_CheckDevice(void)  //包含了复位脉冲、检测存在脉冲
{
		DS18B20_OUTPUT_MODE();//初始化为输出模式
		DS18B20_OUT=0;        //产生复位脉冲
		DelayUs(750);         //产生750us的低电平
		DS18B20_OUT=1;        //释放总线
		DelayUs(15);          //等待DS18B20回应
		if(DS18B20_CleckAck())//检测存在脉冲
		{
				return 1;
		}
		return 0;
}
 
/*
函数功能: 检测DS18B20设备的存在脉冲
返回值  : 1表示错误 0表示正常
*/
u8 DS18B20_CleckAck(void)
{
		u8 cnt=0;
		DS18B20_INPUT_MODE();//初始化为输入模式
		while(DS18B20_IN&&cnt<200) //等待DS18B20响应存在脉冲
		{
				DelayUs(1);
				cnt++;
		}
		if(cnt>=200)return 1; //错误
		
		cnt=0;
		while((!DS18B20_IN)&&cnt<240) //等待DS18B20释放总线
		{
				DelayUs(1);
				cnt++;
		}
		if(cnt>=240)return 1; //错误
		return 0;
}
 
 
/*
函数功能: 写一个字节
首先学会如何写一个位。
*/
void DS18B20_WriteByte(u8 cmd)
{
		u8 i;
		DS18B20_OUTPUT_MODE(); //初始化为输出模式
		for(i=0;i<8;i++)
	  {
				DS18B20_OUT=0;  //产生写时间间隙(写开始)
				DelayUs(2);
				DS18B20_OUT=cmd&0x01; //发送实际的数据位
				DelayUs(60);    //等待写完成
				DS18B20_OUT=1;  //释放总线，准备下一次发送
				cmd>>=1;        //继续发送下一位数据
		}
}
 
/*
函数功能: 读一个字节
首先学会如何读一个位。
*/
u8 DS18B20_ReadByte(void)
{
	 u8 i,data=0;	
	 for(i=0;i<8;i++)
	 {
			DS18B20_OUTPUT_MODE(); //初始化为输出模式
			DS18B20_OUT=0;  //产生读时间间隙(读开始)
			DelayUs(2);
			DS18B20_OUT=1;  //释放总线
			DS18B20_INPUT_MODE(); //初始化为输入模式
			DelayUs(8);    //等待DS18B20的数据输出
			data>>=1;      //高位补0，默认以0为准
		  if(DS18B20_IN) data|=0x80;
			DelayUs(60);
			DS18B20_OUT=1;  //释放总线,等待读取下一位数据
	 }
	 return data;
}
 
 
/*
函数功能: 读取一次DS18B20的温度数据
返 回 值: 读取的温度数据
考虑的情况:  总线上只是接了一个DS18B20的情况
*/
u16 DS18B20_ReadTemp(void)
{
	  u16 temp=0;
		u8 temp_H,temp_L;
	  DS18B20_CheckDevice();   //发送复位脉冲、检测存在脉冲
		DS18B20_WriteByte(0xCC); //跳过ROM序列检测
		DS18B20_WriteByte(0x44); //启动一次温度转换
		
		//等待温度转换完成
		while(DS18B20_ReadByte()!=0xFF){}
		
		DS18B20_CheckDevice();   //发送复位脉冲、检测存在脉冲
		DS18B20_WriteByte(0xCC); //跳过ROM序列检测
		DS18B20_WriteByte(0xBE); //读取温度
		
		temp_L=DS18B20_ReadByte(); //读取的温度低位数据
		temp_H=DS18B20_ReadByte(); //读取的温度高位数据
		temp=temp_L|(temp_H<<8);   //合成温度
		return temp;
}
 
 