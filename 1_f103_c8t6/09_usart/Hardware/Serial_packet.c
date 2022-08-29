#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h> //�ɱ����ͷ�ļ�
#include "OLED.h"
#include "Serial.h"

/**************************
 *     �궨��
 ***************************/
#define SERIAL_PACKET_BGN 0xFF
#define SERIAL_PACKET_END 0xFE
#define SERIAL_PACKET_LEN 8


/**************************
 *     ȫ�ֱ���
 ***************************/

// �������ݰ�
uint8_t g_Serial_TxPacket[SERIAL_PACKET_LEN]; //FF 01 02 03 04 FE
uint8_t g_Serial_RxPacket[SERIAL_PACKET_LEN];
// getrxdataɾ��
// �жϺ���Ҫ�޸�

/***********************************************************
 *     �ⲿ��������    
 ***********************************************************/

/*****************************
Date: 2022.8.244
Author: h00421956
Func:���ڽ����жϺ���(���ݰ�)
******************************/

void USART1_IRQHandler(void)
{
#ifndef SERIAL_PACKET_MODE	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		g_Serial_RxData = USART_ReceiveData(USART1);
		g_Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
#else	
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		uint8_t RxData = USART_ReceiveData(USART1);
		// 3��״̬��
		// ״̬0���ȴ����ݰ�ͷ
		if (RxState == 0) {
			if (RxData == SERIAL_PACKET_BGN) {
				RxState = 1;
				pRxPacket = 0;
			}
		// ״̬1���������ݰ�
		} else if (RxState == 1) {
			// д�뻺����
			// ����������д�룬���ж����������ԭ�ӱ���������������д��
			g_Serial_RxPacket[pRxPacket] = RxData;
			pRxPacket ++;
			if (pRxPacket >= SERIAL_PACKET_LEN) {
				RxState = 2; // ���ݽ�����ϣ�������һ��״̬
			}
		// ״̬2���ȴ����ݰ�β
		} else if (RxState == 2) {
			if (RxData == SERIAL_PACKET_END) {
				RxState = 0;
				g_Serial_RxFlag = 1;
			}
		}
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
#endif
}

/************************************************
Date: 2022.8.25
Author: h00421956
Func:���ڷ���HEX���ݰ�
************************************************/
void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendArray(g_Serial_TxPacket, SERIAL_PACKET_LEN);
	Serial_SendByte(0xFE);
	return;
}

/*****************************
Date: 2022.8.25
Author: h00421956
Func:���ڷ���HEX���ݰ�����
******************************/
void Serial_SendPacket_Test(void)
{
	g_Serial_TxPacket[0] = 0x01;
	g_Serial_TxPacket[1] = 0x02;
	g_Serial_TxPacket[2] = 0x03;
	g_Serial_TxPacket[3] = 0x04;
	
	Serial_SendPacket();
	return;
}

/*****************************
Date: 2022.8.25
Author: h00421956
Func:���ڽ���HEX���ݰ�����
******************************/
void Serial_RecvPacket_Test(void) {

	if (Serial_GetRxFlag() == 1) {
		// �ѽ������ݷ��ظ���λ��
		OLED_ShowString(1, 1, "TxPacket:");
		g_Serial_TxPacket[0] ++;
		g_Serial_TxPacket[1] ++;
		g_Serial_TxPacket[2] ++;
		g_Serial_TxPacket[3] ++;		
		Serial_SendPacket();
		OLED_ShowHexNum(2, 1, g_Serial_TxPacket[0], 2);
		OLED_ShowHexNum(2, 3, g_Serial_TxPacket[1], 2);
		OLED_ShowHexNum(2, 5, g_Serial_TxPacket[2], 2);
		OLED_ShowHexNum(2, 7, g_Serial_TxPacket[3], 2);		
		// ������λ��������getRxPacketData
		OLED_ShowString(3, 1, "RxPacket:");
		OLED_ShowHexNum(4, 1, g_Serial_RxPacket[0], 2);
		OLED_ShowHexNum(4, 3, g_Serial_RxPacket[1], 2);
		OLED_ShowHexNum(4, 5, g_Serial_RxPacket[2], 2);
		OLED_ShowHexNum(4, 7, g_Serial_RxPacket[3], 2);

	}
}

/*****************************
Date: 2022.8.29
Author: h00421956
Func:����HEX���ݰ��շ����������ݣ�SERIAL_PACKET_LEN��Ϊ8

******************************/
void Serial_TRxPacket_Test(void) {

	u32 i = 0;
	u32 buffLen = SERIAL_PACKET_LEN;
	u32 width = 2; // ÿ���ֽ���ռ��ʾ�Ŀ��
	
	if (Serial_GetRxFlag() == 1) {
		// �ѽ������ݷ��ظ���λ��
		OLED_ShowString(1, 1, "TxPacket:");
		// ��䷢������
		for (i = 0; i < buffLen; i++) {
			g_Serial_TxPacket[i]++;
		}	
		Serial_SendPacket();
		for (i = 0; i < buffLen; i++) {
			// ÿ2���һ���ֽ�aa|bb|cc|dd|
			OLED_ShowHexNum(2, width*i+1, g_Serial_TxPacket[i], width);
		}	
		
		// ������λ��������getRxPacketData
		OLED_ShowString(3, 1, "RxPacket:");
		for (i = 0; i < buffLen; i++) {
			OLED_ShowHexNum(4, width*i+1, g_Serial_RxPacket[i], width);
		}
	}
}


/*��֤���ݲ��ᱻ����to do
 Serial_GetRxFlag�ﲻҪ��������
 �Ȳ������֮��������
 �ж��ֻ��flag==0���ż���������һ�����ݰ���������ƿ��Բο���˼·��
 �ο�9-4 �����շ��ı����ݰ�
*/

// �ο���STM32��ѧ�ʼǡ����λ�����



