#include <REGX52.H>
#include "define.h"
#include "UART.h"
#include "DELAY.h"
#include "IIC.h"

//临时变量
uchar i;
char  temp;

//串口接收缓冲区
extern uchar UART_Buf[4];

//主函数
void main(void)
{
//初始化阶段
	UART_Init();     //初始化串口
	UART_Info();     //显示串口信息
	IIC_BUS_Init();  //初始化I^2C总线
	IIC_Info();      //显示I^2C总线状态
	Command_Format();//显示命令格式
	Error_Code();    //显示错误码
	PCM1792_REG_STATUS();//显示PCM1792寄存器文件状态
	delayms(20);     //防止出现“未定义指令”    
	RI=0;            //同上，清除RI标志位    
  
//串口用户界面
	while(1)
	{
		i=Wait_CMD();              //从串口获取命令
		if(i==0)                   //写寄存器命令就绪
		{
			temp=IIC_BUS_WrtiePCM1792Reg(UART_Buf[1], UART_Buf[2], UART_Buf[3]);//写寄存器
			if(temp==WRITE_ERR)      //该寄存器无法写入
			{
				SEND_String("Reg Is Not Able To Write!(0x01)");
				SEND_NewLine();        //换行
			}
			else if(temp==0)         //写入成功，进行读取
			{
        PCM1792_REG_STATUS();  //显示PCM1792寄存器文件状态				
			}
		}
		else if(i==1)              //读寄存器命令就绪
		{
		  PCM1792_REG_STATUS();    //显示PCM1792寄存器文件状态
		}
		else if(i==2)              //未定义的命令
		{
			SEND_String("Undefined Command!(0x02)");
			SEND_NewLine();          //换行
		}
	}
}

