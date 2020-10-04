#include "UART.h"

//临时变量
uint x=0x0000;
uchar z;

//串口接收缓冲区
uchar UART_Buf[4]={0xFF,0xFF,0xFF,0xFF};

//PCM1792寄存器文件状态
uchar code  MAP_0[]="|------------------------------------------PCM1792 REG STATUS-------------------------------------------|";
uchar code  MAP_1[]="|-INFO-||----------------------MSB---------------------||----------------------LSB----------------------|";
uchar code  MAP_2[]="Addr DEC|B15   B14   B13   B12 | B11   B10    B9    B8 |  B7    B6    B5    B4 |  B3    B2    B1    B0  |";
uchar code  MAP_3[]="--------|----------------------|-----------------------|-----------------------|------------------------|";
uchar code  MAP_4[]="0x10 16 |R/W----0-----0-----1--|--0-----0-----0-----0--|-ATL7  ATL6  ATL5  ATL4| ATL3  ATL2  ATL1  ATL0 |";
uchar code  MAP_5[]="0x11 17 |R/W----0-----0-----1--|--0-----0-----0-----1--|-ATR7  ATR6  ATR5  ATR4| ATR3  ATR2  ATR1  ATR0 |";
uchar code  MAP_6[]="0x12 18 |R/W----0-----0-----1--|--0-----0-----1-----0--|-ATLD  FMT2  FMT1  FMT0| DMF1  DMF0  DME   MUTE |";
uchar code  MAP_7[]="0x13 19 |R/W----0-----0-----1--|--0-----0-----1-----1--|-REV   ATS1  ATS0  OPE | RSV   DFMS  FLT   INZD |";
uchar code  MAP_8[]="0x14 20 |R/W----0-----0-----1--|--0-----1-----0-----0--|-RSV   SRST  DSD   DFTH| MONO  CHSL  OS1   OS0  |";
uchar code  MAP_9[]="0x15 21 |R/W----0-----0-----1--|--0-----1-----0-----1--|-RSV   RSV   RSV   RSV | RSV   DZ1   DZ0   PCMZ |";
uchar code MAP_10[]="0x16 22 | R-----0-----0-----1--|--0-----1-----1-----0--|-RSV   RSV   RSV   RSV | RSV   RSV   ZFGR  ZFGL |";
uchar code MAP_11[]="0x17 23 | R-----0-----0-----1--|--0-----1-----1-----1--|-RSV   RSV   RSV   ID4 | ID3   ID2   ID1   ID0  |";
uchar code MAP_12[]="|-------------------------------------------------------------------------------------------------------|";

//命令格式
uchar code CMD_0[]="Write Command Format: W Addr MSB LSB";
uchar code CMD_1[]="Example:              0  11   11  AA";
uchar code CMD_2[]="Means: Write 0x11AA At 0x11";
uchar code CMD_3[]="---------------------------------------------";
uchar code CMD_4[]="Read Command Format:  R";
uchar code CMD_5[]="Example:              1";
uchar code CMD_6[]="Means: Read Whole Regs";
uchar code CMD_7[]="---------------------------------------------";

//错误码
uchar code ERR_0[]="CODE DESCRIPTION";
uchar code ERR_1[]="0x01 Reg Can't Be Written";
uchar code ERR_2[]="0x02 Undefined Command";
uchar code ERR_3[]="---------------------------------------------";

//初始化UART为方式0，并配置中断
void UART_Init(void)
{
	TMOD=0x20; //T1方式二，8位自动重装定时器
	TH1 =0xFD; //T1做波特率发生器，9600bps
	TL1 =0xFD; //同上
	TR1 =1;    //开启T1
	REN= 1;    //允许串口接收
	SM0= 0;    //串口方式一
	SM1= 1;    //同上
}

//显示串口信息
void UART_Info(void)
{
	SEND_String("UART ONLINE @ 9600_bps|CRC_None|DATA_8|STOP_1");
	SEND_NewLine();  //换行
}

//显示I^2C总线状态
void IIC_Info(void)
{
	SEND_String("I^2C BUS Init Successfully!");
	SEND_NewLine();  //换行
	SEND_String("---------------------------------------------");
	SEND_NewLine();  //换行
}

