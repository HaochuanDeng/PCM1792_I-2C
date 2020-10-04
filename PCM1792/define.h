#ifndef __DEFINE_H
#define __DEFINE_H

#include <REGX52.H>

#define uchar unsigned char
#define uint  unsigned int

#define PCM1792Addr 0x9C   //PCM1792的IIC物理地址,RW位默认为0,即写入
#define READ_ERR    0xAAAA //寄存器无法读返回值
#define WRITE_ERR   (-1)   //寄存器无法写返回值

//I^2C总线引脚定义
//89C52   PCM1792
//P2^0---_>SDA
//P2^1---_>SCL
sbit SDA = P2^0;
sbit SCL = P2^1;

#endif