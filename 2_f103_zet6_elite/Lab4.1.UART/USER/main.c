#include "stm32f10x.h"

 
/************************************************
Date: 2022.2.14
Author: h00421956
Func: USART
************************************************/
// 类似于uart_init(u32 bound)
void My_USART1_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStrue;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); //使能PORTA,PORTE时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE); //使能
	
	//USART1_TX   GPIOA.9初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//Usart1 NVIC 配置
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);	
	
	//USART 初始化设置
	USART_InitStrue.USART_BaudRate = 115200;
	USART_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStrue.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStrue.USART_Parity = USART_Parity_No;
	USART_InitStrue.USART_StopBits = USART_StopBits_1;
	USART_InitStrue.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStrue);

	USART_Cmd(USART1, ENABLE); // 使能串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启接收中断
	

}

// 中断服务函数
void USART1_IRQHandler(void) {
	u8 res;
	if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
		res = USART_ReceiveData(USART1);
		USART_SendData(USART1, res);
	}

}

int main(void)
{
	// 设置分组为2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	My_USART1_Init();
	while (1) {
		// null
	}
}
