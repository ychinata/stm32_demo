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
	OLED_Init();
	
	Serial_Init();
	
	// �����ֽ�
	Serial_SendByte(0x41);
	// Serial_SendByte('A'); // ����ͬ0x41
	
	// �����ֽ����飬��ʱ���ڹ���һ��ѡ��HEXģʽ
	
	uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45};	
	Serial_SendArray(MyArray, 4);
	
	// �����ַ���
	Serial_SendString("Helloworld!\r\n");
	//Serial_SendString("\r\nNum1=");
	
	// ���������ַ�
	Serial_SendNumber(12345, 5);
	
	// �ض���printf��ӡ������
	printf("\r\nNum2=%d\r\n", 222);
	
	//sprintf��ʽ����ӡ��ָ������
	char string[100]; 
	sprintf(string, "Num3=%d\r\n", 333);
	Serial_SendString(string);
	
	//���ڿɱ������ӡ
	//Serial_Printf("\r\nNum4=%d", 444);
	Serial_Printf("��ã����ںţ�%d\r\n", 444);
	//Serial_Printf("\r\n");
	
	while (1) {
		
	}
}
