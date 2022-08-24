#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h> //可变参数头文件
#include "OLED.h"


/**************************
 *     全局变量
 ***************************/
uint8_t g_Serial_RxData;
uint8_t g_Serial_RxFlag;

/**************************
 *     内部函数定义
 ***************************/

/*****
Date: 2022.8.23
Author: h00421956
Func:串口初始化
 *****/
void Serial_Init(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// tx配置
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// rx配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	// usart配置
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	// 中断配置
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // 使能串口接收中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	// usart使能
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

/************************************************
Date: 2022.8.24
Author: h00421956
Func:串口接收功能：获取全局变量接收标志位
************************************************/
uint8_t Serial_GetRxFlag(void)
{
	if (g_Serial_RxFlag == 1) {
		// 自动清零
		g_Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

/************************************************
Date: 2022.8.24
Author: h00421956
Func:串口接收功能：获取全局变量接收数据
************************************************/
uint8_t Serial_GetRxData(void)
{
	return g_Serial_RxData;
}

/*****
Date: 2022.8.244
Author: h00421956
Func:串口接收中断函数
 *****/
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		g_Serial_RxData = USART_ReceiveData(USART1);
		g_Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}


/***********************************************************
 *     外部函数定义    
 ***********************************************************/

/************************************************
Date: 2022.8.24
Author: h00421956
Func:串口发送功能测试：单片机->上位机
************************************************/
void Serial_TxOnly_Test(void) {
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
}

/************************************************
Date: 2022.8.24
Author: h00421956
Func:串口接收功能测试：上位机->单片机, 
		使用循环扫描检测接收数据
		函数要放在main函数while里
************************************************/
void Serial_RxOnlyScan_Test(void) {
	uint8_t rxData = 0;
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET) {
		rxData = USART_ReceiveData(USART1);
		OLED_ShowString(1, 1, "Serial Recev:");
		OLED_ShowHexNum(2, 1, rxData, 2);
	}		
}

/************************************************
Date: 2022.8.24
Author: h00421956
Func:串口接收发送功能测试：上位机->单片机->上位机
		使用循环扫描检测接收数据
		函数要放在main函数while里
************************************************/
void Serial_TRx_Test(void) {
	uint8_t rxData = 0;
	if (Serial_GetRxFlag() == 1) {
		// 接收上位机的数据
		rxData = Serial_GetRxData();
		// 把接收数据发回给上位机
		Serial_SendByte(rxData);
		OLED_ShowString(1, 1, "Serial Recev:");
		//OLED_ShowHexNum(2, 1, rxData, 2);
		OLED_ShowHexNum(2, 1, rxData-'A', 2);
		OLED_ShowHexNum(3, 1, 'A', 2);
		
	}
}
