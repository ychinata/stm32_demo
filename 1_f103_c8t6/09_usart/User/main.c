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
	OLED_Init();
	
	Serial_Init();
	
	// 发送字节
	Serial_SendByte(0x41);
	// Serial_SendByte('A'); // 功能同0x41
	
	// 发送字节数组，此时串口工具一般选择HEX模式
	
	uint8_t MyArray[] = {0x42, 0x43, 0x44, 0x45};	
	Serial_SendArray(MyArray, 4);
	
	// 发送字符串
	Serial_SendString("Helloworld!\r\n");
	//Serial_SendString("\r\nNum1=");
	
	// 发送数字字符
	Serial_SendNumber(12345, 5);
	
	// 重定向printf打印到串口
	printf("\r\nNum2=%d\r\n", 222);
	
	//sprintf格式化打印到指定变量
	char string[100]; 
	sprintf(string, "Num3=%d\r\n", 333);
	Serial_SendString(string);
	
	//串口可变参数打印
	//Serial_Printf("\r\nNum4=%d", 444);
	Serial_Printf("你好，串口号：%d\r\n", 444);
	//Serial_Printf("\r\n");
	
	while (1) {
		
	}
}
