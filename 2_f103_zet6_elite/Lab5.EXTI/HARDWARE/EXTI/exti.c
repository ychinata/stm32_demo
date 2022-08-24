#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"

//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序
// 2022.3.1
// h00421956
void EXTIX_Init(void)
{
	EXTI_InitTypeDef extiInitStruct;
	NVIC_InitTypeDef nvicInitStructure;
	
	KEY_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
	
	// 外部中断初始化
	extiInitStruct.EXTI_Line = EXTI_Line4;
	extiInitStruct.EXTI_LineCmd = ENABLE;
	extiInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&extiInitStruct);
	
	// 中断初始化
  	nvicInitStructure.NVIC_IRQChannel = EXTI4_IRQn;	//使能按键WK_UP所在的外部中断通道
  	nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  	nvicInitStructure.NVIC_IRQChannelSubPriority = 0x02;
  	nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能外部中断通道
  	NVIC_Init(&nvicInitStructure);
		
}

// 在startup_stm32f10x_hd.s里面
void EXTI4_IRQHandler(void) {

	// 延时防抖
	delay_ms(20);
	if (KEY0 == 0) {
		LED0 = !LED0;
		LED1 = !LED1;
	}
	// 每次中断进来必须清零
	EXTI_ClearITPendingBit(EXTI_Line4);
}

