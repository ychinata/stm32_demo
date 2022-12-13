#include "stm32f10x.h"                 
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"


/*
 *配置项：Serial.h中的模式： SERIAL_NOT_PACKET_MODE

 */

/************************************************
Date: 2022.8.23
Author: h00421956
Func:串口发送字节 
************************************************/
int main(void) {

	// init
	OLED_Init();
	Serial_Init();
    
    //OLED_ShowString(1, 1, "Serial Recev:");
    
	// 0.串口发送功能测试
	//Serial_TxOnly_Test();
	
	// 3.串口发送数据包测试
	//Serial_SendPacket_Test();
			    
	while (1) {              	
		// 1.串口接收功能测试	
		//Serial_RxOnlyScan_Test();
		// 2.串口收发功能测试
		//Serial_TRx_Test();
		
		// 3.串口接收HEX数据包测试
		//Serial_RecvPacket_Test();
		//Serial_TRxPacket_Test();
		//OLED_ShowString(1, 1, "TxPacketRxPacket");        
	}
}






