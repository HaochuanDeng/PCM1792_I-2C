#ifndef __UART_H
#define __UART_H

#include <REGX52.H>
#include "define.h"
#include "DELAY.h"
#include "IIC.h"

void  UART_Init         (void);
void  UART_Info         (void);
void  IIC_Info          (void);
void  PCM1792_REG_STATUS(void);
void  Error_Code        (void);
void  Command_Format    (void);
void  SEND_Byte         (uchar byte);
uchar Receive_Byte      (void);
void  SEND_String       (uchar *p);
void  SEND_NewLine      (void);
//void  SEND_SP           (void);
uchar Wait_CMD          (void);

#endif