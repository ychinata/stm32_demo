#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

/**************************
 *     宏定义
 ***************************/
 // 根据具体情况,修改成1:数据包模式/0:非数据包
 #define SERIAL_NOT_PACKET_MODE 1

/**************************
 *     全局变量声明
 ***************************/
extern uint8_t g_Serial_RxFlag;
extern uint8_t g_Serial_RxData;
extern uint8_t g_Serial_TxPacket[];
extern uint8_t g_Serial_RxPacket[];


/**************************
 *     函数声明
 ***************************/
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);

uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);

void Serial_Printf(char *format, ...);
// 测试函数
void Serial_TxOnly_Test(void);
void Serial_RxOnlyScan_Test(void);
void Serial_TRx_Test(void);
void Serial_SendPacket_Test(void);
void Serial_RecvPacket_Test(void);
void Serial_TRxPacket_Test(void);


#endif
