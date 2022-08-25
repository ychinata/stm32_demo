#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h> //可变参数头文件
#include "OLED.h"
#include "Serial.h"

/**************************
 *     宏定义
 ***************************/
#define SERIAL_PACKET_BGN 0xFF
#define SERIAL_PACKET_END 0xFE
#define SERIAL_PACKET_LEN 4


/**************************
 *     全局变量
 ***************************/

// 串口数据包
uint8_t g_Serial_TxPacket[SERIAL_PACKET_LEN]; //FF 01 02 03 04 FE
uint8_t g_Serial_RxPacket[SERIAL_PACKET_LEN];
// getrxdata删除
// 中断函数要修改

/***********************************************************
 *     外部函数定义    
 ***********************************************************/

/*****************************
Date: 2022.8.244
Author: h00421956
Func:串口接收中断函数(数据包)
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
		// 3个状态机
		// 状态0：等待数据包头
		if (RxState == 0) {
			if (RxData == SERIAL_PACKET_BGN) {
				RxState = 1;
				pRxPacket = 0;
			}
		// 状态1：接收数据包
		} else if (RxState == 1) {
			// 写入缓冲区
			// 缓冲区既有写入，又有读出，最好做原子保护，即读出完再写入
			g_Serial_RxPacket[pRxPacket] = RxData;
			pRxPacket ++;
			if (pRxPacket >= SERIAL_PACKET_LEN) {
				RxState = 2; // 数据接收完毕，进入下一个状态
			}
		// 状态2：等待数据包尾
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
Func:串口发送HEX数据包
************************************************/
void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendArray(g_Serial_TxPacket, 4);
	Serial_SendByte(0xFE);
	return;
}

/*****************************
Date: 2022.8.25
Author: h00421956
Func:串口发送HEX数据包测试
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
Func:串口接收HEX数据包测试
******************************/
void Serial_RecvPacket_Test(void) {

	if (Serial_GetRxFlag() == 1) {
		// 把接收数据发回给上位机
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
		// 接收上位机的数据getRxPacketData
		OLED_ShowString(3, 1, "RxPacket:");
		OLED_ShowHexNum(4, 1, g_Serial_RxPacket[0], 2);
		OLED_ShowHexNum(4, 3, g_Serial_RxPacket[1], 2);
		OLED_ShowHexNum(4, 5, g_Serial_RxPacket[2], 2);
		OLED_ShowHexNum(4, 7, g_Serial_RxPacket[3], 2);

	}
}

/*****************************
Date: 2022.8.2x
Author: h00421956
Func:串口HEX数据包收发（大量数据）SERIAL_PACKET_LEN改为8
******************************/



//保证数据不会被覆盖to do
// Serial_GetRxFlag里不要立刻清零
// 等操作完成之后，再清零
// 中断里，只有flag==0，才继续接收下一个数据包（其它设计可以参考此思路）
// 参考9-4 串口收发文本数据包

// 参考《STM32自学笔记》环形缓冲区



