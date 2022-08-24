#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h> //�ɱ����ͷ�ļ�
#include "OLED.h"


/**************************
 *     ȫ�ֱ���
 ***************************/
uint8_t g_Serial_RxData;
uint8_t g_Serial_RxFlag;

/**************************
 *     �ڲ���������
 ***************************/

/*****
Date: 2022.8.23
Author: h00421956
Func:���ڳ�ʼ��
 *****/
void Serial_Init(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// tx����
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// rx����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	// usart����
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	// �ж�����
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // ʹ�ܴ��ڽ����ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	// usartʹ��
	USART_Cmd(USART1, ENABLE);
}


/************************************************
Date: 2022.8.23
Author: h00421956
Func:���ڷ����ֽ� 
************************************************/
void Serial_SendByte(uint8_t byte) 
{
	USART_SendData(USART1, byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/************************************************
Date: 2022.8.23
Author: h00421956
Func:���ڷ����ֽ����� 
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
Func:���ڷ����ַ��� 
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
Func:����x^y,��x��y�η� 
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
Func:���ڷ�������
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
Func:�ض���printf������fputc��printf�ĵײ�
************************************************/
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/************************************************
Date: 2022.8.24
Author: h00421956
Func:���� �ɱ������ʽ����ӡ
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
Func:���ڽ��չ��ܣ���ȡȫ�ֱ������ձ�־λ
************************************************/
uint8_t Serial_GetRxFlag(void)
{
	if (g_Serial_RxFlag == 1) {
		// �Զ�����
		g_Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

/************************************************
Date: 2022.8.24
Author: h00421956
Func:���ڽ��չ��ܣ���ȡȫ�ֱ�����������
************************************************/
uint8_t Serial_GetRxData(void)
{
	return g_Serial_RxData;
}

/*****
Date: 2022.8.244
Author: h00421956
Func:���ڽ����жϺ���
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
 *     �ⲿ��������    
 ***********************************************************/

/************************************************
Date: 2022.8.24
Author: h00421956
Func:���ڷ��͹��ܲ��ԣ���Ƭ��->��λ��
************************************************/
void Serial_TxOnly_Test(void) {
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
}

/************************************************
Date: 2022.8.24
Author: h00421956
Func:���ڽ��չ��ܲ��ԣ���λ��->��Ƭ��, 
		ʹ��ѭ��ɨ�����������
		����Ҫ����main����while��
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
Func:���ڽ��շ��͹��ܲ��ԣ���λ��->��Ƭ��->��λ��
		ʹ��ѭ��ɨ�����������
		����Ҫ����main����while��
************************************************/
void Serial_TRx_Test(void) {
	uint8_t rxData = 0;
	if (Serial_GetRxFlag() == 1) {
		// ������λ��������
		rxData = Serial_GetRxData();
		// �ѽ������ݷ��ظ���λ��
		Serial_SendByte(rxData);
		OLED_ShowString(1, 1, "Serial Recev:");
		//OLED_ShowHexNum(2, 1, rxData, 2);
		OLED_ShowHexNum(2, 1, rxData-'A', 2);
		OLED_ShowHexNum(3, 1, 'A', 2);
		
	}
}
