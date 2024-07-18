#include "gpio.h"
#include "motor.h"

void GPIO_Init_TB6612(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;            //����ṹ��GPIO_InitStructure

    RCC_APB2PeriphClockCmd(MOTOR_IN_RCC, ENABLE); // ʹ��PB�˿�ʱ��  
    GPIO_InitStructure.GPIO_Pin =   MOTOR_PIN_AIN1 | MOTOR_PIN_AIN2 | MOTOR_PIN_BIN1 | MOTOR_PIN_BIN2;	  //PB7 PB8
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     	//���죬��������������  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO���ٶ�
    GPIO_Init(MOTOR_IN_GPIO, &GPIO_InitStructure);          //GBIOB��ʼ��  	
}

/********************************************
 *@Func: GPIO��ʼ��TB6612��������PWMA/B
 *@Author: xxy
 *@Date: 2024.6.15
*********************************************/
void GPIO_Init_TB6612Pwm(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// ����TIMxѡ��ͬ��APBx
    RCC_APB2PeriphClockCmd(MOTOR_PWM_RCC , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
    
    //���ø�����Ϊ�����������,���TIM1 CH1 CH4��PWM���岨��
    GPIO_InitStructure.GPIO_Pin = MOTOR_PIN_PWMA | MOTOR_PIN_PWMB; //TIM_CH1 //TIM_CH4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_PWM_GPIO, &GPIO_InitStructure);
}

/********************************************
 *@Func: ����HAL����GPIO��ʼ������ʵ��
 *@Author: xxy
 *@Date: 2023.8.23
*********************************************/
void GPIO_Init_DRV8833(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(MOTOR_IN_RCC,ENABLE);

    GPIO_InitStructure.GPIO_Pin=MOTOR_PIN_AIN2|MOTOR_PIN_BIN2;//AIN2/BIN2��Ϊ�������
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_IN_GPIO,&GPIO_InitStructure);
    GPIO_ResetBits(MOTOR_IN_GPIO, MOTOR_PIN_AIN2|MOTOR_PIN_BIN2);
}
