#include "stm32f10x.h"                 
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

/************************************************
Date: 2022.8.23
Author: h00421956
Func:���ڷ����ֽ� 
************************************************/
int main(void) {
	// init
	OLED_Init();
	Serial_Init();
	
	// ���ڷ��͹��ܲ���
	//Serial_TxOnly_Test();
	
	// ���ڷ������ݰ�����
	//Serial_SendPacket_Test();
			
	while (1) {
		// ���ڽ��չ��ܲ���	
		//Serial_RxOnlyScan_Test();
		// �����շ����ܲ���
		//Serial_TRx_Test();
		
		// ���ڽ���HEX���ݰ�����
		//Serial_RecvPacket_Test();
		Serial_TRxPacket_Test();
		
	}
}
