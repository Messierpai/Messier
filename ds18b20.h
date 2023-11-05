#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "stm32f4xx.h"

void write_odr(GPIO_TypeDef *GPIOx, short int  Pin, unsigned char data);
void dsb_init(void);
void write_byte(unsigned char dat);
unsigned char read_byte(void);
unsigned int read_temper(void);
unsigned int temper_change(void);
#endif