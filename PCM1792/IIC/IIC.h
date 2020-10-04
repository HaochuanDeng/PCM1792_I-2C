#ifndef __IIC_H
#define __IIC_H

#include <REGX52.H>
#include "define.h"
#include "DELAY.h"

void  IIC_BUS_Init             (void);
void  IIC_BUS_Start            (void);
void  IIC_BUS_Stop             (void);
void  IIC_BUS_MasterWaitResp   (void);
void  IIC_BUS_MasterRespToSlave(void);
void  IIC_BUS_MasterNotResp    (void);
void  IIC_BUS_MasterWriteByte  (uchar byte);
uchar IIC_BUS_MasterReadByte   (void);
char  IIC_BUS_WrtiePCM1792Reg  (uchar addr, uchar MSB, uchar LSB);
uint  IIC_BUS_ReadPCM1792Reg   (uchar addr);

#endif