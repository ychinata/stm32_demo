#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"

//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������
// 2022.3.1
// h00421956
void EXTIX_Init(void)
{
	EXTI_InitTypeDef extiInitStruct;
	NVIC_InitTypeDef nvicInitStructure;
	
	KEY_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);
	
	// �ⲿ�жϳ�ʼ��
	extiInitStruct.EXTI_Line = EXTI_Line4;
	extiInitStruct.EXTI_LineCmd = ENABLE;
	extiInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	extiInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&extiInitStruct);
	
	// �жϳ�ʼ��
  	nvicInitStructure.NVIC_IRQChannel = EXTI4_IRQn;	//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	nvicInitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
  	nvicInitStructure.NVIC_IRQChannelSubPriority = 0x02;
  	nvicInitStructure.NVIC_IRQChannelCmd = ENABLE;	//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&nvicInitStructure);
		
}

// ��startup_stm32f10x_hd.s����
void EXTI4_IRQHandler(void) {

	// ��ʱ����
	delay_ms(20);
	if (KEY0 == 0) {
		LED0 = !LED0;
		LED1 = !LED1;
	}
	// ÿ���жϽ�����������
	EXTI_ClearITPendingBit(EXTI_Line4);
}

