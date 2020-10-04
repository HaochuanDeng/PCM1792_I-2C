#include "IIC.h"

/*******************/
//主设备: 89C52RC
//从设备: PCM1792A
/*******************/

//I^2C总线,总线初始化
//结果:SDA-->1
//     SCL-->1
//PS:代码未改动(郭天祥_P182)
void IIC_BUS_Init(void)
{
	SCL=1;   //SCL拉高
	delay(); //延时
	SDA=1;   //SDA拉高
	delay();
}

//I^2C总线,发送启动信号
//结果:SDA-->0
//     SCL-->1
//PS:代码未改动(郭天祥_P182)
void IIC_BUS_Start(void)
{
	SDA=1;  //SDA拉高
	delay();//延时
	SCL=1;  //SCL拉高
	delay();//延时
	SDA=0;  //SDA拉低
	delay();//延时
}

//I^2C总线,发送停止信号
//结果:SDA-->1
//     SCL-->1
//PS:代码未改动(郭天祥_P183)
void IIC_BUS_Stop(void)
{
	SDA=0;  //SDA拉低
	delay();//延时
	SCL=1;  //SDL拉高
	delay();//延时
	SDA=1;  //SDA拉高
	delay();//延时
}

//I^2C总线,主设备等待从设备应答
//结果:SDA-->1，从设备未应答
//     SDA-->0，从设备应答
//     SCL-->0
//PS:程序开始没有释放SDA,但是在其前面调用“主向从写字节”最后会释放SDA,代码未改动(郭天祥_P182)
void IIC_BUS_MasterWaitResp(void)
{
	uchar i=0;
	
//  SDA=1;  //释放SDA
	SCL=1;  //SCL拉高
	delay();//延时
	while((SDA==1) && (i<255))
		i++;  //等待从设备将SDA拉低
	SCL=0;  //主设备将SCL拉低
	delay();//延时
}

//I^2C总线，主设备向从设备应答
//结果:SDA-->0
//     SCL-->0
//PS:程序结束没有释放SDA,之后调用“主向从读”的开始会释放SDA,代码未改动(https://blog.csdn.net/xwwwj/article/details/105333223?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-5.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-5.channel_param)
void IIC_BUS_MasterRespToSlave(void)
{
	SCL=0;  //SCL拉低
	SDA=0;  //SDA拉低
	delay();//延时
	SCL=1;  //SCL拉高
	delay();//延时
	SCL=0;  //SCL拉低
//  SDA=1;  //释放SDA
}

//I^2C总线，主设备不向从设备应答
//结果:SDA-->1
//     SCL-->0
//PS:代码未改动(https://blog.csdn.net/xwwwj/article/details/105333223?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-5.channel_param&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-5.channel_param)
void IIC_BUS_MasterNotResp(void)
{
	SCL=0;  //SCL拉低
	SDA=1;  //SDA拉高
	delay();//延时
	SCL=1;  //SCL拉高
	delay();//延时
	SCL=0;  //SCL拉低
}

//I^2C总线，主设备向从设备写一个字节
//结果:SDA-->1
//     SCL-->0
//PS:代码未改动(郭天祥_P183)
void IIC_BUS_MasterWriteByte(uchar byte)
{
	uchar i,temp=byte;
	
	for(i=0; i<8; i++)
	{
		temp=temp<<1;//待发送字节最高位移入CY位,先发送最高位
		SCL=0;       //SCL拉低
		delay();     //延时
		SDA=CY;      //SDA送出CY
		delay();     //延时
		SCL=1;       //SCL拉高
		delay();     //延时
	}
	SCL=0;         //SCL拉低
	delay();       //延时
	SDA=1;         //释放SDA
	delay();       //延时
}

//I^2C总线，主设备向从设备读一个字节
//结果:SDA-->X
//     SCL-->0
//PS:代码未改动(郭天祥_P184)
uchar IIC_BUS_MasterReadByte(void)
{
	uchar i,k=0x00;
	
	SCL=0;  //SCL拉低
	delay();//延时
	SDA=1;  //释放SDA
	for(i=0; i<8; i++)
	{
		SCL=1;  //SCL拉高
		delay();//延时
		k=(k<<1)|SDA;//存入获取的比特
		SCL=0;  //SCL拉低
		delay();//延时
	}
	delay();  //延时
	return k; //返回获取的字节
}

//I^2C总线，向PCM1792的一个寄存器写入16位数据，其由MSB和LSB组成。
//调用此函数之前需要初始化I^2C总线
//返回值 -1 失败,该寄存器无法写入或该寄存器无效
//        0 成功
char IIC_BUS_WrtiePCM1792Reg(uchar addr, uchar MSB, uchar LSB)
{
	if((addr<0x10) || (addr>0x15))//检查寄存器写权限
		return WRITE_ERR;
	
	MSB &= 0x7F;                  //最高位清0
	IIC_BUS_Start();              //起始信号
	IIC_BUS_MasterWriteByte(PCM1792Addr & 0xFE);//写PCM1792的I^2C物理地址 + W位
	IIC_BUS_MasterWaitResp();     //等待从机应答
	IIC_BUS_MasterWriteByte(addr);//写入待写入的目标寄存器地址
	IIC_BUS_MasterWaitResp();     //等待从机应答
	IIC_BUS_MasterWriteByte(MSB); //写数据，高字节
	IIC_BUS_MasterWaitResp();     //等待从机应答
	IIC_BUS_MasterWriteByte(LSB); //写数据，低字节
	IIC_BUS_MasterWaitResp();     //等待从机应答
	IIC_BUS_Stop();               //停止信号
	delay();                      //起始信号与停止信号之间保持一定间隔
	return 0;
}

//I^2C总线，向PCM1792的一个寄存器读入16位数据，其由MSB和LSB组成。
//调用此函数之前需要初始化I^2C总线
//返回值 READ_ERR    失败，该寄存器无法读或该寄存器无效
//       其他        读到的16位数据
uint IIC_BUS_ReadPCM1792Reg(uchar addr)
{
	uchar MSB,LSB;
	uint date=0x0000;
	
	if((addr<0x10) || (addr>0x17))//检查寄存器读权限
		return READ_ERR;
	
	IIC_BUS_Start();              //起始信号
	IIC_BUS_MasterWriteByte(PCM1792Addr & 0xFE);//写PCM1792的I^2C物理地址 + W位
	IIC_BUS_MasterWaitResp();     //等待从机应答
	IIC_BUS_MasterWriteByte(addr);//写入待读取的目标寄存器地址
	IIC_BUS_MasterWaitResp();     //等待从机应答
	IIC_BUS_Start();              //重复起始信号
	IIC_BUS_MasterWriteByte(PCM1792Addr | 0x01);//写PCM1792的I^2C物理地址 + R位
	IIC_BUS_MasterWaitResp();     //等待从机应答
	MSB = IIC_BUS_MasterReadByte();//读数据，高字节
	IIC_BUS_MasterRespToSlave();  //主设备向从设备应答
	LSB = IIC_BUS_MasterReadByte();//读数据，低字节
	IIC_BUS_MasterNotResp();      //主设备不向从设备应答
	IIC_BUS_Stop();               //停止信号
	
  date = (((date|MSB)<<8) | LSB);	//组装为16位
	return date;
}









