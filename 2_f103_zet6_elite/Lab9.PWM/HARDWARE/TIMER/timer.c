#include "timer.h"
#include "led.h"
#include "usart.h"

/************************************************
Date: 2022.3.8
Author: h00421956
Func:  
//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc) {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	// ��ӳ��
	// �� TIM3_CH2 ֱ��ӳ�䵽�� PB5 �ϣ���ͨ��ǰ���ѧϰ������֪�� PB5 �� DS0 ��ֱ�����ӵ�
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);	
	
	//���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��	GPIOB.5
	GPIO_InitTypeDef gpioInitStru;
	gpioInitStru.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	gpioInitStru.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	gpioInitStru.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &gpioInitStru);//��ʼ��GPIO


	// ��ʼ��TIM3
	TIM_TimeBaseInitTypeDef  timeBaseStru;
	timeBaseStru.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	timeBaseStru.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	timeBaseStru.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	timeBaseStru.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &timeBaseStru); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	// ��ʼ��TIM3 Channel2 PWMģʽ
	TIM_OCInitTypeDef timOCInitStru;
	timOCInitStru.TIM_OCMode = TIM_OCMode_PWM2;
	timOCInitStru.TIM_OCPolarity = TIM_OCPolarity_High;
	timOCInitStru.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC2Init(TIM3, &timOCInitStru);
	
	// ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	// ʹ��TIM3
	TIM_Cmd(TIM3, ENABLE);

}


