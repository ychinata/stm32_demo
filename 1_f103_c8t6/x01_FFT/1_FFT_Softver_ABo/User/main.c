#include "stm32f10x.h"                 
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "Serial.h"


/*
 * �����Serial.h�е�ģʽ�� SERIAL_NOT_PACKET_MODE
 * OLED����
 * USB TO TTL����ģ�����ţ�
    TXD <-> PA10(USART_1_RX)
    RXD <-> PA9 (USART_1_TX)
    GND <-> GND       
 */

/************************************************
Date: 2022.8.23
Author: h00421956
Func:���ڷ����ֽ� 
************************************************/
int main(void) {

	// init
    LED_Init();
	OLED_Init();
	Serial_Init();
    
    //OLED_ShowString(1, 1, "Serial Recev:");
    // 4.OLED��ʽ����ʾ  
    
	// 0.���ڷ��͹��ܲ���
	//Serial_TxOnly_Test();
	
	// 3.���ڷ������ݰ�����
	//Serial_SendPacket_Test();
			    
	while (1) {              	
		// 1.���ڽ��չ��ܲ���	
		//Serial_RxOnlyScan_Test();
		// 2.�����շ����ܲ���
		//Serial_TRx_Test();
		
		// 3.���ڽ���HEX���ݰ�����
		//Serial_RecvPacket_Test();
		//Serial_TRxPacket_Test();
		//OLED_ShowString(1, 1, "TxPacketRxPacket");     

        // 4.���ڿ���LED
        Serial_RxCtrlLed_Test();
	}
}






