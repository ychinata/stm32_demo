#include "stm32f10x.h"                 
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Serial.h"


/*
 * 配置项：Serial.h中的模式： SERIAL_NOT_PACKET_MODE
 * OLED引脚
 * USB TO TTL串口模块引脚：
    TXD <-> PA10(USART_1_RX)
    RXD <-> PA9 (USART_1_TX)
    GND <-> GND       
 */

/************************************************
Date: 2022.8.23
Author: h00421956
Func:串口发送字节 
************************************************/
int main(void) {

	// init
    LED_Init();
	OLED_Init();
	Serial_Init();
    
    //OLED_ShowString(1, 1, "Serial Recev:");
    // 4.OLED格式化显示  
    
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

        // 4.串口控制LED
        Serial_RxCtrlLed_Test();
	}
}






