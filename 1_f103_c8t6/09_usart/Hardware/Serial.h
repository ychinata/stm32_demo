#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

/**************************
 *     �궨��
 ***************************/
 #define SERIAL_PACKET_MODE 0 // ���ݾ������,�޸ĳ�0/1

/**************************
 *     ȫ�ֱ�������
 ***************************/
extern uint8_t g_Serial_RxFlag;
extern uint8_t g_Serial_TxPacket[];
extern uint8_t g_Serial_RxPacket[];


/**************************
 *     ��������
 ***************************/
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
uint8_t Serial_GetRxFlag(void);
void Serial_Printf(char *format, ...);
// ���Ժ���
void Serial_TxOnly_Test(void);
void Serial_RxOnlyScan_Test(void);
void Serial_TRx_Test(void);
void Serial_SendPacket_Test(void);
void Serial_RecvPacket_Test(void);

#endif
