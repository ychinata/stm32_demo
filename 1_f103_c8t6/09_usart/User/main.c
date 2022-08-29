#include "stm32f10x.h"                 
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

/************************************************
Date: 2022.8.23
Author: h00421956
Func:串口发送字节 
************************************************/
int main(void) {
	// init
	OLED_Init();
	Serial_Init();
	
	// 串口发送功能测试
	//Serial_TxOnly_Test();
	
	// 串口发送数据包测试
	//Serial_SendPacket_Test();
			
	while (1) {
		// 串口接收功能测试	
		//Serial_RxOnlyScan_Test();
		// 串口收发功能测试
		//Serial_TRx_Test();
		
		// 串口接收HEX数据包测试
		//Serial_RecvPacket_Test();
		Serial_TRxPacket_Test();
		
	}
}
