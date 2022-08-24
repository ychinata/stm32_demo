#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h> //可变参数头文件

/************************************************
Date: 2022.8.23
Author: h00421956
Func:串口初始化
************************************************/
void Serial_Init(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}


/************************************************
Date: 2022.8.23
Author: h00421956
Func:串口发送字节 
************************************************/
void Serial_SendByte(uint8_t byte) 
{
	USART_SendData(USART1, byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/************************************************
Date: 2022.8.23
Author: h00421956
Func:串口发送字节数组 
************************************************/
void Serial_SendArray(uint8_t *byteArray, uint16_t length)
{
	uint16_t i;
	for (i = 0; i < length; i ++) {
		Serial_SendByte(byteArray[i]);
	}
}

/************************************************
Date: 2022.8.23
Author: h00421956
Func:串口发送字符串 
************************************************/
void Serial_SendString(char *string)
{
	uint8_t i;
	for (i = 0; string[i] != '\0'; i ++) {
		Serial_SendByte(string[i]);
	}
}

/************************************************
Date: 2022.8.24
Author: h00421956
Func:计算x^y,即x的y次方 
************************************************/

uint32_t Serial_Pow(uint32_t x, uint32_t y)
{
	uint32_t result = 1;
	while (y --) {
		result *= x;
	}
	return result;
}

/************************************************
Date: 2022.8.24
Author: h00421956
Func:串口发送数字
************************************************/
void Serial_SendNumber(uint32_t number, uint8_t length)
{
	uint8_t i;
	for (i = 0; i < length; i ++) {
		 
		Serial_SendByte(number / Serial_Pow(10, length - i - 1) % 10 + '0');
//		Serial_SendByte(number / Serial_Pow(10, length - i - 1) % 10);
	}
}

/************************************************
Date: 2022.8.24
Author: h00421956
Func:重定向printf函数，fputc是printf的底层
************************************************/
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/************************************************
Date: 2022.8.24
Author: h00421956
Func:串口 可变参数格式化打印
************************************************/
void Serial_Printf(char *format, ...)
{
	char string[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(string, format, arg);
	va_end(arg);
	Serial_SendString(string);
}
