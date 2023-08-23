#include "pwm.h"
#include "motor.h"

/**************************************************************************
�������ܣ�pwm��ʼ��-TB6612
��ڲ�����arr����Ϊһ��ʱ��Ƶ�ʵ����ֵ  psc�� Ԥ��Ƶֵ
����  ֵ����
Author: ��Ȥ
**************************************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;                 //����ṹ��GPIO_InitStructure
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;       //����ṹTIM_TimeBaseStructure
    TIM_OCInitTypeDef  TIM_OCInitStructure;               //����ṹTIM_OCInitStructure

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  //ʹ�ܶ�ʱ��4ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��PB�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;          //PB6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //IO���ٶ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     	//����ģʽ���
    GPIO_Init(GPIOB, &GPIO_InitStructure);              //GBIOB��ʼ��  

    TIM_TimeBaseStructure.TIM_Period = arr;    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //PWMTIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //����TIM����Ƚϼ���Ϊ��
    TIM_OCInitStructure.TIM_OutputState=	TIM_OutputState_Enable;   //�Ƚ����ʹ��

    TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����TIM_OCInitStructure��ָ���Ĳ�����ʼ������TIM4

    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);	    //ʹ��Ԥװ�ؼĴ���

    TIM_ARRPreloadConfig(TIM4, ENABLE);      //ʹ���Զ�װ������λ

    TIM_Cmd(TIM4, ENABLE);   //������ʱ��TIM4

}

/********************************************
* @brief      TIM2 PWM��ʼ��
* @param      arr
*             psc
* @retval     void
* @Author: https://www.cnblogs.com/xiaohuzaixue/p/17363415.html
//PWM����
 *PA1-T2C2
 *PA2-T2C3
*********************************************/
void TIM2_PWM_2ChannelInit(u16 arr, u16 psc)
{
  GPIO_InitTypeDef GPIO_InitStructure; //�������ų�ʼ���ṹ��
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //���嶨ʱ�жϽṹ��	
  TIM_OCInitTypeDef TIM_OCInitTypeStrue; //����PWM����ṹ��
	
  RCC_APB2PeriphClockCmd(MOTOR_IN_RCC, ENABLE); //ʹ��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʹ��ͨ�ö�ʱ��2ʱ��
	
  GPIO_InitStructure.GPIO_Pin=MOTOR_PIN_AIN1|MOTOR_PIN_BIN1;//��ʱ������PA1 CH2   PA2 CH3
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP; //�����������ģʽ��A0���Ÿ���
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; //��������ٶ�Ϊ
  GPIO_Init(MOTOR_IN_GPIO, &GPIO_InitStructure); //��ʼ������GPIO
	 
  TIM_TimeBaseInitStrue.TIM_Period=arr; //��������
  TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //Ԥ��Ƶϵ��
  TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
  TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //һ�㲻ʹ�ã�Ĭ��TIM_CKD_DIV1
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStrue); //��ʼ����ʱ��TIM2
	
  TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1; //PWMģʽ1����ʱ������С�� TIM_Pulseʱ�������Ч��ƽ
  TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //�����Ч��ƽΪ�ߵ�ƽ
  TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //ʹ��PWM���
  TIM_OCInitTypeStrue.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
  TIM_OC2Init(TIM2, &TIM_OCInitTypeStrue); //��ʼ����ʱ��2ͨ��2
  
  TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1;//PWMģʽ1��ʱ������С��TIM_Puls�����Ч��ƽ
  TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //�����Ч��ƽΪ�ߵ�ƽ
  TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //ʹ��PWM���
  TIM_OCInitTypeStrue.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
  TIM_OC3Init(TIM2, &TIM_OCInitTypeStrue); //��ʼ����ʱ��2ͨ��3

  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable); //CH2Ԥװ��ʹ��
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable); //CH3Ԥװ��ʹ��
  TIM_ARRPreloadConfig(TIM2, ENABLE); //Ԥװ��ʹ��
  TIM_Cmd(TIM2, ENABLE); //ʹ�ܶ�ʱ��TIM2
}

/********************************************
 *@Func: 2·PWM
 *PA6-T3C1
 *PA7-T3C2
 *@Para: speed��ȡֵ��Χ��0-PWM_SPEED_MAX��
 *@Ret: 
 *@Author: xxy
 *@Date: 2023.8.23
*********************************************/
void TIM3_PWM_2ChannelInit(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_IN_RCC, ENABLE);       //ʹ��GPIO����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    //���ø�����Ϊ�����������,���TIM3 CH1/2��PWM���岨��        GPIOA 6 7
    GPIO_InitStructure.GPIO_Pin = MOTOR_PIN_AIN1 | MOTOR_PIN_BIN1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_IN_GPIO, &GPIO_InitStructure);       //��ʼ��GPIOA
    //GPIO_SetBits(GPIOA, PWM_PIN_TIM3_A);

    //GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);//�ı�ָ���ܽŵ�ӳ��   

    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM1 Channel 1~2 PWMģʽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//TIM_OCPolarity_Low //�������:TIM����Ƚϼ��Ը�
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC1
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
    TIM_ARRPreloadConfig(TIM3,ENABLE);//ʹ��Ԥװ�ؼĴ���

    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}