//显示PCM1792寄存器文件状态
void PCM1792_REG_STATUS(void)
{
	SEND_String(MAP_0);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_1);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_2);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_3);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_4);
	SEND_NewLine();
	x=IIC_BUS_ReadPCM1792Reg(0x10);
	SEND_String("          ");
	for(z=0; z<16; z++)
	{
		SEND_String((x&0x8000)? "1" : "0");
		x = x<<1;
		SEND_String("     ");
	}
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_5);
	SEND_NewLine();
	x=IIC_BUS_ReadPCM1792Reg(0x11);
	SEND_String("          ");
	for(z=0; z<16; z++)
	{
		SEND_String((x&0x8000)? "1" : "0");
		x = x<<1;
		SEND_String("     ");
	}
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_6);
	SEND_NewLine();
	x=IIC_BUS_ReadPCM1792Reg(0x12);
	SEND_String("          ");
	for(z=0; z<16; z++)
	{
		SEND_String((x&0x8000)? "1" : "0");
		x = x<<1;
		SEND_String("     ");
	}
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_7);
	SEND_NewLine();
	x=IIC_BUS_ReadPCM1792Reg(0x13);
	SEND_String("          ");
	for(z=0; z<16; z++)
	{
		SEND_String((x&0x8000)? "1" : "0");
		x = x<<1;
		SEND_String("     ");
	}
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_8);
	SEND_NewLine();
	x=IIC_BUS_ReadPCM1792Reg(0x14);
	SEND_String("          ");
	for(z=0; z<16; z++)
	{
		SEND_String((x&0x8000)? "1" : "0");
		x = x<<1;
		SEND_String("     ");
	}
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_9);
	SEND_NewLine();
	x=IIC_BUS_ReadPCM1792Reg(0x15);
	SEND_String("          ");
	for(z=0; z<16; z++)
	{
		SEND_String((x&0x8000)? "1" : "0");
		x = x<<1;
		SEND_String("     ");
	}
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_10);
	SEND_NewLine();
	x=IIC_BUS_ReadPCM1792Reg(0x16);
	SEND_String("          ");
	for(z=0; z<16; z++)
	{
		SEND_String((x&0x8000)? "1" : "0");
		x = x<<1;
		SEND_String("     ");
	}
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_11);
	SEND_NewLine();
	x=IIC_BUS_ReadPCM1792Reg(0x17);
	SEND_String("          ");
	for(z=0; z<16; z++)
	{
		SEND_String((x&0x8000)? "1" : "0");
		x = x<<1;
		SEND_String("     ");
	}
	SEND_NewLine();
	delayms(20);
	
	SEND_String(MAP_12);
	SEND_NewLine();
	delayms(20);
}

//展示错误码
void Error_Code(void)
{
	SEND_String(ERR_0);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(ERR_1);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(ERR_2);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(ERR_3);
	SEND_NewLine();
	delayms(20);
}

//展示命令格式
void Command_Format(void)
{
	SEND_String(CMD_0);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(CMD_1);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(CMD_2);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(CMD_3);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(CMD_4);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(CMD_5);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(CMD_6);
	SEND_NewLine();
	delayms(20);
	
	SEND_String(CMD_7);
	SEND_NewLine();
	delayms(20);
}

//发送一个字节
void SEND_Byte(uchar byte)
{
	SBUF = byte;
	while(!TI); //等待发送完毕
	TI = 0;     //TI清零
}

//接收一个字节
uchar Receive_Byte(void)
{
	uchar i;
	
	while(!RI);//等待接受到字节
	RI=0;      //RI清零
	i=SBUF;    //获取接收到的数据
	
	return i;
}

//发送字符串
void SEND_String(uchar *p)
{
	while(*p != '\0')
	{
		SEND_Byte(*p);
		p++;
	}
}

//发送换行符
void SEND_NewLine(void)
{
	SEND_Byte(0x0A);//0x0A代表"\n"
}

////发送空格
//void SEND_SP(void)
//{
//	SEND_Byte(0x20);//0x20代表" "
//}

//等待命令
//返回 0 写命令就绪
//     1 读命令就绪
//     2 未定义命令
uchar Wait_CMD(void)
{
	uchar i;
	
	UART_Buf[0]=Receive_Byte();    //接收第一个字节
	if(UART_Buf[0]==0x00)          //写命令
	{
		for(i=1;i<4;i++)
		{
			UART_Buf[i]=Receive_Byte();//接收剩余三个字节
		}
		return 0;
	}
	else if(UART_Buf[0]==0x01)     //读命令
	{
		return 1;
	}
	
	return 2;                      //未定义指令
}