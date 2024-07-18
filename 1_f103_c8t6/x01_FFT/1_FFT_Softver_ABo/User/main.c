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
    float fnum = 12.5556;
	// init
    LED_Init();
	OLED_Init();
	Serial_Init();
    
    //OLED_ShowString(1, 1, "Serial Recev:");
    // 4.OLED��ʽ����ʾ  
    
    printf("���ڷ��͹��ܲ���\r\n");
    printf("data=%f\r\n", fnum);
    
	// 0.���ڷ��͹��ܲ���
	Serial_TxOnly_Test();
	
			    
	while (1) {              	

        
	}
}






